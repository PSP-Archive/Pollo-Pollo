//////////////////////////////////////////////////////////////////////////
// Micro Game Engine - CPlayerArea class
//
// Tbis class implements 
//////////////////////////////////////////////////////////////////////////
#if !defined(_CPlayerArea_)
#define _CPlayerArea_

//include some objects deteclarations
#include "CMGEObject.h"
#include "CMGESpriteBank.h"

#include "CBlock.h"
#include "CGameGraphic.h"


class CPlayer;

class CPlayerArea : public CMGEObject 
{
	//this inherits from?
	typedef CMGEObject inherited;

	public:

		//base object style of constructor & destructor
		CPlayerArea()			{ m_bOk=false;};
		virtual ~CPlayerArea()	{End();};
		
		//init & end protocol functions
		virtual bool Init(CPlayer* pPlayer);
		virtual void End();
		
		//get info into log
		void Log();

		int Logic();
		void Draw();
		
		inline bool IsEmpty(int width, int height)
		{
			return m_oBlocks[width][height].IsEmpty();			
		}

		inline BYTE GetType(int width, int height)
		{
			return m_oBlocks[width][height].GetType();			
		}

		inline const WORD GetTotalBlocks()
		{
			int total=0;
			for(BYTE nWidth=0;nWidth<m_nNumBlocksWidth;nWidth++)
			{
				for(BYTE nHeight=0;nHeight<cnNumBlocksHeight;nHeight++)
				{
					if((!IsEmpty(nWidth,nHeight)))
						total++;
				}
			}
			total+=m_nTotalFalling;
			return total;
		}

		enum
		{
			
			cnNumBlocksWidth	= 6,
			cnPolloManiaNumBlocksWidth = 15,
			cnMaxNumBlocksWidth = cnPolloManiaNumBlocksWidth,
			cnNumBlocksHeight	= 10,

			cnMaxFallinBlocks   = 255,
			cxFallinBlocksSpeed = 4,

			cnBlockWidth		= 24,
			cnBlockHeight		= 24,

			cBlockStartX		= 16,
			cBlockStartY		= 16,

			cDistanceFromPlayer	= 304,

			cNexPiecesStar		= 172,
			cDistanceNextPieces = 81,

			cNextPieceSeparationX = (cnBlockWidth*2)/3,
			cNextPieceSeparationY = (cnBlockHeight*3)/2,

			cnMaxCustomGraphics = 255,

			cnLoopUntilDrop = 60,

			cnStartVX	= 4,
			cnStartVY	= 8,	 

			cnCharacterX = 199,			
			cnCharacterY = 223,

			cnCharacterWidth = 64,

			cnCenter = cBlockStartX+((cnNumBlocksWidth*cnBlockWidth)/2),

			cnClearAll = 255,
			cnClearLast = 127
		};

		int SetBlock(int width, int height,CBlock* pBlock);
		void AddFallinBlock(int width, int height,CBlock* pBlock);
		inline int GetFalling(){return m_nTotalFalling;};

		int CheckElminations(int width, int height);
		int CheckSameType(int width, int height,BYTE type);
		int MarkSameType(int width, int height,BYTE type);

		inline int GetTotalBlink(){return m_nTotalBlink;};

		int DisapiarBlocks();

		void DisapiarLogic();

		inline int GetTotalDisapiar(){return m_nTotalDisapiar;};

		void AddGraphic(CGameGraphic::TGraphicsType gtType, int x, int y,
			int vx=999, int vy=999, int dx=999, int dy=999,
			DWORD arg0=0,DWORD arg1=0,DWORD arg2=0,DWORD arg3=0);

		void GenerateRandom();

		inline int GetNumBlocksWidth(){return m_nNumBlocksWidth;}
		void Clear(BYTE nBlockType);
	protected:
		int m_nNumBlocksWidth;
		
		int m_nLoops;

		int m_nTotalBlink;
		int m_nTotalFalling;
		int m_nTotalDisapiar;

		CPlayer* m_pPlayer;

		bool m_oChecked[cnMaxNumBlocksWidth][cnNumBlocksHeight];
		CBlock m_oBlocks[cnMaxNumBlocksWidth][cnNumBlocksHeight];

		CBlock m_oBlocksFalling[cnMaxFallinBlocks];

		CGameGraphic m_oGraphics[cnMaxCustomGraphics];		
		void DrawGraphics(int x,int y);
		void GraphicsLogic();

		int m_nShakeX;
		int m_nShakeY;
		int m_nLoopsShaking;
};

#endif // !defined(_CPlayerArea_)

