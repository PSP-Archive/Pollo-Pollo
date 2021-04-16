//////////////////////////////////////////////////////////////////////////
// Micro Game Engine - CMenuMode class
//
// Tbis class implements 
//////////////////////////////////////////////////////////////////////////
#if !defined(_CMenuMode_)
#define _CMenuMode_

//include some objects deteclarations
#include "CMGEApplicationMode.h"

#include "CMGETgaFile.h"
#include "CMGESpriteBank.h"
#include "CMGESoundBank.h"
#include "CMGEMusicSample.h"

class CPOLLOPOLLOGame;

class CMenuMode : public CMGEApplicationMode 
{
	//this inherits from?
	typedef CMGEApplicationMode inherited;

	public:

		//base object style of constructor & destructor
		CMenuMode()			{ m_bOk=false;};
		virtual ~CMenuMode()	{End();};
		
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

		int m_nAlfa;
		void DrawScrollingBG();
		void DrawMovingTitle();

		inline void ChangeMenu(int menu,int maxoptions,int selected)
		{
			m_nSelected=selected;		
			m_nSubMenu=menu;
			m_nMaxOption=maxoptions;
			m_nLoopsNoChoice=0;
		}
    protected:

		enum TMenuState
		{
			msDisplayMenu,
			msChangeMenu,
			msMovingLogo,
			msFlash,
			msDiplayScore,
			msChangingScore
		};

		enum
		{			
			cCycleScore,
			cCycleDemo,
			cCycleIntro,

			cLastCycle			
		};

		static int m_nCycles[];
		int m_nActualCycle;
		


		TMenuState m_nState;
		int m_nChangeTo;
		int m_nChangeNext;
		int m_nLoopsToChange;

		inline void ChangeState(TMenuState state, int to=-1, int next=-1)
		{
			m_nState = state;
			m_nChangeTo = to;
			m_nChangeNext = next;

			m_nLoopsToChange = 60;
		}

		CPOLLOPOLLOGame* m_pParent;

		//Background image
		CMGETgaFile m_BackTga;
		
		//Sprite bank
		CMGESpriteBank m_Sprites;

		enum
		{
			cNumCharRows = 2,
			cNumCharCols = 4,

			cTotalChars = cNumCharRows*cNumCharCols,

			cCharStartY = 140,

			cCharSpanY = 190,
			cCharSpanX = 70,

			cCharWidth = 32,
			cCharHeight = 32,

			cCharSeparation = 6,

			cTotalStoryChars = 3
		};

		bool m_bPolloManiaLocked;

		bool m_bLockedChars[cTotalChars];
		WORD m_idFaces[cTotalChars];
		WORD m_idFacesSm[cTotalChars];
		WORD m_idFacesSmOff[cTotalChars];

		WORD m_nStoryChar[cTotalStoryChars];

		WORD m_id1Player;
		WORD m_id2Players;
		WORD m_idOptions;
		WORD m_idExit;
		WORD m_idBack;
		WORD m_idStory;
		WORD M_idPolloMania;
		WORD m_idPlayerVsCPU;
		WORD m_id1PlayerGame;
		WORD m_id2PlayersGame;
		WORD m_idPolloManiaGame;
		WORD m_idStart;
		WORD m_idChoose;
		WORD m_idMark;	
		WORD m_idPlayer1text;
		WORD m_idPlayer2text;
		WORD m_idCputext;
		WORD m_idLocked;
		WORD m_idLockedSmall;
		WORD m_idOptionsText;		
		WORD m_idEasy;
		WORD m_idMedium;
		WORD m_idHard;
		WORD m_idBigLogo;
		WORD m_idStoryMode;		
		WORD m_idChoose2;
		WORD m_idStorymoderecords;
		WORD m_id1plvscpurecords;
		WORD m_idpollomaniarecords;
		WORD m_idrecords;

		enum
		{
			LOGO_PART_P1=0,
			LOGO_PART_L,
			LOGO_PART_O,
			LOGO_PART_P2,
			LOGO_PART_MAX,

			MAX_LOGO_PIECES=10
		};

		WORD m_idLogParts[LOGO_PART_MAX];

		typedef struct TagLogoPiece
		{
			int id;

			int dx;
			int dy;

			int x;
			int y;

			int ay;

			int shakex;
			int shakey;

		}TLogoPiece;

		TLogoPiece m_LogoPieces[MAX_LOGO_PIECES];
		void AddLogoPiece(int id, int dx, int dy);
		int m_nLastLogoPiece;
		int m_nCounterPieces;

		WORD m_idSound[2];
		WORD m_idMusic[2];

		int m_nLoops;
		int m_nLoopNoImput;
		int m_nLoopsNoChoice;

		int m_nSelected;		
		int m_nSubMenu;
		int m_nMaxOption;

		void MainMenuDraw();
		void MainMenuClick();

		void SubMenu1PlayerDraw();
		void SubMenu1PlayerClick();

		void SubmenuChoosePlayersDraw();

		int m_nCenterPos;

		//sounds
		CMGESoundBank m_Sounds;

		int m_idSelmenu;
		int m_idTranslate;
		int m_idLogo;

		void NormalMenuLogic();

		void ChoosePlayersLogic();
		void ChooseStoryPlayerLogic();

		int m_nSelectPlayer[2];		
		int m_nSelecting;	

		CMGEMusicSample m_oMusic;
		int m_nIdMusic;

		void SubMenuOptionsDraw();
		void SubMenuOptionsClick();
		void SubMenuHistoryDraw();

		int m_nGameLevel;

		int m_nRenderWidth;
		int m_nRenderHeight;

		int m_nImageWidth;
		int m_nImageHeight;

		int m_nWidthParts;
		int m_nHeightParts;

		int m_nScrollX;
		int m_nScrollY;

		int m_nSoundOn;
		int m_nMusicOn;

		void PlayIntro();
		void GoingToDemo();		

		typedef struct ScoreType
		{
			DWORD nPoints;
			BYTE nChar;
			char sText[32];
			BYTE nLevel;
		}Score;

		enum
		{
			cChangeScoreRate = 60*10,
			cMaxScores = 6,

			cScoreTextStartX = 100,
			cScoreTextStartY = 70,

			cIconColumWidth = 34,
			cNumWidth = 20,
			cLevelWidth = 60,
			cPointWidth = 95,

			cItemHeight = 36
		};

		Score* pCurrentScore;
		Score* pNextScore;

		Score m_scScoreSingle[cMaxScores];
		Score m_scScoreStory[cMaxScores];
		Score m_scScorePollomania[cMaxScores];

		void ReadScore(Score* m_scScore,char* sName);
		void SaveScore(Score* m_scScore,char* sName);
		
		void DrawScore(Score* m_scScore);

		void CheckScore(Score* m_scScore,DWORD nPoints, int idChar, BYTE nLevel);
		int m_nChangedScore;
		bool m_nBlinkChar;
		char m_sCursorText[255];
		int m_nCursorPos;
		char m_sSelectedCursor[1];

		static char m_sPosibleChars[];
};

#endif // !defined(_CMenuMode_)
