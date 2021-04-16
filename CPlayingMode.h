//////////////////////////////////////////////////////////////////////////
// Micro Game Engine - CPlayingMode class
//
// Tbis class implements 
//////////////////////////////////////////////////////////////////////////
#if !defined(_CPlayingMode_)
#define _CPlayingMode_

//include some objects deteclarations
#include "CMGEApplicationMode.h"
#include "CMGEMusicSample.h"
#include "CMGESoundBank.h"
#include "CMGESpriteBank.h"
#include "CMGEFont.h"

#include "CPlayer.h"
#include "CVersusManager.h"
#include "CCharacter.h"

class CPOLLOPOLLOGame;

class CPlayingMode : public CMGEApplicationMode 
{
	//this inherits from?
	typedef CMGEApplicationMode inherited;

	public:

		//base object style of constructor & destructor
		CPlayingMode()			{ m_bOk=false;};
		virtual ~CPlayingMode()	{End();};
		
		//init & end protocol functions
		virtual bool Init(CMGEApp* pApp,int AppMode);
		virtual void End();
		
		//get info into log
		void Log();

		//load app mode
		virtual bool Load();

		//unload app mode
		virtual void Unload();

		//do app mode logic
		virtual void Logic();

		//draw app mode
		virtual void Draw();

		void SetLooser(BYTE nPlayer);
		void SendBlocksFromPlayer(BYTE nPlayer,BYTE nBlocks);

		enum TPlayingState 
		{
			psPreReady,
			psPlaying,
			psPause,
			psExiting,
			psEndGame,
			psContinue,
			psVersusTalk
		};

		void SetState(TPlayingState psState);
		inline bool GetSound(){return m_bSound;};
		inline void BingoSound(){if(GetSound()) m_Sounds.Play(m_idBingo);}
		inline void HitgroundSound(){if(GetSound()) m_Sounds.Play(m_idHitground);}
		inline void TraslateSound(){if(GetSound()) m_Sounds.Play(m_idTranslate);}		
		inline void TurnSound(){if(GetSound()) m_Sounds.Play(m_idTurn);}
		inline void GoSound(){if(GetSound()) m_Sounds.Play(m_idGo);}
		inline void ReadySound(){if(GetSound()) m_Sounds.Play(m_idReady);}
		inline void KonamiSound(){if(GetSound()) m_Sounds.Play(m_idKonami);}
		inline void VsSound(){if(GetSound()) m_Sounds.Play(m_idVs);}
		
		inline CMGESpriteBank* GetSprites(){return &m_sbSpriteBlocks;}

		void DrawTextDialog(int x, int y, const char *sText, int nSide);
		void DrawDialog(int x, int y, int width, int height, int nSide);
		inline CPlayer* GetPlayer(int idPlayer){return &m_oPlayers[idPlayer];};

	protected:
		bool m_bPolloManiaGame;
		bool m_bSound;

		TPlayingState m_psState;
	
		//Blocks sprite bank
		CMGESpriteBank m_sbSpriteBlocks;

		int m_idDemo;
		int m_idBack;
		int m_idExit;

		//playing mode font
		CMGEFont m_oNumberFont;
		CMGEFont* m_opTextFont;

		enum
		{
			cnMaxPlayers = 2,
			vsLeft = 0,
			vsRight = 1,

			cnMaxCountNumber = 19+1,

			cnCountRate = 60,

		};

		CPlayer m_oPlayers[cnMaxPlayers];
		
		CMGEMusicSample m_oMusic;
		int m_nIdMus;

		CPOLLOPOLLOGame* m_pParent;

		int m_nLoops;
		
		//sounds
		CMGESoundBank m_Sounds;

		int m_idReady;
		int m_idGo;
		int m_idBingo;
		int m_idTranslate;
		int m_idHitground;
		int m_idTurn;
		int m_idKonami;
		int m_idVs;

		//versus manager
		CVersusManager m_oVersus;

		int m_nOptionSelected;
		int m_nLoopNoImput;

		WORD idCornerRD;
		WORD idCornerRU;
		WORD idCornerLD;
		WORD idCornerLU;

		WORD idSideU;
		WORD idSideD;
		WORD idSideL;
		WORD idSideR;

		WORD idFill;

		WORD idPointR;
		WORD idPointL;

		int m_nSideWidth;
		int m_nSideHeight;
		int m_nPointHeight;

		WORD idGameOver;
		WORD idPressStart;
		WORD idGameOverBack;

		WORD idCount[cnMaxCountNumber];
		int nCount;
		int m_nNextCountAt;		

		static DWORD m_nSavePoints[cnMaxPlayers];
		void SetAppMode(int num,int next=-1);
};

#endif // !defined(_CPlayingMode_)
