
//include class definition
#include "CIntroAppMode.h"

//include some objects declarations
#include "CMGELog.h"
#include "CPOLLOPOLLOGame.h"

#if defined(PSP_VER)
	#include <psppower.h>
#endif

//init CIntroAppMode
bool CIntroAppMode::Init(CMGEApp* pApp,int AppMode)
{	
	//finalize
	End();

	//log output
	CMGELog::SysLog(CMGELog::lInfo,"Init CIntroAppMode");
	CMGELog::SysIdent();

	//base initialize
	inherited::Init(pApp,AppMode);

	//are we ok?
	if(IsOk())
	{
        
	}

	//log output
	Log();

	CMGELog::SysUnident();	
	

	//are we ok?
	if (IsOk())
		CMGELog::SysLog(CMGELog::lInfo,"Init CIntroAppMode [Ok]");
	else
		CMGELog::SysLog(CMGELog::lInfo,"Init CIntroAppMode [Fail]");	

	//return initialization status
	return IsOk();
}

//finalize object
void CIntroAppMode::End()
{
	//are we ok?
	if (IsOk())
	{
		//call base finalize
		inherited::End();
	}
}

//log some info
void CIntroAppMode::Log()
{	
	//log output
	CMGELog::SysLog(CMGELog::lInfo,"Logging CIntroAppMode class");
	CMGELog::SysIdent();

	CMGELog::SysUnident();
}

//load app mode
bool CIntroAppMode::Load()
{
	bool bResult = true;

	GetControl()->m_cuControl.buttons[0] = PSP_CTRL_CROSS |PSP_CTRL_CIRCLE | PSP_CTRL_SQUARE | 
		PSP_CTRL_TRIANGLE |PSP_CTRL_START | PSP_CTRL_SELECT | PSP_CTRL_LTRIGGER | PSP_CTRL_RTRIGGER;
	
	bResult = m_oIntro.Init("dat/intro/intro.xml");

	if(bResult)
		m_dwLastTime=GetAbsTime();

	m_nLoops=0;

	return bResult;
}

//unload app mode
void CIntroAppMode::Unload()
{

	m_oIntro.End();

}

//do app mode logic
void CIntroAppMode::Logic()
{
	m_nLoops++;
	if(m_nLoops>2)
	{
		m_oIntro.Logic(GetEplapsedTime());
		if( (GetControl()->m_cuResponse.buttons[0].wasPressed) || (m_oIntro.ReachEnd()) )
		{
			SetAppMode(CPOLLOPOLLOGame::LoadingMode,CPOLLOPOLLOGame::MenuMode);
		}	
		#if defined(PSP_VER)
			//cause PSP to not be off
			scePowerTick(0);
		#endif
	}
}

//draw app mode
void CIntroAppMode::Draw()
{
	if(m_nLoops>2)
	{
		m_oIntro.Draw();	
	}
	else
	{
		GetRenderDevice()->GetBackBuffer()->Clear(0);
	}
}


DWORD CIntroAppMode::GetAbsTime()
{
	DWORD dwTime=0;

	#if defined(PSP_VER)
		timeval now;
		gettimeofday(&now, NULL);

		dwTime= (now.tv_sec) * 1000000 + now.tv_usec;
		dwTime/=1000;
	#else
		dwTime=timeGetTime();
	#endif

	return dwTime;
}

DWORD CIntroAppMode::GetEplapsedTime()
{
	
	DWORD dwTime=GetAbsTime();

	DWORD dwEplapsed = dwTime-m_dwLastTime;

	m_dwLastTime=dwTime;

	return dwEplapsed;
}
