//////////////////////////////////////////////////////////////////////////
// Micro Game Engine - CGameGraphic class (All prupose game graphic)
//
// Tbis class implements 
//////////////////////////////////////////////////////////////////////////
#if !defined(_CGameGraphic_)
#define _CGameGraphic_

//include some objects deteclarations
#include "CMGEObject.h"

#include "CMGESpriteBank.h"

class CGameGraphic : public CMGEObject 
{
	//this inherits from?
	typedef CMGEObject inherited;

	public:

		//base object style of constructor & destructor
		CGameGraphic()			{ m_bOk=false;};
		virtual ~CGameGraphic()	{End();};
		
		//init & end protocol functions
		virtual bool Init();
		virtual void End();
		
		//get info into log
		void Log();

		static void InitSprites(CMGESpriteBank* pBank);

		enum
		{
			cnMaxArg=32
		};

		enum TGraphicsType
		{
			gtEmpty=0,

			gtStar,
			gtLigth,

			gtReadyLogo,
			gtGoLogo,

			gtMax
		};
		
		inline bool IsAlive(){return (m_gtType!=gtEmpty);};

		void Draw(int x, int y);
		void Step();
		void Logic();

		void Set(TGraphicsType gtType, int x=0, int y=0);

		inline void SetVxVy(int vx,int vy)
		{
			m_vx=vx;
			m_vy=vy;
		}

		inline void SetDxDy(int dx,int dy)
		{
			m_dx=dx;
			m_dy=dy;
		}
		inline void SetArg(BYTE nArg, DWORD nValue)
		{
			m_nArgs[nArg] = nValue;
		}

    protected:

		int m_ActualSprite;
		int m_x;
		int m_y;
		int m_gtType;
		int m_dx;
		int m_dy;
		
		signed int m_vx;
		signed int m_vy;

		int m_alfa;
		int m_nLoop;

		enum
		{
			cnMaxFrames = 100,

			cnStarMaxFrames = 50
		};

		static int m_nIdSprite[gtMax][cnMaxFrames];

		static CMGESpriteBank* m_pBank;

		DWORD m_nArgs[cnMaxArg];

};

#endif // !defined(_CGameGraphic_)
