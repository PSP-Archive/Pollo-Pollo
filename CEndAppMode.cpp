
//include class definition
#include "CEndAppMode.h"

//include some objects declarations
#include "CMGELog.h"
#include "CPOLLOPOLLOGame.h"

#if defined(PSP_VER)
	#include <psppower.h>
#endif

//init CEndAppMode
bool CEndAppMode::Init(CMGEApp* pApp,int AppMode)
{	
	//finalize
	End();

	//log output
	CMGELog::SysLog(CMGELog::lInfo,"Init CEndAppMode");
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
		CMGELog::SysLog(CMGELog::lInfo,"Init CEndAppMode [Ok]");
	else
		CMGELog::SysLog(CMGELog::lInfo,"Init CEndAppMode [Fail]");	

	//return initialization status
	return IsOk();
}

//finalize object
void CEndAppMode::End()
{
	//are we ok?
	if (IsOk())
	{
		//call base finalize
		inherited::End();
	}
}

//log some info
void CEndAppMode::Log()
{	
	//log output
	CMGELog::SysLog(CMGELog::lInfo,"Logging CEndAppMode class");
	CMGELog::SysIdent();

	CMGELog::SysUnident();
}

//load app mode
bool CEndAppMode::Load()
{
	bool bResult = true;

	GetControl()->m_cuControl.buttons[0] = PSP_CTRL_CROSS |PSP_CTRL_CIRCLE | PSP_CTRL_SQUARE | 
		PSP_CTRL_TRIANGLE |PSP_CTRL_START | PSP_CTRL_SELECT | PSP_CTRL_LTRIGGER | PSP_CTRL_RTRIGGER;
	
	bResult = m_oEnd.Init("dat/ending/ending.xml");

	if(bResult)
		m_dwLastTime=GetAbsTime();

	m_nLoops=0;

	return true;
}

//unload app mode
void CEndAppMode::Unload()
{
	m_oEnd.End();
}

//do app mode logic
void CEndAppMode::Logic()
{
	m_nLoops++;
	if(m_nLoops>2)
	{
		m_oEnd.Logic(GetEplapsedTime());
		if( (GetControl()->m_cuResponse.buttons[0].wasPressed) || (m_oEnd.ReachEnd()) )
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
void CEndAppMode::Draw()
{
	if(m_nLoops>2)
	{
		m_oEnd.Draw();	
	}
	else
	{
		GetRenderDevice()->GetBackBuffer()->Clear(0);
	}
}

DWORD CEndAppMode::GetAbsTime()
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

DWORD CEndAppMode::GetEplapsedTime()
{
	
	DWORD dwTime=GetAbsTime();

	DWORD dwEplapsed = dwTime-m_dwLastTime;

	m_dwLastTime=dwTime;

	return dwEplapsed;
}
