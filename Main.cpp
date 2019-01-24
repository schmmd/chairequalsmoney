#include <windows.h>

#include "lib/app.h"
#include "procedure.h"
#include "resource.h"

#include "main.h"

LRESULT CALLBACK CClass::Procedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	char * szClassName = "CWindow";
	char * szCaption   = "Chair Equals Money";

	CClass windowclass (szClassName, hInstance);
	windowclass.SetMenu(IDR_MENU);
	windowclass.SetIcon(IDI_ICON);
	windowclass.Register();

	CProcedure proc;

	CWindow window(szCaption, szClassName, hInstance);
	window.SetStyle(WINDOW_STYLE);
	window.SetDimentions(580, 490, true);
	window.Create(&proc);
	window.AttachAccelerator(IDR_ACCELERATOR);
	window.Show();

	while(window.ProcessMessage())
	{

	}

	proc.SaveState();

	return 0;
}

void AddLocalPath(char* szDest, int iSize, const char* szFilename)
{
	char* szTemp = new char[iSize];

	// Get the executable name
	GetModuleFileName(NULL, szTemp, iSize);

	// End string at the last backslash
	*strrchr(szTemp, '\\') = '\0';

	// Append szFilename
	sprintf (szDest, "%s\\%s", szTemp, szFilename);

	delete szTemp;
}