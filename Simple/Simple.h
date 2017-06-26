#ifndef _SIMPLE_H_
#define _SIMPLE_H_
#pragma once
#include "..\Player\Player.h"

class CMainDlg : public WindowImplBase
{
public:
	CMainDlg();
	~CMainDlg();

public:
	CDuiString GetSkinFile() {return _T("MainDlg.xml");};
	LPCTSTR GetWindowClassName() const {return _T("CMainDlg");};
	void InitWindow();
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnFinalMessage(HWND hWnd);

protected:
	DUI_DECLARE_MESSAGE_MAP()
	void Notify(TNotifyUI& msg);

private:
	CLabelUI*		labTitle;
	CPlayerUI*		pPlayerUI;
};

#endif