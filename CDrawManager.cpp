#include "CDrawManager.h"
//===================================================================================
CDrawManager gDraw;

#define ESP_HEIGHT 14
//===================================================================================
void CDrawManager::Initialize() // Create font IDs here
{
	if (gInts.Surface == NULL)
		return;

	gFonts.Initialize();
	gTex.Initialize();

	Reload();
}
void CDrawManager::Reload() // Create (or reload) your font styles
{
	if (gInts.Surface == NULL)
		return;

	gInts.Engine->GetScreenSize(gScreenSize.width, gScreenSize.height);

	gFonts.Reload();
	gTex.Reload();
}
//===================================================================================
void CDrawManager::DrawString(int x, int y, Color color, string text, HFont font)
{
	DrawString(x, y, color, text.c_str(), font);
}
void CDrawManager::DrawString(int x, int y, Color color, const char *pszText, HFont font)
{
	if (pszText == NULL)
		return;

	va_list va_alist;
	char szBuffer[1024] = { '\0' };
	wchar_t szString[1024] = { '\0' };

	va_start(va_alist, pszText);
	vsprintf_s(szBuffer, pszText, va_alist);
	va_end(va_alist);

	wsprintfW(szString, L"%S", szBuffer);

	gInts.Surface->DrawSetTextPos(x, y);
	gInts.Surface->DrawSetTextFont(font);
	gInts.Surface->DrawSetTextColor(color[0], color[1], color[2], color[3]);
	gInts.Surface->DrawPrintText(szString, wcslen(szString));
}
//===================================================================================
byte CDrawManager::GetESPHeight( )
{
	return ESP_HEIGHT;
}
//===================================================================================
void CDrawManager::DrawLine(int x, int y, int x1, int y1, Color color)
{
	gInts.Surface->DrawSetColor(color[0], color[1], color[2], color[3]);
	gInts.Surface->DrawLine(x, y, x1, y1);
}
//===================================================================================
void CDrawManager::DrawRect(int x, int y, int w, int h, Color color)
{
	gInts.Surface->DrawSetColor(color[0], color[1], color[2], color[3]);
	gInts.Surface->DrawFilledRect(x, y, x + w, y + h);
}
//===================================================================================
void CDrawManager::OutlineRect(int x, int y, int w, int h, Color color)
{
	gInts.Surface->DrawSetColor(color[0], color[1], color[2], color[3]);
	gInts.Surface->DrawOutlinedRect(x, y, x + w, y + h);
}
//===================================================================================
void CDrawManager::DrawBox(Vector vOrigin, Color color, int box_width, int radius)
{
	Vector vScreen;

	if (!WorldToScreen(vOrigin, vScreen))
		return;

	int radius2 = radius << 1;

	OutlineRect(vScreen.x - radius + box_width, vScreen.y - radius + box_width, radius2 - box_width, radius2 - box_width, 0x000000FF);
	OutlineRect(vScreen.x - radius - 1, vScreen.y - radius - 1, radius2 + (box_width + 2), radius2 + (box_width + 2), 0x000000FF);
	DrawRect(vScreen.x - radius + box_width, vScreen.y - radius, radius2 - box_width, box_width, color);
	DrawRect(vScreen.x - radius, vScreen.y + radius, radius2, box_width, color);
	DrawRect(vScreen.x - radius, vScreen.y - radius, box_width, radius2, color);
	DrawRect(vScreen.x + radius, vScreen.y - radius, box_width, radius2 + box_width, color);
}
//===================================================================================
void CDrawManager::DrawPolygon(int count, Vertex_t* verts, int texture)
{
	//static int Texture = gInts.Surface->CreateNewTextureID(true); // need to make a texture with procedural true
	//unsigned char const buffer[4] = { RED(dwColor), GREEN(dwColor), BLUE(dwColor), ALPHA(dwColor) }; // r,g,b,a

	//gInts.Surface->DrawSetTextureRGBA(Texture, buffer, 1, 1); //Texture, char array of texture, width, height
	gInts.Surface->DrawSetColor(255, 255, 255, 255); // keep this full color and opacity use the RGBA @top to set values.
	gInts.Surface->DrawSetTexture(texture); // bind texture

	gInts.Surface->DrawTexturedPolygon(count, verts, true);
}
//===================================================================================
//===================================================================================
void CDrawManager::DrawPolygon(int count, Vertex_t* verts, HTexture texture)
{
	gInts.Surface->DrawSetColor(255, 255, 255, 255); // keep this full color and opacity use the RGBA @top to set values.
	gInts.Surface->DrawSetTexture(texture); // bind texture
	gInts.Surface->DrawTexturedPolygon(count, verts);
}
//===================================================================================
bool CDrawManager::WorldToScreen( Vector &vOrigin, Vector &vScreen )
{
	const matrix3x4& worldToScreen = gInts.Engine->WorldToScreenMatrix(); //Grab the world to screen matrix from CEngineClient::WorldToScreenMatrix

	float w = worldToScreen[3][0] * vOrigin[0] + worldToScreen[3][1] * vOrigin[1] + worldToScreen[3][2] * vOrigin[2] + worldToScreen[3][3]; //Calculate the angle in compareson to the player's camera.
	vScreen.z = 0; //Screen doesn't have a 3rd dimension.

	if( w > 0.001 ) //If the object is within view.
	{
		float fl1DBw = 1 / w; //Divide 1 by the angle.
		vScreen.x = (gScreenSize.width / 2) + ( 0.5 * ((worldToScreen[0][0] * vOrigin[0] + worldToScreen[0][1] * vOrigin[1] + worldToScreen[0][2] * vOrigin[2] + worldToScreen[0][3]) * fl1DBw) * gScreenSize.width + 0.5); //Get the X dimension and push it in to the Vector.
		vScreen.y = (gScreenSize.height / 2) - ( 0.5 * ((worldToScreen[1][0] * vOrigin[0] + worldToScreen[1][1] * vOrigin[1] + worldToScreen[1][2] * vOrigin[2] + worldToScreen[1][3]) * fl1DBw) * gScreenSize.height + 0.5); //Get the Y dimension and push it in to the Vector.
		return true;
	}

	return false;
}