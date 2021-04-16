//////////////////////////////////////////////////////////////////////////
// Micro Game Engine - CCPULogic class
//
// Tbis class implements 
//////////////////////////////////////////////////////////////////////////
#if !defined(_CCPULogic_)
#define _CCPULogic_

//include some objects deteclarations
#include "CMGEObject.h"

#include "CMGEControl.h"

#include "CPlayerArea.h"
#include "CPiece.h"

class CCPULogic : public CMGEObject 
{
	//this inherits from?
	typedef CMGEObject inherited;

	public:

		//base object style of constructor & destructor
		CCPULogic()			{ m_bOk=false; m_pArea=NULL; m_pPiece=NULL;};
		virtual ~CCPULogic()	{End();};
		
		//init & end protocol functions
		virtual bool Init(CPlayerArea* pArea,CPiece* pPiece,int nGameLevel);
		virtual void End();
		
		//get info into log
		void Log();

		void Calculate();
		void Reset();
		CMGEControl::ControlResponse* GetResponse(){return &m_oResponse;};

		enum
		{
			cnMaxTurns = 3
		};

    protected:

		//saved actual player area
		CPlayerArea* m_pArea;

		//saved player piece
		CPiece* m_pPiece;

		//actual CPU response buttons
		CMGEControl::ControlResponse m_oResponse;

		//wante rotation and width
		CPiece::TPieceCycles m_WantedRotate;
		int m_WantedWidth;

		//logic area, each reset get copy from actual
		BYTE m_SavedLogicArea[CPlayerArea::cnNumBlocksWidth][CPlayerArea::cnNumBlocksHeight];

		//area for the current test, reset each choice
		BYTE m_LogicArea[cnMaxTurns][CPlayerArea::cnNumBlocksWidth][CPlayerArea::cnNumBlocksHeight];

		//area check for functions that count neighbor pieces
		BYTE m_Checked[CPlayerArea::cnNumBlocksWidth][CPlayerArea::cnNumBlocksHeight];

		//area for deleting pieces
		BYTE m_ToDelete[CPlayerArea::cnNumBlocksWidth][CPlayerArea::cnNumBlocksHeight];

		//copy actual player area to saved, for all calculations
		void CopyPlayerArea();

		//copy the saved area to logic calculation
		void SavedPlayerAreaToLogic(int level);


		//test to drop a piece with a rotation in a pos
		int TestDropPiece(int rotation, int width, BYTE btCenter, BYTE btStick, int level);

		//test to drop a block in a pos
		int TesDropBlock(int width, BYTE type, int level);

		//store a block in logic area
		int SetBlock(int width, int height, BYTE type,int level);

		//count neighbors of the same type
		int CheckSameType(int width, int height,BYTE type,int level);

		//mark neighbors to be deleted
		int MarkToDeleteSameType(int width, int height,BYTE type,int level);

		//delete marked to delete blocks, and return points
		int DeleteMarked(int level);

		//need deletion flag
		bool m_bToDelete;

		//logic loops
		int m_nLoops;

		int m_nLoopsPressingDown;
		int m_nLoopsReleasingDown;

		int m_nLoopsTo;
		bool m_bPressingDown;

		int DoStep(int level);

		int m_nGameLevel;

};

#endif // !defined(_CCPULogic_)
