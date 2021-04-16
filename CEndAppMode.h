//////////////////////////////////////////////////////////////////////////
// Micro Game Engine - CEndAppMode class
//
// Tbis class implements 
//////////////////////////////////////////////////////////////////////////
#if !defined(_CEndAppMode_)
#define _CEndAppMode_

//include some objects deteclarations
#include "CMGEApplicationMode.h"

#include "CMGEIntro.h"

class CEndAppMode : public CMGEApplicationMode 
{
	//this inherits from?
	typedef CMGEApplicationMode inherited;

	public:

		//base object style of constructor & destructor
		CEndAppMode()			{ m_bOk=false;};
		virtual ~CEndAppMode()	{End();};
		
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

    protected:

		CMGEIntro m_oEnd;

		DWORD m_dwLastTime;
		DWORD GetAbsTime();
		DWORD GetEplapsedTime();


		int m_nLoops;
};

#endif // !defined(_CEndAppMode_)
