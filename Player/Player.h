#ifndef _PLAYERUI_H_
#define _PLAYERUI_H_
#pragma once
#pragma warning (disable:4996)

#ifdef PLAYER_EXPORTS
#define PLAYER_API __declspec(dllexport)
#else
#define PLAYER_API __declspec(dllimport)
#endif

namespace DuiLib
{
	class PLAYER_API CPlayerUI : public CControlUI
	{
		friend class CPlayerWnd;
	public:
		CPlayerUI();
		~CPlayerUI();

		virtual LPCTSTR GetClass() const;
		virtual LPVOID GetInterface(LPCTSTR pstrName);
		virtual HWND GetNativeWindow() const;
		virtual bool Activate();

		void SetPos(RECT rc, bool bNeedInvalidate = true);
		void DoEvent(TEventUI& event);
		void SetVisible(bool bVisible);
		void SetInternVisible(bool bVisible);

		bool Open(LPCSTR filepath);
		void Play();
		void Pause();
		void Stop();
		void Exit();

	protected:
		CPlayerWnd* m_pWindow;
	};
}

#endif
