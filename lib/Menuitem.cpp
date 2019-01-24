#ifndef _H_MENUITEM
#define _H_MENUITEM

class CMenuItem
{
public:
	CMenuItem(UINT id)
		: _id(id)
	{};

	void SetMenu(HMENU hMenu)
	{
		_hMenu = hMenu;
	}

	void SetCheck(bool bCheck)
	{
		CheckMenuItem(_hMenu, _id, bCheck ? MF_CHECKED : MF_UNCHECKED);
	}

private:
	HMENU _hMenu;
	const UINT  _id;
};

#endif