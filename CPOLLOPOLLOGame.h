//////////////////////////////////////////////////////////////////////////
// POLLOPOLLO application class
//
// Tbis its the implementation of the POLLOPOLLO
//////////////////////////////////////////////////////////////////////////

#if !defined(__CPOLLOPOLLOGameGame__)
#define __CPOLLOPOLLOGame__

//includes some classes
#include "CMGEApp.h"
#include "CMGETgaFile.h"
#include "CMGESpriteBank.h"

#include "CMGERawSample.h"
#include "CMGEMusicSample.h"

#include "CMGEFont.h"


//Our App
class CPOLLOPOLLOGame : public CMGEApp
{
	//this inherits from?
	typedef CMGEApp inherited;
	
	public:
		//base object style of constructor & destructor
		CPOLLOPOLLOGame()			{}		
		virtual ~CPOLLOPOLLOGame()	{ End(); }	

		//App AI Logic		
		void Logic();

		//App Draw frame
		void Draw();

		//Load, Unload app data
		bool Load();
		void Unload();

		//app modes
		enum AppMode
		{
			IntroAppMode,
			ZoomAppMode,
			MenuMode,
			PlayingMode,
			LoadingMode,
			EndAppMode,
			CongratsMode
		};

		enum TPlayingMode
		{			
			PlayerVsCPU,
			TwoPlayers,
			CPUvsCPU,
			History,
			Pollomania
		};

		enum
		{
			cMaxPlayers = 2
		};

		typedef struct GameLevelConfigType
		{
			int m_nSpeed;
			int m_nCpuPressingDown;
			int m_nCpuReleasingDown;

		}GameLevelConfig;

		GameLevelConfig GetGameLevelConfig(int nLevel);

		inline void SetGameLevel(int nLevel){m_nLevel=nLevel;};
		inline int GetGameLevel()
		{
			int result=m_nLevel;

			if(m_nHistoryLevel!=-1)
			{
				//easy
				if(m_nLevel==0)
				{
					//first round
					if(m_nHistoryLevel==0)
					{
						//easy
						result=0;
					}
					//second round
					else if (m_nHistoryLevel==1)
					{
						//easy
						result=0;
					}
					//third round
					else if (m_nHistoryLevel==2)
					{
						//medium
						result=1;
					}
				}
				//medium
				else if(m_nLevel==1)
				{
					//first round
					if(m_nHistoryLevel==0)
					{
						//easy
						result=0;
					}
					//second round
					else if(m_nHistoryLevel==1)
					{
						//medium
						result=1;
					}
					//third round
					else if (m_nHistoryLevel==2)
					{
						//medium
						result=1;
					}
				}
				//hard
				else if(m_nLevel==2)
				{
					//first round
					if(m_nHistoryLevel==0)
					{
						//medium
						result=1;
					}
					//second round
					else if(m_nHistoryLevel==1)
					{
						//medium
						result=1;
					}
					//third round
					else if (m_nHistoryLevel==2)
					{
						//hard
						result=2;
					}
				}
			}
			
			return result;
		};

		inline int GetLevel(){return m_nLevel;};

		inline int GetHistoryGameLevel(){return m_nHistoryLevel;};
		inline void SetHistoryGameLevel(int nLevel){m_nHistoryLevel=nLevel;};

		inline void SetPlayingMode(TPlayingMode pmMode){m_pmMode=pmMode;};
		inline TPlayingMode GetPlayingMode(){return m_pmMode;};

		inline void SetPlayersId(int player, int idChar){m_nIdChars[player]=idChar;};
		inline int GetPlayersId(int player){return m_nIdChars[player];};

		inline void SetSound(int nSoundOn){m_bSoundOn=nSoundOn?true:false;};
		inline bool GetSound(){return m_bSoundOn;};

		inline void SetMusic(int nMusicOn){m_bMusicOn=nMusicOn?true:false;};
		inline bool GetMusic(){return m_bMusicOn;};

		inline bool GetEasyWin(){return m_bEasyWin;};
		inline bool GetClearAll(){
			bool result = m_bClearAll;
			m_bClearAll=false;
			return result;
		};
		inline bool GetClearGhost(){
			bool result = m_bClearGhost;
			m_bClearGhost=false;
			return result;
		};
		inline bool GetClearLast(){
			bool result = m_bClearLast;
			m_bClearLast=false;
			return result;
		};

		inline void SetEasyWinCheat(){
			m_bEasyWinCheat=true;
			SetConfigValue("cheats","easywin","true");
		};
		inline void SetClearAllCheat(){
			m_bClearAllCheat=true;
			SetConfigValue("cheats","clear-all","true");
		};
		inline void SetClearGhostCheat(){
			m_bClearGhostCheat=true;
			SetConfigValue("cheats","clear-ghost","true");
		};
		inline void SetClearLastCheat(){
			m_bClearLastCheat=true;
			SetConfigValue("cheats","clear-last","true");
		};
		inline CMGEFont* GetFont(){return &m_oFont;};
		inline void SetPosibleRecord(int nPlayer, DWORD nPoints)
		{
			m_nPossibleRecord[nPlayer] = nPoints;
		}
		inline DWORD GetPosibleRecord(int nPlayer)
		{
			return m_nPossibleRecord[nPlayer];
		}
		inline TPlayingMode GetPosibleRecordMode()
		{
			return m_nPosibleRecordMode;
		}

		inline void SetPosibleRecordMode(TPlayingMode pmMode )
		{
			m_nPosibleRecordMode=pmMode;
		}
		inline void SetPosibleRecordId(int nPlayer, int nId)
		{
			m_nPossibleRecordChar[nPlayer] = nId;
		}
		inline DWORD GetPosibleRecordId(int nPlayer)
		{
			return m_nPossibleRecordChar[nPlayer];
		}
		inline BYTE GetPosibleRecordLevel()
		{
			return m_nPossibleRecordLevel;
		}

		inline void SetPosibleRecordLevel(BYTE nLevel )
		{
			m_nPossibleRecordLevel=nLevel;
		}
	private:

		bool m_bClearAll;
		bool m_bClearAllCheat;

		bool m_bClearGhost;
		bool m_bClearGhostCheat;

		bool m_bClearLast;
		bool m_bClearLastCheat;

		bool m_bEasyWin;
		bool m_bEasyWinCheat;
		
		bool m_bSoundOn;
		bool m_bMusicOn;

		int m_nLevel;
		int m_nHistoryLevel;
		TPlayingMode m_pmMode;
		BYTE m_nIdChars[cMaxPlayers];

		CMGEFont m_oFont;	

		DWORD m_nPossibleRecord[cMaxPlayers];
		TPlayingMode m_nPosibleRecordMode;

		int m_nPossibleRecordChar[cMaxPlayers];
		BYTE m_nPossibleRecordLevel;
};	
#endif

