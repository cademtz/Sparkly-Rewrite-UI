#include "Menu.h"
#include "CDrawManager.h"
Menu gMenu;

#define keyDown GetAsyncKeyState
#define keyPress(x) keyDown(x) & 1
#define boolStr(boolean) string(boolean? "True" : "False")

CKey gKey;

// - Feel free to use whatever default width you want
#define GROUP_WIDTH 180

#include "Aimbot.h" // Getting the aimbot controls from its header
void Menu::CreateGUI()
{
	Tabs.AddTab(
		new Tab("Controls", {
			new Groupbox("Groupbox", {
				new Checkbox("Checkbox"),
				new Slider("Slider", 0, -10, 10, 1),
				new Listbox("Listbox", {"This", "is", "a", "list", "box"}),
				new KeyBind("Keybind", VK_SHIFT),
				new Space(5),
				new ColorPicker("Color picker", Color(64, 128, 255))
			}, GROUP_WIDTH)
		})
	);

	Tabs.AddTab(
		new Tab("Example", {
			new Groupbox("Aimbot", {
				&gAim.enabled,
				&gAim.shoot,
				new Space(5),
				&gAim.hitscan,
				&gAim.hitbox,
			}, GROUP_WIDTH),
			new Groupbox("Aim method", {
				&gAim.silent,
				&gAim.backtrack,
				&gAim.tick
			}, GROUP_WIDTH)
		})
	);
}

#define TAB_WIDTH 150
#define MENU_TOPBAR 30
//#define MARGIN 4

void Menu::Draw()
{
#pragma region Handle input
	if (key == VK_INSERT || key == VK_F11)
		enabled = !enabled;

	if (!enabled)
		return;

	style->WaterMark("Sparkly Rewrite", 20, 20);

	static bool dragging = false;

	if (mb == e_mb::lclick && mouseOver(pos.x, pos.y, scale.x, MENU_TOPBAR))
		dragging = true;
	else if (mb != e_mb::ldown)
		dragging = false;

	if (dragging && focus == 0)
	{
		pos.x += mouse.x - pmouse.x;
		pos.y += mouse.y - pmouse.y;
	}

	POINT _pos = pos, _scale = scale;
	_scale.y += MENU_TOPBAR;
#pragma endregion

#pragma region Main window
	int topbar = style->TopBar(_pos.x, _pos.y, _scale.x, "Main window");

	// Re-adjusting pos and scale for easy coding
	_pos.y += topbar, _scale.y -= topbar;

	// Tab region
	gDraw.DrawRect(_pos.x, _pos.y, TAB_WIDTH, _scale.y, Color(25));
	// Dividing line
	gDraw.DrawRect(_pos.x + TAB_WIDTH - 2, _pos.y, 2, _scale.y, Color(20));

	Tabs.SetPos(_pos.x, _pos.y + topbar);
	Tabs.SetWidth(TAB_WIDTH);
	Tabs.HandleInput();
	Tabs.Draw(false);

	// Control region
	gDraw.DrawRect(_pos.x + TAB_WIDTH, _pos.y, _scale.x - TAB_WIDTH, _scale.y, Color(36, 36, 42));
	// Re-adjusting pos and scale again
	_pos.x += TAB_WIDTH + 3, _scale.x = scale.x - (_pos.x - pos.x);

#pragma endregion

#pragma region Controls
	if (Tabs.active)
	{
		int cx = _pos.x + 13, cy = _pos.y + 12;
		int maxWidth = 0;
		vector<BaseControl*> controls = Tabs.active->GetChildren();
		for (size_t i = 0; i < controls.size(); i++)
		{
			if (controls[i]->flags & nodraw)
				continue;

			if (cy + controls[i]->GetHeight() > scale.y + _pos.y - 12)
				cy = _pos.y + 12, cx += 13 + maxWidth + 10, maxWidth = 0;

			if (controls[i]->GetWidth() > maxWidth)
				maxWidth = controls[i]->GetWidth();
			controls[i]->SetPos(cx, cy);

			bool over = mouseOver(cx, cy, controls[i]->GetWidth(), controls[i]->GetHeight());
			bool getInput = !(controls[i]->flags & noinput) && over && !IsDialogOpen();
			if (getInput)
				controls[i]->HandleInput();

			controls[i]->Draw(getInput);

			cy += controls[i]->GetHeight() + SPACING;
		}
	}
#pragma endregion

#pragma region Dialogs
	size_t last = dialogs.size() - 1;
	if (dialogs.size() > 1)
	{
		e_mb new_mb = mb;
		e_mw new_mw = mw;
		POINT new_mouse = mouse, new_pmouse = pmouse;

		// Enforce focus so that only the last dialog gets to use these variables
		mb = e_mb::null, mw = e_mw::null, mouse = pmouse = { 0, 0 };

		for (size_t i = 0; i < last; i++)
		{
			if (dialogs[i] == nullptr)
				continue;

			dialogs[i]->Draw(dialogs[i]->data, i + 1);
		}
		mb = new_mb, mw = new_mw, mouse = new_mouse, pmouse = new_pmouse;
		dialogs[last]->Draw(dialogs[last]->data, last + 1);
	}
	else if (!last)
		dialogs[last]->Draw(dialogs[last]->data, last + 1);

	if (key == VK_ESCAPE && dialogs.size())
		dialogs.pop_back();
#pragma endregion
}

bool Menu::mouseOver(int x, int y, int w, int h)
{
	return mouse.x >= x && mouse.x <= x + w && mouse.y >= y && mouse.y <= y + h;
}

void Menu::GetInput()
{
	int mx = 0, my = 0;
	gInts.Surface->GetCursorPosition(mx, my);

	pmouse = mouse;
	mouse = { mx, my };

	if (keyDown(VK_LBUTTON))
	{
		if (mb == e_mb::lclick || mb == e_mb::ldown)
			mb = e_mb::ldown;
		else
			mb = e_mb::lclick;
	}
	else if (keyDown(VK_RBUTTON))
	{
		if (mb == e_mb::rclick || mb == e_mb::rdown)
			mb = e_mb::rdown;
		else
			mb = e_mb::rclick;
	}
	else
		mb = e_mb::null;
}

void Menu::EndInput()
{
	// Reseting Window message variables so they won't stick
	mw = e_mw::null;
	key = NULL;
}

LRESULT __stdcall Hooked_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_MOUSEWHEEL:
		if ((int)wParam < 0)
			gMenu.mw = e_mw::up;
		else
			gMenu.mw = e_mw::down;
		break;
	case WM_KEYDOWN:
		if (wParam > 255)
			break;
		gMenu.keys[wParam] = true, gMenu.last_key = wParam, gMenu.key = wParam;

		break;
	case WM_KEYUP:
		if (wParam > 255)
			break;
		gMenu.keys[wParam] = false;
		if (gMenu.last_key == wParam)
			gMenu.last_key = NULL;

		/*break;
		case WM_LBUTTONDOWN:
		if (wParam == MK_LBUTTON)
		gMenu.key = (e_key)VK_LBUTTON;
		break;
		case WM_RBUTTONDOWN:
		if (wParam == MK_RBUTTON)
		{
		gKey.key = (e_key)VK_RBUTTON;
		break;
		}
		case WM_MBUTTONDOWN:
		if (wParam == MK_MBUTTON)
		gKey.key = (e_key)VK_MBUTTON;*/
	}

	return CallWindowProc(gMenu.windowProc, hWnd, uMsg, wParam, lParam);
}

void Menu::OpenDialog(Dialog& dlg)
{
	dialogs.push_back(&dlg);
	focus = dialogs.size();
}
void Menu::CloseDialog(size_t Index)
{
	if (Index == 0)
		return;

	Index--;
	if (Index >= dialogs.size())
		return;

	dialogs.erase(dialogs.begin() + Index);
	focus = dialogs.size();
}