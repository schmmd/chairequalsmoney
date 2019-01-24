#ifndef _H_MENUITEM
#define _H_MENUITEM

class CCheckMenuItem
{
public:
	CCheckMenuItem() {}
	CCheckMenuItem(UINT id)
		: _id(id)
	{};

	void Set(HMENU hMenu, UINT id)
	{
		_hMenu = hMenu;
		_id = id;
	}

	void Check(bool bCheck)
	{
		CheckMenuItem(_hMenu, _id, bCheck ? MF_CHECKED : MF_UNCHECKED);
	}

	bool GetCheck()
	{
		return GetMenuState(_hMenu, _id, MF_BYCOMMAND) == MF_CHECKED;
	}

	UINT GetID() { return _id; }

private:
	HMENU _hMenu;
	UINT  _id;
};

#endif