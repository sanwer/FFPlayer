#include "stdafx.h"
#include "Player.h"
#include "FFplay.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		FFplayInitDLL();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		FFplayExitDLL();
		break;
	}
	return TRUE;
}

extern "C" PLAYER_API DuiLib::CControlUI* CreateControl(LPCTSTR pstrClass)
{
	if (_tcsicmp(pstrClass, _T("Player")) == 0)
	{
		return new DuiLib::CPlayerUI();
	}
	return	NULL;
}

void PlayerCallback(const char * filename)
{
}

namespace DuiLib
{
	class CPlayerWnd : public CWindowWnd
	{
	public:
		CPlayerWnd();
		~CPlayerWnd();

		void Init(CPlayerUI* pOwner);
		LRESULT Play(LPCSTR filepath);

		LPCTSTR GetWindowClassName() const;
		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
		void OnFinalMessage(HWND hWnd);

		bool Open(LPCSTR filepath);
		void Play();
		void Pause();
		void Stop();

	protected:
		CPaintManagerUI m_Manager;
		CPlayerUI* m_pOwner;
		bool bOpen;
		bool bPlay;
		bool bPause;
	};

	CPlayerWnd::CPlayerWnd() : m_pOwner(NULL),bOpen(false),bPlay(false),bPause(false)
	{
	}

	CPlayerWnd::~CPlayerWnd()
	{
	}

	void CPlayerWnd::Init(CPlayerUI* pOwner)
	{
		m_pOwner = pOwner;
		RECT rcPos = {0,0,0,0};
		UINT uStyle = WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		Create(m_pOwner->GetManager()->GetPaintWindow(),_T("UIPlayer"),uStyle,0,rcPos);
		FFplayInit();
	}

	LPCTSTR CPlayerWnd::GetWindowClassName() const
	{
		return _T("PlayerWnd");
	}

	LRESULT CPlayerWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if( uMsg == WM_LBUTTONDOWN ) {
			FFplayPause();
			return 0;
		}

		LRESULT lRes = 0;
		if( m_Manager.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	}

	void CPlayerWnd::OnFinalMessage(HWND hWnd)
	{
		FFplayStop();
		FFplayExit();
		m_pOwner->Invalidate();
		m_pOwner->m_pWindow = NULL;

		delete this;
	}

	bool CPlayerWnd::Open(LPCSTR filepath)
	{
		return bOpen = FFplayOpen(filepath);
	}

	void CPlayerWnd::Play()
	{
		if(bOpen){
			::ShowWindow(m_hWnd, SW_SHOW);
			FFplayPlay(m_hWnd,m_pOwner->GetRelativePos());
		}
	}

	void CPlayerWnd::Pause()
	{
		if(bOpen){
			FFplayPause();
		}
	}

	void CPlayerWnd::Stop()
	{
		if(bOpen){
			FFplayStop();
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////
	//

	CPlayerUI::CPlayerUI() : m_pWindow(NULL)
	{
	}

	CPlayerUI::~CPlayerUI()
	{
		if(m_pWindow){
			::DestroyWindow(*m_pWindow);
			delete m_pWindow;
			m_pWindow =	NULL;
		}
	}

	LPCTSTR CPlayerUI::GetClass() const
	{
		return _T("CPlayerUI");
	}

	LPVOID CPlayerUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, _T("Player")) == 0 ) return static_cast<CPlayerUI*>(this);
		return CControlUI::GetInterface(pstrName);
	}

	void CPlayerUI::SetVisible(bool bVisible)
	{
		if( m_bVisible == bVisible ) return;
		CControlUI::SetVisible(bVisible);
		if( IsVisible() && m_pWindow != NULL )
			::ShowWindow(*m_pWindow, IsVisible() ? SW_SHOW : SW_HIDE);
	}

	void CPlayerUI::SetPos(RECT rc, bool bNeedInvalidate)
	{
		CControlUI::SetPos(rc, bNeedInvalidate);
		if( m_pWindow != NULL ) {
			::SetWindowPos(m_pWindow->GetHWND(), NULL, rc.left, rc.top, rc.right - rc.left, 
				rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE);
		}
	}

	void CPlayerUI::DoInit()
	{
		if(m_pWindow == NULL)
			m_pWindow = new CPlayerWnd();
		if(m_pWindow != NULL)
			m_pWindow->Init(this);
	}

	void CPlayerUI::DoEvent(TEventUI& event)
	{
		CControlUI::DoEvent(event);
	}

	bool CPlayerUI::Open(LPCSTR filepath)
	{
		if(m_pWindow){
			return m_pWindow->Open(filepath);
		}

		return false;
	}

	void CPlayerUI::Play()
	{
		if(m_pWindow){
			m_pWindow->Play();
		}
	}

	void CPlayerUI::Pause()
	{
		if(m_pWindow){
			m_pWindow->Pause();
		}
	}

	void CPlayerUI::Stop()
	{
		if(m_pWindow){
			m_pWindow->Stop();
		}
	}
}
