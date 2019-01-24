#ifndef _H_DLGCOMMON
#define _H_DLGCOMMON

bool OpenDlg(HWND hwnd, char* szFilter, int iFilterIndex, char* szFile, int iFileSize, char* szFilename, int iFilenameSize, int& nFileExtension)
{
	strcpy(szFile, "");
	strcpy(szFilename, "");

	OPENFILENAME open;
	memset(&open, 0, sizeof(OPENFILENAME));

	open.lStructSize = sizeof(OPENFILENAME);
	open.hwndOwner = hwnd;
	open.hInstance = GetModuleHandle(NULL);

	open.lpstrFilter = szFilter;
	open.nFilterIndex = iFilterIndex;

	open.lpstrFile = szFile;
	open.nMaxFile  = iFileSize;

	open.lpstrFileTitle = szFilename;
	open.nMaxFileTitle  = iFilenameSize;

	open.lpstrInitialDir = NULL; // Uses initial directory if any 
								 // files match, else my documents

	open.lpstrTitle = NULL; // Use default title, "Save As" or "Open"

	open.Flags = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&open) == TRUE)
	{
		iFilterIndex = open.nFilterIndex;
		nFileExtension = open.nFileExtension;
		return true;
	}
	else
		return false;
}

bool SaveAsDlg(HWND hwnd, char* szFilter, int& iFilterIndex, char* szDefaultExt, char* szFile, int iFileSize, char* szFilename, int iFilenameSize, int& nFileExtension)
{
	OPENFILENAME save;
	memset(&save, 0, sizeof(OPENFILENAME));

	save.lStructSize = sizeof(OPENFILENAME);
	save.hwndOwner = hwnd;
	save.hInstance = GetModuleHandle(NULL);

	save.lpstrFilter = szFilter;
	save.nFilterIndex = iFilterIndex;

	save.lpstrDefExt = szDefaultExt;

	save.lpstrFile = szFile;
	save.nMaxFile  = iFileSize;

	save.lpstrFileTitle = szFilename;
	save.nMaxFileTitle  = iFilenameSize;

	save.lpstrInitialDir = NULL; // Uses initial directory if any 
								 // files match, else my documents

	save.lpstrTitle = NULL; // Use default title, "Save As" or "Open"

	save.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

	if (GetSaveFileName(&save) == TRUE)
	{
		iFilterIndex   = save.nFilterIndex;
		nFileExtension = save.nFileExtension;
		return true;
	}
	else
		return false;
}

/*
class CDlgOpen
{
	void SaveAs(char* szBuffer)
	{
		char szFile[512] = "";
		char szFilename[256] = "";

		OPENFILENAME save;
		memset(&save, 0, sizeof(OPENFILENAME));

		save.lStructSize = sizeof(OPENFILENAME);
		save.hwndOwner = _hwnd;
		save.hInstance = GetModuleHandle(NULL);

		save.lpstrFilter = "Othello Games (*.oth)\0*.oth\0";
		save.nFilterIndex = 1;

		save.lpstrFile = szFile;
		save.nMaxFile  = 512;

		save.lpstrFileTitle = szFilename;
		save.nMaxFileTitle  = 256;

		save.lpstrInitialDir = NULL; // Uses initial directory if any 
									 // files match, else my documents

		save.lpstrTitle = NULL; // Use default title, "Save As" or "Open"

		save.Flags = OFN_HIDEREADONLY;

		_world->Pause();
		if (GetSaveFileName(&save) == TRUE)
		{
			_world->Save(save.lpstrFile);
		}
	}
};
*/

#endif