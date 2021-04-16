//////////////////////////////////////////////////////////////////////////
// Micro Game Engine - CLoadingMode class
//
// This class implements CLoadingMode
//////////////////////////////////////////////////////////////////////////
#if !defined(_CLoadingMode_)
#define _CLoadingMode_

//include some objects deteclarations
#include "CMGEApplicationMode.h"
#include "CMGETgaFile.h"

class CLoadingMode : public CMGEApplicationMode 
{
	//this inherits from?
	typedef CMGEApplicationMode inherited;

	public:

		//base object style of constructor & destructor
		CLoadingMode()			{ m_bOk=false;};
		virtual ~CLoadingMode()	{End();};
		
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
		CMGETgaFile m_oImage;
		int m_nLoops;
};

#endif // !defined(_CLoadingMode_)
