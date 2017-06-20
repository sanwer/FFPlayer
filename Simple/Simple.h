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
	virtual CDuiString GetSkinFile() {return _T("MainDlg.xml");};
	virtual LPCTSTR GetWindowClassName() const {return _T("CMainDlg");};
	virtual void InitWindow();
	virtual void OnFinalMessage(HWND hWnd);
	//virtual CControlUI* CreateControl(LPCTSTR pstrClass);

protected:
	DUI_DECLARE_MESSAGE_MAP()
		void Notify(TNotifyUI& msg);

private:
	CLabelUI*		labTitle;
	CPlayerUI*		pPlayerUI;
};

#endif