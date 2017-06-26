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

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		void SetVisible(bool bVisible);
		void SetPos(RECT rc, bool bNeedInvalidate = true);
		void DoInit();
		void DoEvent(TEventUI& event);

		bool Open(LPCSTR filepath);
		void Play();
		void Pause();
		void Stop();
		void VolumeUp();
		void VolumeDown();

	protected:
		CPlayerWnd* m_pWindow;
	};
}

#endif
