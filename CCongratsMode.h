//////////////////////////////////////////////////////////////////////////
// Micro Game Engine - CCongratsMode class
//
// This class implements CCongratsMode
//////////////////////////////////////////////////////////////////////////
#if !defined(_CCongratsMode_)
#define _CCongratsMode_

//include some objects deteclarations
#include "CMGEApplicationMode.h"
#include "CMGETgaFile.h"
#include "CMGESpriteBank.h"
#include "CMGESoundBank.h"
#include "CMGEMusicSample.h"

class CCongratsMode : public CMGEApplicationMode 
{
	//this inherits from?
	typedef CMGEApplicationMode inherited;

	public:

		//base object style of constructor & destructor
		CCongratsMode()			{ m_bOk=false;};
		virtual ~CCongratsMode()	{End();};
		
		//init & end protocol functions
		virtual bool Init(CMGEApp* pApp,int AppMode);
		virtual void End();
		
		//get info into log
		void Log();

		//load app mode
		bool Load();

		//unload app mode
		void Unload();

		//do app mode logic
		void Logic();

		//draw app mode
		void Draw();

    protected:

		enum eRocketState
		{
			rsFree,
			rsGoingUP,
			rsFlaking
		};

		enum 
		{
			eMaxRockets = 3
		};

		struct TRocket
		{
			int x,y;
			int sprite;
			int alive;
			int next;
			int lived;
			eRocketState state;
			int color;
		};

		struct TColor
		{
			char r;
			char g;
			char b;
		};

		static TColor tcColors[];
		int m_nActualCity;
		BYTE m_nActualCityAlfa;

		int m_nMaxFlakepSprite;

		TRocket m_stRockets[eMaxRockets];

		void InitRockets();
		void EndRockets();
		void DrawRockets();
		void RocketsLogic();

		//background
		CMGETgaFile m_oImage;
		
		//city
		CMGESpriteBank m_oCity;

		//firework
		CMGESpriteBank m_oFireWork;

		//rocket
		CMGESpriteBank m_oRocket;

		//misc
		CMGESpriteBank m_oMisc;
		
		//misc sprites ids
		int m_nIdCongrats;

		int m_nLoops;

		int m_nFlashing;

		//pump sound
		CMGESoundBank m_oPump;
		CMGEMusicSample m_oMusic;
};

#endif // !defined(_CCongratsMode_)
