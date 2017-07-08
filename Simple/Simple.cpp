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

	//释放UI界面资源
	CPaintManagerUI::Term();
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

LRESULT CMainDlg::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_SPACE:
			pPlayerUI->Pause();
			return 0;
		default:
			break;
		}
		break;
	default:
		break;
	}
	return WindowImplBase::HandleMessage(uMsg, wParam, lParam);
}

void CMainDlg::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

void CMainDlg::Notify(TNotifyUI& msg)
{
	if (_tcsicmp(msg.sType,DUI_MSGTYPE_WINDOWINIT) == 0)
	{
		if(pPlayerUI != NULL){
			char szFilepath[MAX_PATH] = {0};
			char szFilename[MAX_PATH] = {0};
			char szDrive[MAX_PATH] = {0};
			char szDir[MAX_PATH] = {0};
			sprintf(szFilename,"cuc_ieschool.flv");
			if(strchr(szFilename,':') >= 0){
				::GetModuleFileNameA(CPaintManagerUI::GetInstance(), szFilepath, MAX_PATH);
				_splitpath(szFilepath,szDrive,szDir,NULL,NULL);
				sprintf(szFilepath,"%s%s%s",szDrive,szDir,szFilename);
			}else{
				strcpy(szFilepath,szFilename);
			}
			if(PathFileExistsA(szFilepath)){
				if(pPlayerUI->Open(szFilepath))
					pPlayerUI->Play();
				//Close();
			}
		}
	}

	WindowImplBase::Notify(msg);
}
