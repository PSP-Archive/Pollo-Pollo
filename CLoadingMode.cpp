
//include class definition
#include "CLoadingMode.h"

//include some objects declarations
#include "CMGELog.h"

//init CLoadingMode
bool CLoadingMode::Init(CMGEApp* pApp,int AppMode)
{	
	//finalize
	End();

	//log output
	CMGELog::SysLog(CMGELog::lInfo,"Init CLoadingMode");
	CMGELog::SysIdent();

	//base initialize
	inherited::Init(pApp,AppMode);

	//are we ok?
	if(IsOk())
	{
		m_bOk = true;
	}

	//log output
	Log();

	CMGELog::SysUnident();	
	

	//are we ok?
	if (IsOk())
		CMGELog::SysLog(CMGELog::lInfo,"Init CLoadingMode [Ok]");
	else
		CMGELog::SysLog(CMGELog::lInfo,"Init CLoadingMode [Fail]");	

	//return initialization status
	return IsOk();
}

//finalize object
void CLoadingMode::End()
{
	//are we ok?
	if (IsOk())
	{
		//call base finalize
		inherited::End();
	}
}

//log some info
void CLoadingMode::Log()
{	
	//log output
	CMGELog::SysLog(CMGELog::lInfo,"Logging CLoadingMode class");
	CMGELog::SysIdent();

	CMGELog::SysUnident();
}

//load app mode
bool CLoadingMode::Load()
{
	bool bResult = true;
	
	char sBuff[255];
	static int sw = 0;
	if(!sw)
		sw = (rand()%2)+1;

	sprintf(sBuff,"dat/images/loading%d.tga",sw);
	sw = (sw==1)?2:1;

	bResult = m_oImage.Init(sBuff);

	m_nLoops=0;

	return bResult;
}

//unload app mode
void CLoadingMode::Unload()
{
	m_oImage.End();
}

//do app mode logic
void CLoadingMode::Logic()
{
	m_nLoops++;

	if(m_nLoops==4)
		NextMode();
}

//draw app mode
void CLoadingMode::Draw()
{
	m_oImage.ScreenCopy();
}

