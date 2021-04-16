
//App includes
#include "CPOLLOPOLLOGame.h"

#include "CIntroAppMode.h"
#include "CZoomAppMode.h"
#include "CMenuMode.h"
#include "CPlayingMode.h"
#include "CLoadingMode.h"
#include "CEndAppMode.h"
#include "CCongratsMode.h"
//Setting app desc
PSP_MODULE_INFO("NewOlds POLLO POLLO! 1.0", 0, 1, 1);

//Load data
bool CPOLLOPOLLOGame::Load()
{
	//loading result
	bool bResult = true;

	bResult = m_oFont.Init("dat/font/white/font.xml");
	//bResult = m_oFont.Init("dat/font/white2/white2.xml");

	if(bResult)
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

		srand(dwTime);

		//configure playable control, button 0 (Exit)
		m_oControl.m_cuControl.buttons[0]=PSP_CTRL_SELECT;

		//app modes
		AddApplicationMode(new CIntroAppMode(),IntroAppMode);
		AddApplicationMode(new CZoomAppMode(),ZoomAppMode);
		AddApplicationMode(new CMenuMode(),MenuMode);
		AddApplicationMode(new CPlayingMode(),PlayingMode);
		AddApplicationMode(new CLoadingMode,LoadingMode);
		AddApplicationMode(new CEndAppMode,EndAppMode);
		AddApplicationMode(new CCongratsMode,CongratsMode);
		
		bResult=SetAppMode(ZoomAppMode,IntroAppMode);		

		m_pmMode=CPUvsCPU;

		m_bSoundOn=true;
		m_bMusicOn=true;

		m_bEasyWin=false;
		m_bEasyWinCheat=false;

		m_bClearAll=false;
		m_bClearAllCheat=false;

		m_bClearGhost=false;
		m_bClearGhostCheat=false;

		m_bClearLast=false;
		m_bClearLastCheat=false;
		
		m_nHistoryLevel=-1;
		m_nPosibleRecordMode=CPUvsCPU;
		m_nPossibleRecord[0]=0;
		m_nPossibleRecord[0]=0;

		char sValue[256];

		GetConfigValue("cheats","easywin",sValue,sizeof(sValue),"false");
		if(strcmp(sValue,"true")==0)
			m_bEasyWinCheat=true;

		GetConfigValue("cheats","clear-all",sValue,sizeof(sValue),"false");
		if(strcmp(sValue,"true")==0)
			m_bClearAllCheat=true;

		GetConfigValue("cheats","clear-ghost",sValue,sizeof(sValue),"false");
		if(strcmp(sValue,"true")==0)
			m_bClearGhostCheat=true;

		GetConfigValue("cheats","clear-last",sValue,sizeof(sValue),"false");
		if(strcmp(sValue,"true")==0)
			m_bClearLastCheat=true;
	}

	//return load status
	return bResult;
}

void CPOLLOPOLLOGame::Unload()
{

}

//App AI Logic
void CPOLLOPOLLOGame::Logic()
{ 	
	m_bEasyWin=false;
	//button 0 (Exit/Cheat) pushed. Exit app. 
	if(m_oControl.m_cuResponse.buttons[0].wasPressed)
	{
		bool bCheat=false;
		if(m_bEasyWinCheat&&m_oControl.m_cuResponse.up)
		{
			m_bEasyWin=true;
			bCheat=true;
		}
		if(m_bClearAllCheat&&m_oControl.m_cuResponse.left)
		{
			m_bClearAll=true;
			bCheat=true;
		}
		if(m_bClearGhostCheat&&m_oControl.m_cuResponse.rigth)
		{
			m_bClearGhost=true;
			bCheat=true;
		}
		if(m_bClearLastCheat&&m_oControl.m_cuResponse.down)
		{
			m_bClearLast=true;
			bCheat=true;
		}

		/*if(!bCheat)
			m_bExit=true;*/
	}
}

//App Draw frame
void CPOLLOPOLLOGame::Draw()
{
	//m_oFont.Write(10,10,"FPS %.2f",GetRenderDevice()->GetFPS());
}

CPOLLOPOLLOGame::GameLevelConfig CPOLLOPOLLOGame::GetGameLevelConfig(int nLevel)
{
	CPOLLOPOLLOGame::GameLevelConfig gcResult;
	char sSection[255];
	char sValue[255];

	sprintf(sSection,"level_%d",nLevel);

	GetConfigValue(sSection,"speed",sValue,sizeof(sValue),"2");
	gcResult.m_nSpeed=atoi(sValue);

	GetConfigValue(sSection,"cpuPressingDown",sValue,sizeof(sValue),"0");
	gcResult.m_nCpuPressingDown=atoi(sValue);

	GetConfigValue(sSection,"cpuReleasingDown",sValue,sizeof(sValue),"0");
	gcResult.m_nCpuReleasingDown=atoi(sValue);

	return (gcResult);
}
