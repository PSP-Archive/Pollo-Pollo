//////////////////////////////////////////////////////////////////////////
// Micro Game Engine - CBlock class
//
// Tbis class implements 
//////////////////////////////////////////////////////////////////////////
#if !defined(_CBlock_)
#define _CBlock_

//include some objects deteclarations
#include "CMGEObject.h"
#include "CMGESpriteBank.h"

class CBlock : public CMGEObject 
{
	//this inherits from?
	typedef CMGEObject inherited;

	public:

		//base object style of constructor & destructor
		CBlock()			{ m_bOk=false;};
		virtual ~CBlock()	{End();};
		
		//init & end protocol functions
		virtual bool Init(int nX, int nY);
		virtual void End();
		
		//get info into log
		void Log();

		void Logic();
		void Draw(int xoffset,int yoffset);
		
		static void InitSprites(CMGESpriteBank* pBank);
		
		inline BYTE GetType(){return m_nType;};
		void SetType(BYTE nType);

		inline int GetX(){return m_nX;};
		inline int GetY(){return m_nY;};
		inline void SetX(int nX){m_nX=nX;};
		inline void SetY(int nY){m_nY=nY;};
		inline void Move(int x, int y)
		{
			m_nX+=x;
			m_nY+=y;
		}

		inline bool IsEmpty(){return m_bsState==bsEmpty;};

		enum TBlockStates
		{
			bsEmpty,
			bsNormal,
			bsCenter,
			bsFalling,
			bsSteped,
			bsBlink,
			bsDisapiar,
			bsNoEyes
		};

		void SetState(TBlockStates bsState);
		inline TBlockStates GetState(){ return m_bsState;};

		signed int GetStateArg(int index){return m_nStateArgs[index];};
		enum
		{
			cnBlockEmptyType = 255,

			cnMaxBlocks = 8,

			cnMaxArgs = 6,

			ARG_EYE = 0,
			ARG_LOOP_CHANGE = 1,
			ARG_ALFA_PASS = 2,
			ARG_ALFA = 3,
			ARG_BLINK
		};
    protected:

		int m_nX;
		int m_nY;


		TBlockStates m_bsState;
		BYTE m_nType;
		



		BYTE m_nLoops;		
		signed int m_nStateArgs[cnMaxArgs];

			
		static WORD m_idBasicType[cnMaxBlocks];
		static WORD m_idEyes;
		static WORD m_idEyes2;
		static WORD m_idEyesLeft;
		static WORD m_idEyesRigth;
		static WORD m_idEyesClosed;	
		static WORD m_idMark;	
		static WORD m_idCross;	
		

		static CMGESpriteBank* m_pSprites;
		
};

#endif // !defined(_CBlock_)
