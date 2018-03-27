#include "SDK.h"
#include "Client.h"
#include "Panels.h"

#include "Menu.h"

COffsets gOffsets;
CPlayerVariables gPlayerVars;
CInterfaces gInts;

CreateInterface_t EngineFactory = NULL;
CreateInterface_t ClientFactory = NULL;
CreateInterface_t VGUIFactory = NULL;
CreateInterface_t VGUI2Factory = NULL;

DWORD WINAPI dwMainThread( LPVOID lpArguments )
{
	if (gInts.Client != NULL) //Prevent repeat callings.
		return 0;

	//Gottammove those factorys up.
	//Grab the factorys from their resptive module's EAT.
	ClientFactory = ( CreateInterfaceFn ) GetProcAddress( gSignatures.GetModuleHandleSafe( "client.dll" ), "CreateInterface" );
	gInts.Client = ( CHLClient* )ClientFactory( "VClient017", NULL);
	XASSERT(gInts.Client);
	gInts.EntList = ( CEntList* ) ClientFactory( "VClientEntityList003", NULL );
	XASSERT(gInts.EntList);
	EngineFactory = ( CreateInterfaceFn ) GetProcAddress( gSignatures.GetModuleHandleSafe( "engine.dll" ), "CreateInterface" );
	gInts.Engine = ( EngineClient* ) EngineFactory( "VEngineClient013", NULL );
	XASSERT(gInts.Engine);
	VGUIFactory = ( CreateInterfaceFn ) GetProcAddress( gSignatures.GetModuleHandleSafe( "vguimatsurface.dll" ), "CreateInterface" );
	gInts.Surface = ( ISurface* ) VGUIFactory( "VGUI_Surface030", NULL );
	XASSERT(gInts.Surface);

	//Setup the Panel hook so we can draw.
	if( !gInts.Panels )
	{
		VGUI2Factory = (CreateInterfaceFn)GetProcAddress(gSignatures.GetModuleHandleSafe("vgui2.dll"), "CreateInterface");
		gInts.Panels = ( IPanel* ) VGUI2Factory( "VGUI_Panel009", NULL );
		XASSERT( gInts.Panels );

		if( gInts.Panels )
		{
			VMTBaseManager* panelHook = new VMTBaseManager(); //Setup our VMTBaseManager for Panels.
			panelHook->Init(gInts.Panels);
			panelHook->HookMethod(&Hooked_PaintTraverse, gOffsets.iPaintTraverseOffset);
			panelHook->Rehook();
		}
	}

	DWORD dwClientModeAddress = gSignatures.GetClientSignature("8B 0D ? ? ? ? 8B 02 D9 05");
	XASSERT(dwClientModeAddress);
	gInts.ClientMode = **(ClientModeShared***)(dwClientModeAddress + 2);
	LOGDEBUG("g_pClientModeShared_ptr client.dll+0x%X", (DWORD)gInts.ClientMode - dwClientBase);

	VMTBaseManager* clientModeHook = new VMTBaseManager(); //Setup our VMTBaseManager for ClientMode.
	clientModeHook->Init(gInts.ClientMode);
	clientModeHook->HookMethod(&Hooked_CreateMove, gOffsets.iCreateMoveOffset); //ClientMode create move is called inside of CHLClient::CreateMove, and thus no need for hooking WriteUserCmdDelta.
	clientModeHook->Rehook();

	HWND thisWindow;
	while ((thisWindow = FindWindow("Valve001", NULL)) == false);

	gMenu.windowProc;
	if (thisWindow)
		gMenu.windowProc = (WNDPROC)SetWindowLongPtr(thisWindow, GWLP_WNDPROC, (LONG_PTR)&Hooked_WndProc);

	return 0; //The thread has been completed, and we do not need to call anything once we're done. The call to Hooked_PaintTraverse is now our main thread.
}

BOOL APIENTRY DllMain(HMODULE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	//If you manually map, make sure you setup this function properly.
	if(dwReason == DLL_PROCESS_ATTACH)
	{
		Log::Init(hInstance);
		CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)dwMainThread, NULL, 0, NULL ); //CreateThread > _BeginThread. (For what we're doing, of course.)
	}
	return true;
}