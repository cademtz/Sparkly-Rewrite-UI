#include "SDK.h"
#include "Panels.h"
#include "CDrawManager.h"

#include "Menu.h"

CScreenSize gScreenSize;
//===================================================================================
void __fastcall Hooked_PaintTraverse( PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce )
{
	try
	{
		VMTManager& hook = VMTManager::GetHook(pPanels); //Get a pointer to the instance of your VMTManager with the function GetHook.
		hook.GetMethod<void(__thiscall*)(PVOID, unsigned int, bool, bool)>(gOffsets.iPaintTraverseOffset)(pPanels, vguiPanel, forceRepaint, allowForce); //Call the original.

		const char* szName = gInts.Panels->GetName(vguiPanel);
		static unsigned int FocusOverlay;

		if (!FocusOverlay)
		{
			if (szName[0] == 'F' && szName[5] == 'O' && szName[12] == 'P')
			{
				FocusOverlay = vguiPanel;
				Intro();
			}
		}
		else
			gInts.Panels->SetTopmostPopup(FocusOverlay, true);

		if (FocusOverlay != vguiPanel || gInts.Engine->IsDrawingLoadingImage())
			return;

		// ========== This can be improved. It just prevents screen resolution changes from destroying all fonts ========== //
		CScreenSize newSize;
		gInts.Engine->GetScreenSize(newSize.width, newSize.width);
		if (newSize.width != gScreenSize.width || newSize.height != gScreenSize.height)
			gDraw.Reload();
		// ========== Update your input FIRST to enable usage throughout your program ========== //
		gMenu.GetInput();
		// - Other cheat code here that requires input can go here
		// ========== Menu code must be called AFTER everything else draws ========== //
		gMenu.Draw();
		gInts.Panels->SetMouseInputEnabled(vguiPanel, gMenu.enabled);
		// ========== I may or may not remove this and find a better method. Call after you're done with input ========== //
		gMenu.EndInput();
	}
	catch(...)
	{
		Log::Fatal("Failed PaintTraverse");
	}
}
//===================================================================================

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !! Do NOT forget to initialize your menu, fonts, textures, etc.	!!
// !! This is required for the menu to work							!!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

void Intro( void )
{
	try
	{
		gDraw.Initialize(); //Initalize the drawing class.
		gMenu.CreateGUI();
		InitTextures();

		Log::Msg("Injection Successful"); //If the module got here without crashing, it is good day.
	}
	catch(...)
	{
		Log::Fatal("Failed Intro");
	}
}