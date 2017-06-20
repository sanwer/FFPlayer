#include "Stdafx.h"
#include "Simple.h"

#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")

int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR lpCmdLine,
	int nCmdShow)
{
	//初始化COM
	HRESULT Hr = ::CoInitialize(NULL);
	if(FAILED(Hr))
		return 0;

	//初始化UI界面
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourceType(UILIB_ZIPRESOURCE);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());
	CPaintManagerUI::SetResourceZip(MAKEINTRESOURCE(IDR_SKIN), _T("ZIPRES"));

#ifdef _DEBUG
	CPaintManagerUI::LoadPlugin(_T("Player_d.dll"));
#else
	CPaintManagerUI::LoadPlugin(_T("Player.dll"));
#endif

	//运行主界面
	CMainDlg *pMainDlg = new CMainDlg();
	if(pMainDlg == NULL) return 0;
	pMainDlg->Create(NULL,_T(""),UI_WNDSTYLE_FRAME,WS_EX_STATICEDGE | WS_EX_APPWINDOW,0,0,800,600);
	pMainDlg->CenterWindow();
	UINT iRet = pMainDlg->ShowModal();

	//反初始化COM
	::CoUninitialize();

	return 0;
}

CMainDlg::CMainDlg()
{
	labTitle = NULL;
	pPlayerUI = NULL;
}

CMainDlg::~CMainDlg()
{
	PostQuitMessage(0);
}

DUI_BEGIN_MESSAGE_MAP(CMainDlg,WindowImplBase)
	DUI_END_MESSAGE_MAP()

void CMainDlg::InitWindow()
{
	CDuiString sTitle = _T("Simple");
	labTitle = static_cast<CLabelUI*>(m_Manager.FindControl(_T("labTitle")));
	pPlayerUI = static_cast<CPlayerUI*>(m_Manager.FindControl(_T("mPlayer")));
	if (labTitle == NULL || pPlayerUI == NULL)
	{
		MessageBox(NULL,_T("Loading resources fail"),sTitle.GetData(),MB_OK|MB_ICONERROR);
		return;
	}
	SetIcon(IDI_MAINICON);
	::SetWindowText(GetHWND(),sTitle.GetData());
	labTitle->SetText(sTitle.GetData());
}

void CMainDlg::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

//CControlUI* CMainDlg::CreateControl(LPCTSTR pstrClass)
//{
//	if (_tcsicmp(pstrClass, _T("Player")) == 0)
//	{
//		return new CPlayerUI();
//	}
//	return NULL;
//}

void CMainDlg::Notify(TNotifyUI& msg)
{
	if (_tcsicmp(msg.sType,DUI_MSGTYPE_WINDOWINIT) == 0)
	{
		if(pPlayerUI != NULL){
			CDuiString sBuffer,sFilename = _T("cuc_ieschool.flv");
			if(sFilename.Find(_T(':'))==-1){
				CDuiString sPath = CPaintManagerUI::GetInstancePath();
				sBuffer = sPath + sFilename;
				sFilename = sBuffer;
			}
			if(PathFileExists(sFilename.GetData())){
				LPSTR lpAscii = NULL;
				int iLength;
				iLength = WideCharToMultiByte(CP_ACP, 0, sFilename.GetData(), -1, 0, 0, 0, 0);
				if(iLength > 1)
				{
					lpAscii = new char[iLength];
					iLength = WideCharToMultiByte(CP_ACP, 0, sFilename.GetData(), -1, lpAscii, iLength, 0, 0);
					pPlayerUI->Open(lpAscii);
					pPlayerUI->Play();
					delete[] lpAscii;
				}
			}
		}
	}

	WindowImplBase::Notify(msg);
}
