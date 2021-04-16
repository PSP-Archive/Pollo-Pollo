//////////////////////////////////////////////////////////////////////////
// Micro Game Engine - CZoomAppMode class
//
// Tbis class implements 
//////////////////////////////////////////////////////////////////////////
#if !defined(_CZoomAppMode_)
#define _CZoomAppMode_

//include some objects deteclarations
#include "CMGEApplicationMode.h"
#include "CMGETgaFile.h"

class CZoomAppMode : public CMGEApplicationMode 
{
	//this inherits from?
	typedef CMGEApplicationMode inherited;

	public:

		//base object style of constructor & destructor
		CZoomAppMode()			{ m_bOk=false;};
		virtual ~CZoomAppMode()	{End();};
		
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

		//mode states
		enum ModeState
		{
			Zoom,
			CrossFade,
			Still,
			Fading
		};

		int m_alfa;

		ModeState m_state;

		//Bacground color
		WORD m_bgColor;

		//Zoomed size		
		RECT m_rDest;

		//actual image size
		int m_w;
		int m_h;

		//zooming step
		int m_step;

		//Background image
		CMGETgaFile m_Tga;
		CMGETgaFile m_TgaPS2Dev;


};

#endif // !defined(_CZoomAppMode_)
