#include "RDR2ScriptHook\main.h"

#include "AnimalFinder.h"
#include "Keyboard.h"

#include <minwindef.h>
#include <winnt.h>

BOOL APIENTRY DllMain(HMODULE hInstance, DWORD reason, LPVOID lpReserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		scriptRegister(hInstance, ScriptMain);
		keyboardHandlerRegister(Keyboard::OnKeyboardMessage);
		break;
	case DLL_PROCESS_DETACH:
		scriptUnregister(hInstance);
		keyboardHandlerUnregister(Keyboard::OnKeyboardMessage);
		break;
	}		
	return TRUE;
}