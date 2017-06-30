#include "stdafx.h"
#include "Player.h"
#include "..\FFPlay\FFPlay.h"

#ifdef LOAD_FFPLAY_DLL
typedef IPlayer* (*LPPlayerInit)();
typedef bool (*LPPlayerExit)(IPlayer* pPlayer);
HMODULE hFFplay = NULL;
LPPlayerInit pPlayerInit = NULL;
LPPlayerExit pPlayerExit = NULL;
#endif

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
#ifdef LOAD_FFPLAY_DLL
#ifdef _DEBUG
		hFFplay = ::LoadLibrary(_T("FFplay_d.dll"));
#else
		hFFplay = ::LoadLibrary(_T("FFplay.dll"));
#endif
		if(hFFplay) {
			pPlayerInit = (LPPlayerInit)::GetProcAddress(hFFplay, "PlayerInit");
			pPlayerExit = (LPPlayerExit)::GetProcAddress(hFFplay, "PlayerExit");
		}
#endif
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
#ifdef LOAD_FFPLAY_DLL
		if(hFFplay) {
			FreeLibrary(hFFplay);
			hFFplay = NULL;
			pPlayerInit = NULL;
			pPlayerExit = NULL;
		}
#endif
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
		void Exit();

	protected:
		CPaintManagerUI m_Manager;
		CPlayerUI* m_pOwner;
		IPlayer* m_pPlayer;
		bool bOpen;
		bool bPlay;
		bool bPause;
	};

	CPlayerWnd::CPlayerWnd()
		: m_pOwner(NULL),m_pPlayer(false),
		bOpen(false),bPlay(false),bPause(false)
	{
	}

	CPlayerWnd::~CPlayerWnd()
	{
		Exit();
	}

	void CPlayerWnd::Exit()
	{
#ifdef LOAD_FFPLAY_DLL
		if(m_pPlayer && pPlayerExit){
			pPlayerExit(m_pPlayer);
			m_pPlayer = NULL;
		}
#else
		if(m_pPlayer){
			PlayerExit(m_pPlayer);
			m_pPlayer = NULL;
		}
#endif
	}

	void CPlayerWnd::Init(CPlayerUI* pOwner)
	{
		m_pOwner = pOwner;
		RECT rcPos = {0,0,0,0};
		UINT uStyle = WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		Create(m_pOwner->GetManager()->GetPaintWindow(),_T("UIPlayer"),uStyle,0,rcPos);
		if(pPlayerInit && pPlayerExit){
			if(m_pPlayer == NULL)
				m_pPlayer = pPlayerInit();
			if(m_pPlayer)
				 m_pPlayer->Init();
		}

	}

	LPCTSTR CPlayerWnd::GetWindowClassName() const
	{
		return _T("PlayerWnd");
	}

	LRESULT CPlayerWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if( uMsg == WM_LBUTTONDOWN ) {
			if(m_pPlayer)
				 m_pPlayer->Pause();
			return 0;
		}

		LRESULT lRes = 0;
		if( m_Manager.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	}

	void CPlayerWnd::OnFinalMessage(HWND hWnd)
	{
		if(m_pPlayer){
			m_pPlayer->Stop();
		}
		m_pOwner->Invalidate();
		m_pOwner->m_pWindow = NULL;

		delete this;
	}

	bool CPlayerWnd::Open(LPCSTR filepath)
	{
		if(m_pPlayer){
			bOpen = m_pPlayer->Open(filepath);
		}
		return bOpen;
	}

	void CPlayerWnd::Play()
	{
		if(bOpen){
			::ShowWindow(m_hWnd, SW_SHOW);
			RECT rcPlay,rcPos = m_pOwner->GetPos();
			rcPlay.top = rcPlay.left = 0;
			rcPlay.right = rcPos.right - rcPos.left;
			rcPlay.bottom = rcPos.bottom - rcPos.top;
			if(m_pPlayer){
				return m_pPlayer->Play(m_hWnd,rcPlay);
			}
		}
	}

	void CPlayerWnd::Pause()
	{
		if(m_pPlayer)
			m_pPlayer->Pause();
	}

	void CPlayerWnd::Stop()
	{
		if(m_pPlayer)
			m_pPlayer->Stop();
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
		if( m_pWindow) {
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
