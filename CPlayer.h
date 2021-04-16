//////////////////////////////////////////////////////////////////////////
// Micro Game Engine - CPlayer class
//
// Tbis class implements 
//////////////////////////////////////////////////////////////////////////
#if !defined(_CPlayer_)
#define _CPlayer_

//include some objects deteclarations
#include "CMGEObject.h"
#include "CMGEFont.h"

#include "CPlayerArea.h"
#include "CPiece.h"
#include "CMGEControl.h"
#include "CCPULogic.h"

#include "CCharacter.h"
#include "CMGESpriteBank.h"

class CPlayingMode;

class CPlayer : public CMGEObject 
{
	//this inherits from?
	typedef CMGEObject inherited;

	public:
		enum TPlayerMode
		{
			pmSolo,			// play solo (move with d-pad, action cross)
			pmCpu,			// c.p.u. logic (a.i.)
			pmFirst,		// play vs (move width d-pad action l-button)
			pmSecond		// play vs (move with circle,triangle,cross,square, action r-button)
		};

		//base object style of constructor & destructor
		CPlayer()			{ m_bOk=false;};
		virtual ~CPlayer()	{End();};
		
		//init & end protocol functions
		virtual bool Init(CPlayingMode* pParent, int nSide, TPlayerMode pmMode, int idChar, int nGameLevel);
		virtual void End();
		
		//get info into log
		void Log();

		void Draw();
		void Logic();

		inline BYTE GetSide(){return m_nSide;};

		enum TPlayerStates
		{
			bsPlaying,
			bsStoped,
			bsWaitingFalling,
			bsWaitingBlink,
			bsWaitingDisapiar,
			bsWinning,
			bsWinningNoSound,
			bsLoosing,
			bsLoosingNoSound,
			bsPreReady,
			bsReady
		};

		static void InitSprites(CMGESpriteBank* pSprites, CMGEFont* pFont);


		inline TPlayerStates GeState(){return m_psState;};

		inline void SetSate(TPlayerStates psState)
		{
			m_psState=psState;

			switch(m_psState)
			{
				case bsWinning:
					m_oChar.SetMode("winner");
				break;
				case bsLoosing:
					m_oChar.SetMode("looser");
				break;
				case bsWinningNoSound:
					m_oChar.SetMode("winner-nosound");
					m_psState=bsWinning;
				break;
				case bsLoosingNoSound:
					m_oChar.SetMode("looser-nosound");
					m_psState=bsLoosing;
				break;
			}
		};

		void AddBlocks(BYTE nBlocks);

		void BingoSound();
		void HitgroundSound();
		void GroundBlocks(bool bEmotion);
		void SetDemoPlay();

		inline CCharacter* GetChar(){return &m_oChar;};

		inline DWORD GetPoints(){return m_nPoints;};
		inline DWORD SetPoints(int nPoints){return m_nPoints=nPoints;};
		inline CPlayerArea* GetArea(){return &m_oArea;}
		inline int GetSpeed(){return m_nSpeed;}
    protected:
		bool m_bPolloManiaGame;
		int m_nCountToNext;		

		DWORD m_nScrenWidth;
		DWORD m_nPoints;

		TPlayerMode m_pmMode;

		enum
		{
			cWaitNoInput = 5,

			cMaxSpeed = 8,
			cMaxPlayableSpeed = 6,

			cCharCountToPlay = 4
		};

		TPlayerStates m_psState;

		BYTE m_nSide;
		CPlayingMode* m_pParent;

		CPlayerArea m_oArea;

		CPiece m_oPiece;

		bool Collide(bool bOnlyTest=false);
		bool CollideBlock(BYTE block);
		void SetBlock(BYTE block);
		void AddFallinBlock(BYTE block);

		bool CheckControls();
		int m_nWaitNoInput;

		bool CouldMoveToLeft();
		bool CouldMoveToRigth();

		bool CouldRotate();
		static CMGEFont* m_poFont;

		static CMGESpriteBank* m_poSprites;
		static WORD m_idWinner;
		static WORD m_idLooser;

		BYTE m_nSpeed;
		BYTE m_nLocalSpeed;
		int m_nLoops;
		bool m_bBlinkText;

		CMGEControl::ControlResponse m_oPlayerResponse;

		CCPULogic m_oCpuLogic;

		signed int m_DisapiarBlocks;
		signed int m_nAcumulatedBlocks;

		void DropAcumulated();
		void DrawAcumulated(int x, int y);

		signed int m_nLoopUntilDrop;

		//chars
		int m_idChar;

		int m_nNextCharCount;
		bool m_bNextMoveDoChar;
		CCharacter m_oChar;
		void DoCharacterStep();

		int m_nGameLevel;
		int m_nShowNextPieces;
};

#endif // !defined(_CPlayer_)
