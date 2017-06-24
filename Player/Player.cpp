#include "stdafx.h"
#include "Player.h"
#include "FFplay.h"

HMODULE gModule = NULL;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		gModule = hModule;
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

extern "C" PLAYER_API DuiLib::CControlUI* CreateControl(LPCTSTR pstrClass)
{
	if (_tcsicmp(pstrClass, _T("Player")) == 0 && gModule!=NULL)
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
		RECT CalPos();

		LPCTSTR GetWindowClassName() const;
		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
		void OnFinalMessage(HWND hWnd);

		bool Open(LPCSTR filepath);
		void Play();
		void Pause();
		void Stop();
		void Exit();

	protected:
		CPaintManagerUI m_Manager;
		CPlayerUI* m_pOwner;
	};

	CPlayerWnd::CPlayerWnd() : m_pOwner(NULL)
	{
	}

	CPlayerWnd::~CPlayerWnd()
	{
	}

	void CPlayerWnd::Init(CPlayerUI* pOwner)
	{
		m_pOwner = pOwner;
		RECT rcPos = CalPos();
		UINT uStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		Create(m_pOwner->GetManager()->GetPaintWindow(),_T("UIPlayer"),uStyle,0,rcPos);
		::ShowWindow(m_hWnd, pOwner->IsVisible() ? SW_SHOW : SW_HIDE);
	}

	RECT CPlayerWnd::CalPos()
	{
		CDuiRect rcPos = m_pOwner->GetPos();
		CControlUI* pParent = m_pOwner;
		RECT rcParent;
		while( pParent = pParent->GetParent() ) {
			if( !pParent->IsVisible() ) {
				rcPos.left = rcPos.top = rcPos.right = rcPos.bottom = 0;
				break;
			}
			rcParent = pParent->GetClientPos();
			if( !::IntersectRect(&rcPos, &rcPos, &rcParent) ) {
				rcPos.left = rcPos.top = rcPos.right = rcPos.bottom = 0;
				break;
			}
		}

		return rcPos;
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
		FFplayExit();
		m_pOwner->Invalidate();
		m_pOwner->m_pWindow = NULL;

		delete this;
	}

	bool CPlayerWnd::Open(LPCSTR filepath)
	{
		if(gModule)
			FFplayInit(gModule,m_hWnd,CalPos());
		else
			return false;
		FFplayOpen(filepath);
		return true;
	}

	void CPlayerWnd::Play()
	{
		FFplayPlay();
	}

	void CPlayerWnd::Pause()
	{
		FFplayPause();
	}

	void CPlayerWnd::Stop()
	{
		FFplayStop();
	}

	void CPlayerWnd::Exit()
	{
		FFplayExit();
		::SendMessage(m_hWnd, WM_CLOSE, 0, 0);
	}


	/////////////////////////////////////////////////////////////////////////////////////
	//

	CPlayerUI::CPlayerUI() : m_pWindow(NULL)
	{
	}

	CPlayerUI::~CPlayerUI()
	{
		if( m_pWindow != NULL ) {
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

	HWND CPlayerUI::GetNativeWindow() const
	{
		if (m_pWindow) return m_pWindow->GetHWND();
		return NULL;
	}

	bool CPlayerUI::Activate()
	{
		if( !IsVisible() ) return false;
		if( !IsEnabled() ) return false;

		if(m_pWindow == NULL){
			m_pWindow = new CPlayerWnd();
			m_pWindow->Init(this);
		}
		return true;
	}

	void CPlayerUI::DoEvent(TEventUI& event)
	{
		CControlUI::DoEvent(event);
	}

	void CPlayerUI::SetPos(RECT rc, bool bNeedInvalidate)
	{
		CControlUI::SetPos(rc, bNeedInvalidate);
		if( m_pWindow != NULL ) {
			::SetWindowPos(m_pWindow->GetHWND(), NULL, rc.left, rc.top, rc.right - rc.left,
				rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOACTIVATE);
		}
	}

	void CPlayerUI::SetVisible(bool bVisible)
	{
		if( m_bVisible == bVisible ) return;
		CControlUI::SetVisible(bVisible);
		if( IsVisible() && m_pWindow != NULL )
			::ShowWindow(*m_pWindow, IsVisible() ? SW_SHOW : SW_HIDE);
	}

	void CPlayerUI::SetInternVisible(bool bVisible)
	{
		if( m_bVisible == bVisible ) return;
		CControlUI::SetInternVisible(bVisible);
		if( IsVisible() && m_pWindow != NULL )
			::ShowWindow(*m_pWindow, IsVisible() ? SW_SHOW : SW_HIDE);
	}

	bool CPlayerUI::Open(LPCSTR filepath)
	{
		Activate();
		if(m_pWindow){
			return m_pWindow->Open(filepath);
		}

		return false;
	}

	void CPlayerUI::Play()
	{
		if(m_pWindow){
			SetVisible(true);
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

	void CPlayerUI::Exit()
	{
		Activate();
		if(m_pWindow){
			m_pWindow->Exit();
		}
	}
}
