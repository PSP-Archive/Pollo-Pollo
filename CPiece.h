//////////////////////////////////////////////////////////////////////////
// Micro Game Engine - CPiece class
//
// Tbis class implements 
//////////////////////////////////////////////////////////////////////////
#if !defined(_CPiece_)
#define _CPiece_

//include some objects deteclarations
#include "CMGEObject.h"

#include "CBlock.h"

class CPiece : public CMGEObject 
{
	//this inherits from?
	typedef CMGEObject inherited;

	public:

		//base object style of constructor & destructor
		CPiece()			{ m_bOk=false; m_nNextPiece=0;};
		virtual ~CPiece()	{End();};
		
		//init & end protocol functions
		virtual bool Init(int nX,int nY,int nShowPieces);
		virtual void End();
		
		//get info into log
		void Log();

		void Logic();
		void Draw(int xoffset, int yoffset);
		void DrawNextPieces(int xoffset, int yoffset);
		
		inline int GetY()
		{
			return m_nY;
		}

		inline int GetX()
		{
			return m_nX;
		}
		inline void Move(int nX, int nY)
		{
			m_nX+=nX;
			m_nY+=nY;
		}

		inline RECT GetRect()
		{
			RECT result;

			switch(m_pcCicle)
			{

				case tcTOP:	
					result.left = GetBlock(cStickBlock)->GetX();
					result.top = GetBlock(cStickBlock)->GetY();
					result.right = GetBlock(cCenterBlock)->GetX();
					result.bottom = GetBlock(cCenterBlock)->GetY();
				break;

				case tcLEFT:
					result.left = GetBlock(cStickBlock)->GetX();
					result.top = GetBlock(cStickBlock)->GetY();
					result.right = GetBlock(cCenterBlock)->GetX();
					result.bottom = GetBlock(cCenterBlock)->GetY();
				break;

				case tcDOWN:
					result.left = GetBlock(cCenterBlock)->GetX();
					result.top = GetBlock(cCenterBlock)->GetY();
					result.right = GetBlock(cStickBlock)->GetX();
					result.bottom = GetBlock(cStickBlock)->GetY();
				break;

				case tcRIGHT:
					result.left = GetBlock(cCenterBlock)->GetX();
					result.top = GetBlock(cCenterBlock)->GetY();
					result.right = GetBlock(cStickBlock)->GetX();
					result.bottom = GetBlock(cStickBlock)->GetY();
				break;
			}

			return result;
		}
		inline CBlock* GetBlock(int num){return &m_nBlocks[num];};
		enum
		{
			cnMaxBlocks = 2,

			cCenterBlock = 0,
			cStickBlock = 1,

			cNextPiecesShow	= 3,
			cMaxNextPieces = 100
		};

		//get next piece for giving one
		inline int GetNextPiece(int number, int amount=1)
		{
			int nextPiece = number+amount;			

			if(nextPiece>(cMaxNextPieces-1))
				nextPiece=(nextPiece-(cMaxNextPieces-1))-1;

			return nextPiece;
		}

		//piece cycles
		enum TPieceCycles
		{
			tcTOP,
			tcLEFT,
			tcDOWN,
			tcRIGHT
		};

		inline TPieceCycles GetCicle(){return m_pcCicle;};

		void Rotate();


		static void InitNextPieces(bool bPolloMania);

		inline BYTE GetNextPieceBlockType(int nextpiece,BYTE block)
		{
			return m_saNextPieces[nextpiece][block];

		}
		inline int GetNextPieceNum(){return m_nNextPiece;};

    protected:

		TPieceCycles m_pcCicle;

		CBlock m_nBlocks[cnMaxBlocks];

		int m_nX;
		int m_nY;

		int m_nNextPiece;
		static BYTE m_saNextPieces[cMaxNextPieces][cnMaxBlocks];

		int m_nShowPieces;
};

#endif // !defined(_CPiece_)
