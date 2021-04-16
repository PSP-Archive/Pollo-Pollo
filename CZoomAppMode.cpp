
//include class definition
#include "CZoomAppMode.h"

//include some objects declarations
#include "CMGELog.h"
#include "CPOLLOPOLLOGame.h"

//init CZoomAppMode
bool CZoomAppMode::Init(CMGEApp* pApp,int AppMode)
{	
	//finalize
	End();

	//log output
	CMGELog::SysLog(CMGELog::lInfo,"Init CZoomAppMode");
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
		CMGELog::SysLog(CMGELog::lInfo,"Init CZoomAppMode [Ok]");
	else
		CMGELog::SysLog(CMGELog::lInfo,"Init CZoomAppMode [Fail]");	

	//return initialization status
	return IsOk();
}

//finalize object
void CZoomAppMode::End()
{
	//are we ok?
	if (IsOk())
	{
		//call base finalize
		inherited::End();
	}
}

//log some info
void CZoomAppMode::Log()
{	
	//log output
	CMGELog::SysLog(CMGELog::lInfo,"Logging CZoomAppMode class");
	CMGELog::SysIdent();

	CMGELog::SysUnident();
}

//load app mode
bool CZoomAppMode::Load()
{
	bool bResult = true;

	//Load background
	bResult = m_Tga.Init("dat/back.tga");

	if (bResult)
	{		
		bResult = m_TgaPS2Dev.Init("dat/images/ps2dev.tga");
		if (bResult)
		{		
			//init zoom vars
			m_w = 0;
			m_h = 0;

			//calculate center position
			m_rDest.left=(GetWidth()-m_w)>>1;
			m_rDest.right=m_rDest.left+m_w;
			m_rDest.top=(GetHeight()-m_h)>>1;
			m_rDest.bottom=m_rDest.top+m_h;

			//zooming steps
			m_step = 3;

			//Get background color (white) in display format
			m_bgColor=GetRenderDevice()->GetColor(255,255,255);		
		}
	}

	m_alfa = 0;
	m_state = Zoom;

	return bResult;
}

//unload app mode
void CZoomAppMode::Unload()
{
	m_Tga.End();
	m_TgaPS2Dev.End();
}

//do app mode logic
void CZoomAppMode::Logic()
{
	switch(m_state)
	{
		case Zoom:
			//zoom width
			m_w+=m_step;

			//Rich width?
			if(m_w>m_Tga.m_Width-1)
			{
				//don't overflow
				m_w=m_Tga.m_Width-1;

				m_state = CrossFade;
				m_alfa = 32;
			}
			//zoom height
			m_h+=m_step;

			//Rich height?
			if(m_h>m_Tga.m_Height-1)
			{
				//don't overflow
				m_h=m_Tga.m_Height-1;
			}

			//calculate center position
			m_rDest.left=(GetWidth()-m_w)>>1;
			m_rDest.right=m_rDest.left+m_w;
			m_rDest.top=(GetHeight()-m_h)>>1;
			m_rDest.bottom=m_rDest.top+m_h;
		break;
		case CrossFade:
			m_alfa--;
			if(m_alfa<0)
			{
				m_state = Still;
				m_alfa = 60*2;
			}
		break;
		case Still:
			m_alfa--;
			if(m_alfa<0)
			{
				m_state = Fading;
				m_alfa = 32;
			}
		break;
		case Fading:
			m_alfa--;
			if(m_alfa<0)
			{
				//NextMode();
				SetAppMode(CPOLLOPOLLOGame::LoadingMode,CPOLLOPOLLOGame::IntroAppMode);
			}	
		break;
	}
}

//draw app mode
void CZoomAppMode::Draw()
{
	switch(m_state)
	{
		case Zoom:		
			//Clear screen
			GetWorkingScreen()->Clear(m_bgColor);

			//zoom FULL image to working screen area
			m_Tga.Zoom(m_rDest.left,m_rDest.top,m_rDest.right,m_rDest.bottom);
		break;
		case CrossFade:
			m_Tga.ScreenCopyInk(255,255,255,m_alfa);
			m_TgaPS2Dev.ScreenCopyTrans(m_rDest.left,m_rDest.top,m_rDest.right,m_rDest.bottom,
				m_rDest.left,m_rDest.top,32-m_alfa);
		break;
		case Still:
			m_TgaPS2Dev.ScreenCopy();
		break;
		case Fading:
			m_TgaPS2Dev.ScreenCopyInk(0,0,0,m_alfa);
		break;
	}
}
