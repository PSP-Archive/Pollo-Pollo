
//include class definition
#include "CCongratsMode.h"

//include some objects declarations
#include "CMGELog.h"

#include "CPOLLOPOLLOGame.h"

//init CCongratsMode
bool CCongratsMode::Init(CMGEApp* pApp,int AppMode)
{	
	//finalize
	End();

	//log output
	CMGELog::SysLog(CMGELog::lInfo,"Init CCongratsMode");
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
		CMGELog::SysLog(CMGELog::lInfo,"Init CCongratsMode [Ok]");
	else
		CMGELog::SysLog(CMGELog::lInfo,"Init CCongratsMode [Fail]");	

	//return initialization status
	return IsOk();
}

//finalize object
void CCongratsMode::End()
{
	//are we ok?
	if (IsOk())
	{



		//call base finalize
		inherited::End();
	}
}

//log some info
void CCongratsMode::Log()
{	
	//log output
	CMGELog::SysLog(CMGELog::lInfo,"Logging CCongratsMode class");
	CMGELog::SysIdent();

	CMGELog::SysUnident();
}

//load app mode
bool CCongratsMode::Load()
{
	bool bResult = true;
	

	bResult = m_oImage.Init("dat/game/congrats/img/back.tga");
	if(bResult)
		bResult = m_oCity.Init("dat/game/congrats/sprites/city.xml");
	if(bResult)
		bResult = m_oFireWork.Init("dat/game/congrats/sprites/firework.xml");

	if(bResult)
		bResult = m_oRocket.Init("dat/game/congrats/sprites/rocket.xml");

	if(bResult)
		bResult = m_oMisc.Init("dat/game/congrats/sprites/misc.xml");
	
	if(bResult)
		bResult = m_oPump.Init("dat/game/congrats/sound/pump.xml");

	if(bResult)
		bResult = m_oMusic.Init("dat/game/congrats/sound/celebration.ogg",false);

	if(bResult)
	{
		m_nIdCongrats = m_oMisc.GetId("congrats");		

		InitRockets();

		m_nLoops=0;
		m_nActualCity=0;
		m_nFlashing=0;

		GetControl()->m_cuControl.buttons[0] = PSP_CTRL_CROSS |PSP_CTRL_CIRCLE | PSP_CTRL_SQUARE | 
			PSP_CTRL_TRIANGLE |PSP_CTRL_START | PSP_CTRL_SELECT | PSP_CTRL_LTRIGGER | PSP_CTRL_RTRIGGER;
	}

	return bResult;
}

//unload app mode
void CCongratsMode::Unload()
{
	EndRockets();

	GetSoundDevice()->StopSound();


	m_oImage.End();

	m_oCity.End();

	m_oFireWork.End();


	m_oRocket.End();


	m_oMisc.End();


	m_oPump.End();


	m_oMusic.End();
	
	InitRockets();
}

//do app mode logic
void CCongratsMode::Logic()
{
	if(m_nLoops==0)
		GetSoundDevice()->Play(&m_oMusic);
	m_nLoops++;

	if((m_nLoops>60*60)||(GetControl()->m_cuResponse.buttons[0].wasPressed)||(m_oMusic.IsStoping()))
	{
		//going to next but going from loading screen
		int nMode = GetNextMode();
		
		SetAppMode(CPOLLOPOLLOGame::LoadingMode,nMode);
	}
	else
	{
		if(!m_nFlashing)
		{
			RocketsLogic();
		}
		else
		{
			m_nFlashing--;
			if(m_nFlashing<30)
				m_nFlashing=0;
		}
	}
}

//draw app mode
void CCongratsMode::Draw()
{
	if(!m_nFlashing)
	{
		//draw background
		m_oImage.ScreenCopy();

		//draw rockets
		DrawRockets();

		//draw front city
		m_oCity.Draw(0,0,0);

		//if we are lighting a city, draw it
		if(m_nActualCity)
		{			
			m_oCity.TransDraw(m_nActualCity,0,0,m_nActualCityAlfa);

			m_oMisc.InkDraw(m_nIdCongrats,GetRenderDevice()->GetWidth()>>1,GetRenderDevice()->GetHeight()>>1,
				tcColors[m_nActualCity-1].r,tcColors[m_nActualCity-1].g,tcColors[m_nActualCity-1].b,(32-(m_nActualCityAlfa>>1)));


		}
		else
		{
			m_oMisc.Draw(m_nIdCongrats,GetRenderDevice()->GetWidth()>>1,GetRenderDevice()->GetHeight()>>1);
		}

		
		
	}
	else
	{
		m_oImage.ScreenCopyInk(255,255,255,32-m_nFlashing,0,0);	

		//m_oMisc.Draw(m_nIdCongrats,GetRenderDevice()->GetWidth()>>1,GetRenderDevice()->GetHeight()>>1);

		m_oCity.InkDraw((WORD)0,0,0,(BYTE)255,(BYTE)255,(BYTE)255,(BYTE)32-m_nFlashing);
	}

}

void CCongratsMode::RocketsLogic()
{
	int nNext=0;
	for(int n=0;n<eMaxRockets;n++)
	{
		m_stRockets[n].alive++;
		switch(m_stRockets[n].state)
		{
			case rsGoingUP:
				m_stRockets[n].y-=2;
				if(m_stRockets[n].y<=50+(rand()%25))
				{
					m_oPump.Play(rand()%m_oPump.m_TotalSounds);

					m_stRockets[n].state=rsFlaking;
					nNext=n+1;
					if(nNext>=eMaxRockets)
						nNext=0;

					m_stRockets[nNext].state=rsGoingUP;
					m_stRockets[nNext].x=50+rand()%((GetRenderDevice()->GetWidth()>>1)-50);
					m_stRockets[nNext].y=GetRenderDevice()->GetHeight();
					m_stRockets[nNext].sprite=0;
					m_stRockets[nNext].color=nNext;

					m_nActualCity=n+1;
					m_nActualCityAlfa=32;
					m_nFlashing=32;

				}
			break;
			case rsFlaking:
				//if((m_nLoops%2)==0)
				{
					m_stRockets[n].sprite++;
					if(m_stRockets[n].sprite>=m_oFireWork.GetTotalSprites()-1)
					{
						m_stRockets[n].state=rsFree;
						m_nActualCity=0;
					}					
					else
					{
						m_nActualCityAlfa = 32 - (m_stRockets[n].sprite*32/(m_oFireWork.GetTotalSprites()-1));
					}
				}
			break;
		}
	}
}

void CCongratsMode::DrawRockets()
{
	int iAlfa = 0;

	for(int n=0;n<eMaxRockets;n++)
	{
		switch(m_stRockets[n].state)
		{
			case rsGoingUP:
				//m_oRocket.Draw(m_stRockets[n].sprite,m_stRockets[n].x,m_stRockets[n].y);
				m_oRocket.Draw(m_stRockets[n].sprite,m_stRockets[n].x,m_stRockets[n].y);
			break;
			case rsFlaking:
				//m_oFireWork.Draw(m_stRockets[n].sprite,m_stRockets[n].x,m_stRockets[n].y);

				iAlfa = 32 - (m_stRockets[n].sprite*16/(m_oFireWork.GetTotalSprites()-1));

				m_oFireWork.AddInkDraw(m_stRockets[n].sprite,m_stRockets[n].x,m_stRockets[n].y,
					tcColors[m_stRockets[n].color].r,
					tcColors[m_stRockets[n].color].g,
					tcColors[m_stRockets[n].color].b,iAlfa,0);
			break;
		}
	}
}

void CCongratsMode::InitRockets()
{
	for(int n=0;n<eMaxRockets;n++)
	{

		m_stRockets[n].state = rsFree;		
		m_stRockets[n].alive=0;
		m_stRockets[n].next=0;
		m_stRockets[n].color = 0;
		m_stRockets[n].sprite=0;
	}
	m_stRockets[0].state=rsGoingUP;
	m_stRockets[0].x=GetRenderDevice()->GetWidth()>>1;
	m_stRockets[0].y=GetRenderDevice()->GetHeight()+15;
}

void CCongratsMode::EndRockets()
{
	InitRockets();
}


CCongratsMode::TColor CCongratsMode::tcColors[]={{255,0,0},{0,255,0},{0,0,255}};
