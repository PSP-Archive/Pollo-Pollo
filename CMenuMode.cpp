
//include class definition
#include "CMenuMode.h"

//include some objects declarations
#include "CMGELog.h"

#include "CPOLLOPOLLOGame.h"

#if defined(PSP_VER)
	#include <psppower.h>
#endif

//init CMenuMode
bool CMenuMode::Init(CMGEApp* pApp,int AppMode)
{	
	//finalize
	End();

	//log output
	CMGELog::SysLog(CMGELog::lInfo,"Init CMenuMode");
	CMGELog::SysIdent();

	//base initialize
	inherited::Init(pApp,AppMode);

	//are we ok?
	if(IsOk())
	{
		m_pParent = (CPOLLOPOLLOGame*)pApp;
		m_sPosibleChars[0]=245;
		m_sPosibleChars[1]=246;
	}

	//log output
	Log();

	CMGELog::SysUnident();	
	

	//are we ok?
	if (IsOk())
		CMGELog::SysLog(CMGELog::lInfo,"Init CMenuMode [Ok]");
	else
		CMGELog::SysLog(CMGELog::lInfo,"Init CMenuMode [Fail]");	

	//return initialization status
	return IsOk();
}

//finalize object
void CMenuMode::End()
{
	//are we ok?
	if (IsOk())
	{
		//call base finalize
		inherited::End();
	}
}

//log some info
void CMenuMode::Log()
{	
	//log output
	CMGELog::SysLog(CMGELog::lInfo,"Logging CMenuMode class");
	CMGELog::SysIdent();

	CMGELog::SysUnident();
}

int CMenuMode::m_nCycles[]={cCycleScore,cCycleDemo,cCycleScore,cCycleIntro,cLastCycle};

//load app mode
bool CMenuMode::Load()
{
	bool bResult = true;

	m_nIdMusic = -1;
	//load music
	bResult = m_oMusic.Init("dat/menu/sound/menu.ogg");	

	if(bResult)
	{
		//m_nCycles[0];{cCycleScore,cCycleDemo,cCycleScore,cCycleIntro}; 
		//Load background
		bResult = m_BackTga.Init("dat/menu/background.tga");

		m_nRenderWidth = GetRenderDevice()->GetWidth();
		m_nRenderHeight = GetRenderDevice()->GetHeight();

		m_nImageWidth = m_BackTga.m_Width;
		m_nImageHeight = m_BackTga.m_Height;

		m_nWidthParts = (m_nRenderWidth / m_nImageWidth)+2;
		m_nHeightParts = (m_nRenderHeight / m_nImageHeight)+2;

		m_nScrollX=0;
		m_nScrollY=0;
		m_nLoops=0;
		m_nAlfa=0;
		m_nActualCycle=0;

		if (bResult)
		{				
			bResult = m_Sprites.Init("dat/menu/sprites/menu.xml");
			
			m_idStoryMode = m_Sprites.GetId("storymode");
			m_idChoose2 = m_Sprites.GetId("choose2");
			m_id1Player = m_Sprites.GetId("1player");
			m_id2Players = m_Sprites.GetId("2players");
			m_idOptions = m_Sprites.GetId("options");
			m_idExit = m_Sprites.GetId("exit");
			m_idBack = m_Sprites.GetId("back");
			
			m_idStorymoderecords = m_Sprites.GetId("Storymoderecords");
			m_id1plvscpurecords = m_Sprites.GetId("1plvscpu_records");
			m_idpollomaniarecords = m_Sprites.GetId("pollomania_records");
			m_idrecords  = m_Sprites.GetId("records");

			m_idStory = m_Sprites.GetId("story");
			m_idPlayerVsCPU = m_Sprites.GetId("1playerVsCpu");
			m_id1PlayerGame = m_Sprites.GetId("1playerGame");
			m_id2PlayersGame = m_Sprites.GetId("2playersgame");
			M_idPolloMania = m_Sprites.GetId("pollomania");
			m_idPolloManiaGame = m_Sprites.GetId("pollomaniagame");
			
			m_idStart = m_Sprites.GetId("start");
			m_idChoose = m_Sprites.GetId("choose");

			m_idMark = m_Sprites.GetId("mark");

			m_idPlayer1text = m_Sprites.GetId("player1text");
			m_idPlayer2text = m_Sprites.GetId("player2text");
			m_idCputext = m_Sprites.GetId("cputext");

			m_idLocked = m_Sprites.GetId("locked");
			m_idLockedSmall = m_Sprites.GetId("lockedsmall");

			m_idOptionsText = m_Sprites.GetId("optionstext");

			m_idBigLogo = m_Sprites.GetId("biglogo");
			m_idLogParts[LOGO_PART_P2] = m_Sprites.GetId("biglogo_P2");
			m_idLogParts[LOGO_PART_L] = m_Sprites.GetId("biglogo_L");
			m_idLogParts[LOGO_PART_O] = m_Sprites.GetId("biglogo_O");
			m_idLogParts[LOGO_PART_P1] = m_Sprites.GetId("biglogo_P1");
			
			m_nLastLogoPiece=0;
			AddLogoPiece(LOGO_PART_P1,      15,19);
			AddLogoPiece(LOGO_PART_O,       77,75);
			AddLogoPiece(LOGO_PART_L,      129,50);
			AddLogoPiece(LOGO_PART_L,      159,50);
			AddLogoPiece(LOGO_PART_O,      190,75);

			AddLogoPiece(LOGO_PART_P2,     245,54);
			AddLogoPiece(LOGO_PART_O,      297,75);
			AddLogoPiece(LOGO_PART_L,      349,50);
			AddLogoPiece(LOGO_PART_L,      379,50);
			AddLogoPiece(LOGO_PART_O,      410,75);

			m_idEasy = m_Sprites.GetId("easy");
			m_idMedium = m_Sprites.GetId("medium");
			m_idHard = m_Sprites.GetId("hard");

			m_idSound[0] = m_Sprites.GetId("sound_off");
			m_idSound[1] = m_Sprites.GetId("sound_on");
			m_idMusic[0] = m_Sprites.GetId("music_off");
			m_idMusic[1] = m_Sprites.GetId("music_on");

			char sBuf[255];
			char sValue[255];

			m_pApp->GetConfigValue("game","level",sValue,sizeof(sValue),"1");
			m_nGameLevel = atoi(sValue);							

			m_pApp->GetConfigValue("game","sound",sValue,sizeof(sValue),"1");
			m_nSoundOn = atoi(sValue);	
			m_pApp->GetConfigValue("game","music",sValue,sizeof(sValue),"1");
			m_nMusicOn = atoi(sValue);	

			for(int c=0;c<cTotalChars;c++)
			{					
				sprintf(sBuf,"facesmall%02doff",c+1);
				m_idFacesSmOff[c]=m_Sprites.GetId(sBuf);

				sprintf(sBuf,"facesmall%02d",c+1);
				m_idFacesSm[c]=m_Sprites.GetId(sBuf);

				sprintf(sBuf,"face%02d",c+1);
				m_idFaces[c]=m_Sprites.GetId(sBuf);				

				sprintf(sBuf,"char%d",c+1);
				m_pApp->GetConfigValue("characters",sBuf,sValue,sizeof(sValue),"normal");
				if(strcmp(sValue,"locked")==0)
					m_bLockedChars[c]=true;
				else
					m_bLockedChars[c]=false;				
			}

			m_pApp->GetConfigValue("game","pollomania",sValue,sizeof(sValue),"locked");
			if(strcmp(sValue,"locked")==0)
				m_bPolloManiaLocked=true;
			else
				m_bPolloManiaLocked=false;							

			if (bResult)
			{
				bResult = m_Sounds.Init("dat/menu/sound/menu.xml");

				m_idSelmenu = m_Sounds.GetId("selmenu");
				m_idTranslate = m_Sounds.GetId("translate");
				m_idLogo = m_Sounds.GetId("pollopollo");

				if (bResult)
				{
					//configure playable control, button 0 (Exit)
					GetControl()->m_cuControl.buttons[0]=PSP_CTRL_SELECT;
					GetControl()->m_cuControl.buttons[1]=PSP_CTRL_CROSS;
					GetControl()->m_cuControl.buttons[2]=PSP_CTRL_CIRCLE;
					GetControl()->m_cuControl.buttons[3]=PSP_CTRL_START;
					GetControl()->m_cuControl.buttons[4]=PSP_CTRL_TRIANGLE|PSP_CTRL_SQUARE;

					m_nLoopsNoChoice=0;
					m_nLoopNoImput=0;
					ChangeMenu(0,4,0);

					m_nCenterPos = GetWidth()>>1;

					m_nState = msMovingLogo;	

					m_nStoryChar[0]=0;
					m_nStoryChar[1]=2;
					m_nStoryChar[2]=5;					

					ReadScore(m_scScoreSingle,"score_single");
					ReadScore(m_scScoreStory,"score_story");
					ReadScore(m_scScorePollomania,"score_pollomania");
					
					pCurrentScore=m_scScoreSingle;
					pNextScore=NULL;
					m_nChangedScore=-1;
					m_nBlinkChar=false;

					CPOLLOPOLLOGame::TPlayingMode pmRecordMode = m_pParent->GetPosibleRecordMode();

					int idPlayer = m_pParent->GetPosibleRecordId(0);
					DWORD nPoints = m_pParent->GetPosibleRecord(0); 
					BYTE nLevel = m_pParent->GetPosibleRecordLevel();

					if(pmRecordMode==CPOLLOPOLLOGame::PlayerVsCPU)
					{
						CheckScore(m_scScoreSingle,nPoints,idPlayer,nLevel);
					}
					else if (pmRecordMode==CPOLLOPOLLOGame::History) 
					{
						CheckScore(m_scScoreStory,nPoints,idPlayer,nLevel);
					}
					else if (pmRecordMode==CPOLLOPOLLOGame::Pollomania) 
					{
						CheckScore(m_scScorePollomania,nPoints,idPlayer,nLevel);
					}

					//CheckScore(m_scScorePollomania,150000,1,1);
				}
			}
		}
	}

	return bResult;
}

//unload app mode
void CMenuMode::Unload()
{
	char sValue[255];
	sprintf(sValue,"%d",m_nGameLevel);
	m_pApp->SetConfigValue("game","level",sValue);	

	sprintf(sValue,"%d",m_nMusicOn);
	m_pApp->SetConfigValue("game","music",sValue);

	sprintf(sValue,"%d",m_nSoundOn);
	m_pApp->SetConfigValue("game","sound",sValue);

	m_BackTga.End();	
	m_Sprites.End();
	m_Sounds.End();

	if(m_nIdMusic!=-1)
		FreeChannel(m_nIdMusic);

	m_oMusic.End();
}

//do app mode logic
void CMenuMode::Logic()
{
	m_nLoops++;

	#if defined(PSP_VER)
		//cause PSP to not be off
		scePowerTick(0);
	#endif

	switch(m_nState)
	{
		case msFlash:
			m_nAlfa++;

			if(m_nAlfa>32){
				m_nState=msDisplayMenu;

				if(m_nSoundOn)
					m_Sounds.Play(m_idLogo);
			}
		break;

		case msMovingLogo:
			for(int nCounterLogoPiece=0;nCounterLogoPiece<m_nLastLogoPiece;nCounterLogoPiece++)
			{
				if(m_LogoPieces[nCounterLogoPiece].y<m_LogoPieces[nCounterLogoPiece].dy)
				{
					m_LogoPieces[nCounterLogoPiece].y+=m_LogoPieces[nCounterLogoPiece].ay;

					if(m_LogoPieces[nCounterLogoPiece].y>=m_LogoPieces[nCounterLogoPiece].dy)
					{						
						m_LogoPieces[nCounterLogoPiece].y=m_LogoPieces[nCounterLogoPiece].dy;
						m_LogoPieces[nCounterLogoPiece].shakey=2;
						m_nCounterPieces--;
						if(m_nCounterPieces==0)
						{
							//m_nState=msDisplayMenu;
							m_nAlfa=0;
							m_nState=msFlash;
						}
					}
				}
				else
				{
					if((m_LogoPieces[nCounterLogoPiece].shakey!=0)&&(m_nLoops%8)==0)
					{
						m_LogoPieces[nCounterLogoPiece].shakey*=-1;
						m_LogoPieces[nCounterLogoPiece].shakey-=
							m_LogoPieces[nCounterLogoPiece].shakey/abs(m_LogoPieces[nCounterLogoPiece].shakey);
					}
				}
			}
		break;

		case msDisplayMenu:

			switch(m_nSubMenu)
			{
				case 0:
				case 1:
				case 3:
					NormalMenuLogic();
				break;

				case 2:
					ChoosePlayersLogic();
				break;

				case 4:
					ChooseStoryPlayerLogic();
				break;
			}			

		m_nLoopsNoChoice++;
		if(m_nLoopsNoChoice>(60*20))
		{			

			switch(m_nCycles[m_nActualCycle])
			{
				case cCycleScore:
					m_nLoopsToChange=cChangeScoreRate;
					m_nState=msDiplayScore;				
					pCurrentScore=m_scScoreSingle;
					pNextScore=m_scScoreStory;
					m_nChangedScore=-1;
					m_nBlinkChar=false;
				break;
				case cCycleDemo:
					GoingToDemo();
				break;
				case cCycleIntro:
					PlayIntro();
				break;
			}

			m_nLoopsNoChoice=0;
			m_nActualCycle = m_nActualCycle++;
			if(m_nCycles[m_nActualCycle]==cLastCycle)
				m_nActualCycle=0;
		}
		break;

		case msChangeMenu:
			if(m_nLoopsToChange)
				m_nLoopsToChange--;
			else
				SetAppMode(m_nChangeTo,m_nChangeNext);				
		break;

		case msChangingScore:
			if(m_nLoopsToChange)
				m_nLoopsToChange--;
			else
			{
				m_nLoopsToChange=15;
				m_nBlinkChar=!m_nBlinkChar;
			}
			
			if(GetControl()->m_cuResponse.buttons[1].wasPressed)
			{
				if((unsigned char)m_sPosibleChars[m_nCursorPos]==245)
				{
					GetControl()->m_cuResponse.buttons[2].wasPressed=true;
				}
				else if((unsigned char)m_sPosibleChars[m_nCursorPos]==246)
				{
					GetControl()->m_cuResponse.buttons[3].wasPressed=true;
				}
				else if(strlen(m_sCursorText)<12)
				{
					strcat(m_sCursorText,m_sSelectedCursor);

					if(m_nSoundOn)
						m_Sounds.Play(m_idSelmenu);
				}				
			}
			if(GetControl()->m_cuResponse.buttons[2].wasPressed)
			{
				if(strlen(m_sCursorText)>0)
				{
					m_sCursorText[strlen(m_sCursorText)-1]='\0';

					if(m_nSoundOn)
						m_Sounds.Play(m_idSelmenu);
				}
			}
			if(GetControl()->m_cuResponse.buttons[3].wasPressed)
			{
				if(strlen(m_sCursorText)>0)
				{										
					bool bSpecialImput=false;
					if(strcmpi(m_sCursorText,"YS")==0)
					{
						bSpecialImput=true;
						m_pParent->SetEasyWinCheat();
					}
					else if(strcmpi(m_sCursorText,"QUARTZ")==0)
					{
						bSpecialImput=true;
						m_pParent->SetClearAllCheat();
					}
					else if(strcmpi(m_sCursorText,"GRADIUS")==0)
					{
						bSpecialImput=true;
						m_pParent->SetClearGhostCheat();
					}
					else if(strcmpi(m_sCursorText,"RUNE MASTER")==0)
					{
						bSpecialImput=true;
						m_pParent->SetClearLastCheat();
					}
					if(bSpecialImput)
					{
						bSpecialImput=true;
						m_Sounds.Play(m_idLogo);
						strcpy(m_sCursorText,"");
						break;
					}

					strcpy(pCurrentScore[m_nChangedScore].sText,m_sCursorText);

					m_nLoopsToChange=cChangeScoreRate;
					m_nState=msDiplayScore;								
					m_nChangedScore=-1;	

					if(m_nSoundOn)
						m_Sounds.Play(m_idSelmenu);

					if(pCurrentScore==m_scScoreSingle)
						SaveScore(pCurrentScore,"score_single");
					else if(pCurrentScore==m_scScoreStory)
						SaveScore(pCurrentScore,"score_story");					
					else
						SaveScore(pCurrentScore,"score_pollomania");					

				}
			}

			if(!m_nLoopNoImput)
			{	
				bool bClick=false;
				if(GetControl()->m_cuResponse.down)
				{
					m_nCursorPos++;
					if(m_nCursorPos>(strlen(m_sPosibleChars)-1))
						m_nCursorPos=0;
					bClick=true;
				}
				if(GetControl()->m_cuResponse.up)
				{
					m_nCursorPos--;
					if(m_nCursorPos<0)
						m_nCursorPos=strlen(m_sPosibleChars)-1;
					bClick=true;
				}
				if(bClick)
				{
					m_sSelectedCursor[0]=m_sPosibleChars[m_nCursorPos];
					m_nLoopNoImput=10;
					if(m_nSoundOn)
						m_Sounds.Play(m_idTranslate);
				}
			}
			else
				m_nLoopNoImput--;

		break;
		case msDiplayScore:
			m_nLoopNoImput=0;
			if(m_nLoopsToChange)
				m_nLoopsToChange--;
			else
			{
				if(pNextScore==NULL)
				{
					m_nState=msDisplayMenu;
				}
				else
				{
					m_nLoopsToChange=cChangeScoreRate;
					pCurrentScore=pNextScore;
					if(pNextScore==m_scScoreStory)
					{
						pNextScore=m_scScorePollomania;
					}
					else if(pNextScore==m_scScorePollomania)
					{
						pNextScore=NULL;					
					}
				}
				/*if(pCurrentScore==m_scScoreSingle)
				{
					m_nLoopsToChange=cChangeScoreRate;
					pCurrentScore=m_scScoreStory;
				}
				else
				{
					m_nState=msDisplayMenu;	
				}*/
			}
			if( (GetControl()->m_cuResponse.buttons[1].wasPressed ) ||
				(GetControl()->m_cuResponse.buttons[2].wasPressed ) ||
				(GetControl()->m_cuResponse.buttons[3].wasPressed ) ||
				(GetControl()->m_cuResponse.buttons[4].wasPressed ) )
			{
				m_nState=msDisplayMenu;	
			}
		break;
	}


	if(m_nState!=msChangeMenu)
		if((m_nIdMusic==-1)&&(m_nMusicOn))
		{	
			m_nIdMusic=PlaySound(&m_oMusic);						
		}
}

//draw app mode
void CMenuMode::Draw()
{		
	switch(m_nState)
	{
		case msFlash:
			{
			DrawScrollingBG();
			DrawMovingTitle();
			m_Sprites.TransDraw(m_idBigLogo,13,16,m_nAlfa);			

			int n=0;
			const int step = 30;
			int x=160-step;

			m_Sprites.TransDraw	(m_id1Player,	m_nCenterPos,x+=step,m_nAlfa);
			m_Sprites.TransDraw	(m_id2Players,	m_nCenterPos,x+=step,m_nAlfa>>1);
			m_Sprites.TransDraw	(m_idOptions,	m_nCenterPos,x+=step,m_nAlfa>>1);
			m_Sprites.TransDraw	(m_idExit,		m_nCenterPos,x+=step,m_nAlfa>>1);
			}
		break;

		case msDisplayMenu:

			//m_BackTga.ScreenCopy();	
			DrawScrollingBG();

			switch(m_nSubMenu)
			{
				case 0:
					MainMenuDraw();
				break;
				case 1:
					SubMenu1PlayerDraw();
				break;
				case 2:
					SubmenuChoosePlayersDraw();
				break;
				case 3:
					SubMenuOptionsDraw();
				break;
				case 4:
					SubMenuHistoryDraw();
				break;
			}	
		break;

		case msChangeMenu:			
		break;

		case msMovingLogo:
			DrawScrollingBG();
			DrawMovingTitle();
		break;

		case msChangingScore:
		case msDiplayScore:
			DrawScrollingBG();
			DrawScore(pCurrentScore);
		break;
	}	
}

void CMenuMode::MainMenuDraw()
{
	m_Sprites.Draw(m_idBigLogo,13,17);

	int n=0;
	const int step = 30;
	int y=160-step;

	m_Sprites.TransDraw	(m_id1Player,	m_nCenterPos,y+=step,(m_nSelected==n++)?28:16);
	m_Sprites.TransDraw	(m_id2Players,	m_nCenterPos,y+=step,(m_nSelected==n++)?28:16);
	m_Sprites.TransDraw	(m_idOptions,	m_nCenterPos,y+=step,(m_nSelected==n++)?28:16);	
	m_Sprites.TransDraw	(m_idExit,		m_nCenterPos,y+=step,(m_nSelected==n++)?28:16);
	
}

void CMenuMode::MainMenuClick()
{
	switch(m_nSelected)
	{
		case 0:
			ChangeMenu(1,4,0);
		break;

		case 1:
			m_pParent->SetPlayingMode(CPOLLOPOLLOGame::TwoPlayers);
			//SetAppMode(CPOLLOPOLLOGame::PlayingMode,CPOLLOPOLLOGame::MenuMode);
			//ChangeState(msChangeMenu,CPOLLOPOLLOGame::PlayingMode,CPOLLOPOLLOGame::MenuMode);
			m_nSelectPlayer[0]=0;
			m_nSelectPlayer[1]=cNumCharCols;
			m_nSelecting=0;
			ChangeMenu(2,0,0);
		break;

		case 2:			
			ChangeMenu(3,5,0);
		break;

		case 3:
			if(m_nIdMusic!=-1)
				GetSoundDevice()->Free(m_nIdMusic);

			m_pApp->GetSoundDevice()->StopSound();
			CloseApp();			
		break;
	}
}

void CMenuMode::SubMenu1PlayerDraw()
{
	m_Sprites.Draw(m_id1PlayerGame,m_nCenterPos,70);

	int n=0;
	const int step = 30;
	int x=100;

	m_Sprites.TransDraw	(m_idStory,			m_nCenterPos,x+=step,(m_nSelected==n++)?28:16);
	m_Sprites.TransDraw	(m_idPlayerVsCPU,	m_nCenterPos,x+=step,(m_nSelected==n++)?28:16);
	if(m_bPolloManiaLocked)
		m_Sprites.TransDraw	(M_idPolloMania,	m_nCenterPos,x+=step,(m_nSelected==n++)?20:8);
	else
		m_Sprites.TransDraw	(M_idPolloMania,	m_nCenterPos,x+=step,(m_nSelected==n++)?28:16);
	m_Sprites.TransDraw	(m_idBack,			m_nCenterPos,x+=step,(m_nSelected==n++)?28:16);	
}

void CMenuMode::SubMenu1PlayerClick()
{
	switch(m_nSelected)
	{
		case 0:
			m_pParent->SetPlayingMode(CPOLLOPOLLOGame::History);
			m_nSelectPlayer[0]=0;
			m_nSelectPlayer[1]=cTotalChars-1;
			m_nSelecting=0;

			ChangeMenu(4,0,0);
		break;
		case 1:
			m_pParent->SetPlayingMode(CPOLLOPOLLOGame::PlayerVsCPU);
			//SetAppMode(CPOLLOPOLLOGame::PlayingMode,CPOLLOPOLLOGame::MenuMode);
			//ChangeState(msChangeMenu,CPOLLOPOLLOGame::PlayingMode,CPOLLOPOLLOGame::MenuMode);
			m_nSelectPlayer[0]=0;
			m_nSelectPlayer[1]=cNumCharCols;
			m_nSelecting=0;
			ChangeMenu(2,0,0);
		break;
		case 2:
			if(!m_bPolloManiaLocked)
			{
				m_pParent->SetPlayingMode(CPOLLOPOLLOGame::Pollomania);
				m_nSelectPlayer[0]=0;
				m_nSelectPlayer[1]=cNumCharCols;
				m_nSelecting=0;

				ChangeMenu(2,0,0);
			}
		break;
		case 3:
			ChangeMenu(0,4,0);
		break;
	}
}

void CMenuMode::NormalMenuLogic()
{
	if(!m_nLoopNoImput)
	{

		bool bDoSound = false;
		if(GetControl()->m_cuResponse.down)
		{
			bDoSound=true;
			m_nSelected++;
			if(m_nSelected>(m_nMaxOption-1))
				m_nSelected=0;

			m_nLoopNoImput=12;
			m_nLoopsNoChoice=0;
		}

		if(GetControl()->m_cuResponse.up)
		{
			bDoSound=true;
			m_nSelected--;
			if(m_nSelected<0)
				m_nSelected=(m_nMaxOption-1);

			m_nLoopNoImput=12;
			m_nLoopsNoChoice=0;
		}

		if(bDoSound&&(m_nSoundOn))
			m_Sounds.Play(m_idTranslate);
	}
	else
		m_nLoopNoImput--;

	if(GetControl()->m_cuResponse.buttons[1].wasPressed)
	{
		m_nLoopsNoChoice=0;

		switch(m_nSubMenu)
		{			
			case 0:
				MainMenuClick();
			break;
			case 1:
				SubMenu1PlayerClick();
			break;
			case 3:
				SubMenuOptionsClick();
			break;
		}

		if(m_nSoundOn)
			m_Sounds.Play(m_idSelmenu);
	}
}

void CMenuMode::ChoosePlayersLogic()
{
	if(!m_nLoopNoImput)
	{
		bool bPress=false;

		if(m_nSelected<cTotalChars)
		{
			if(GetControl()->m_cuResponse.rigth)
			{
				m_nSelected++;
				if(m_nSelected>(cTotalChars-1))
					m_nSelected=0;

				bPress=true;
			}

			if(GetControl()->m_cuResponse.left)
			{
				m_nSelected--;

				if(m_nSelected<0)
					m_nSelected=cTotalChars-1;

				bPress=true;
			}

			if(GetControl()->m_cuResponse.up)
			{
				m_nSelected-=cNumCharCols;

				if(m_nSelected<0)
					m_nSelected=cTotalChars+1;

				bPress=true;
			}

			if(GetControl()->m_cuResponse.down)
			{
				m_nSelected+=cNumCharCols;

				if(m_nSelected>(cTotalChars-1))
					m_nSelected=cTotalChars;

				bPress=true;
			}
		}
		else
		{
			if(GetControl()->m_cuResponse.up)
			{
				m_nSelected--;

				if(m_nSelected<(cTotalChars))
					//m_nSelected-=(cNumCharCols>1);
					m_nSelected=m_nSelectPlayer[m_nSelecting];

				bPress=true;
			}

			if(GetControl()->m_cuResponse.down)
			{
				m_nSelected++;

				if(m_nSelected>(cTotalChars+1))
					//m_nSelected=(cNumCharCols>1);
					m_nSelected=m_nSelectPlayer[m_nSelecting];

				bPress=true;
			}
		}

		if(bPress)
		{
			m_nLoopsNoChoice=0;
			m_nLoopNoImput=12;
			if(m_nSoundOn)
				m_Sounds.Play(m_idTranslate);
		}
	}
	else
		m_nLoopNoImput--;

	if(GetControl()->m_cuResponse.buttons[1].wasPressed)
	{
		m_nLoopsNoChoice=0;
		bool bSound = true;		

		//back click, start click, character click
		if(m_nSelected==cTotalChars+1)
		{
			//back to two players or main menu
			if (m_pParent->GetPlayingMode()==CPOLLOPOLLOGame::TwoPlayers )
				ChangeMenu(0,4,0);
			else
				ChangeMenu(1,4,0);
		}
		else if(m_nSelected==cTotalChars)
		{
			if( (m_pParent->GetPlayingMode()==CPOLLOPOLLOGame::Pollomania) )
				m_nSelectPlayer[1]=m_nSelectPlayer[0]?0:1;

			if( (m_nSelectPlayer[0]!=m_nSelectPlayer[1]) &&
				(!m_bLockedChars[m_nSelectPlayer[0]]) &&
				(!m_bLockedChars[m_nSelectPlayer[1]])
			)
			{				
				m_pParent->SetHistoryGameLevel(-1);				
				m_pParent->SetGameLevel(m_nGameLevel);				
				m_pParent->SetSound(m_nSoundOn);
				m_pParent->SetMusic(m_nMusicOn);
				m_pParent->SetPlayersId(0,m_nSelectPlayer[0]+1);
				m_pParent->SetPlayersId(1,m_nSelectPlayer[1]+1);

				//ChangeState(msChangeMenu,CPOLLOPOLLOGame::PlayingMode,CPOLLOPOLLOGame::MenuMode);
				ChangeState(msChangeMenu,CPOLLOPOLLOGame::LoadingMode,CPOLLOPOLLOGame::PlayingMode);
			}	
			else
				bSound=false;
		}
		else
		{
			if( (m_nSelecting==0)&&(m_pParent->GetPlayingMode()!=CPOLLOPOLLOGame::Pollomania) )
			{				
				m_nSelecting=1;				
				m_nSelected=m_nSelectPlayer[m_nSelecting];				
			}							
			else
			{
				m_nSelecting=0;
				m_nSelected=cTotalChars;
			}			
		}

		if(bSound&&(m_nSoundOn))
			m_Sounds.Play(m_idSelmenu);
	}

	if((m_nSelected>=0)&&(m_nSelected<cTotalChars))
		m_nSelectPlayer[m_nSelecting]=m_nSelected;

}

void CMenuMode::SubmenuChoosePlayersDraw()
{
	WORD idText,idChooseText;

	CPOLLOPOLLOGame::TPlayingMode pmMode = m_pParent->GetPlayingMode();
	switch(pmMode)
	{
		case CPOLLOPOLLOGame::TwoPlayers:
			idText = m_id2PlayersGame;
			idChooseText=m_idChoose;
		break;
		case CPOLLOPOLLOGame::Pollomania:
			idText = m_idPolloManiaGame;
			idChooseText=m_idChoose2;
		break;
		default:
			idText = m_id1PlayerGame;
			idChooseText=m_idChoose;
		break;
	}

	m_Sprites.Draw(idText,m_nCenterPos,40);
	m_Sprites.Draw(idChooseText,m_nCenterPos,70);

	int absoluteX = GetRenderDevice()->GetWidth()>>1;

	absoluteX-=((cNumCharRows)*cCharWidth+cCharSeparation);
	absoluteX+=(cCharWidth>>1);

	int x=absoluteX,y=cCharStartY;

	int c=0;

	//draw characters
	for (int row=0;row<cNumCharRows;row++)
	{
		x=absoluteX;
		for (int col=0;col<cNumCharCols;col++)
		{	
			/*
			if(m_nSelected>cTotalChars)
			{
				m_Sprites.Draw((c!=m_nSelected)?m_idFacesSmOff[c]:m_idFacesSm[c],x,y);
			}*/

			if(m_bLockedChars[c])
				m_Sprites.Draw(m_idLockedSmall,x,y);
			else
				m_Sprites.Draw((c!=m_nSelected)?m_idFacesSmOff[c]:m_idFacesSm[c],x,y);

			if(m_nSelectPlayer[0]==c)
			{
				m_Sprites.InkDraw(m_idMark,x,y,0,0,255,0);
			}
			else if((m_nSelectPlayer[1]==c)&&(pmMode!=CPOLLOPOLLOGame::Pollomania))
			{
				m_Sprites.InkDraw(m_idMark,x,y,255,0,0,0);
			}
			

			x+=(cCharWidth+cCharSeparation);
			c++;
		}	
		y+=(cCharHeight+cCharSeparation);		
	}	

	m_Sprites.TransDraw(m_idStart,m_nCenterPos,210,(cTotalChars==m_nSelected)?32:16);
	m_Sprites.TransDraw(m_idBack,m_nCenterPos,240,((cTotalChars+1)==m_nSelected)?32:16);
	
	//Draw big players
	if(m_bLockedChars[m_nSelectPlayer[0]])
		m_Sprites.Draw(m_idLocked,cCharSpanX,cCharSpanY);
	else
		m_Sprites.Draw(m_idFaces[m_nSelectPlayer[0]],cCharSpanX,cCharSpanY);

	if(pmMode!=CPOLLOPOLLOGame::Pollomania)
	{
		if(m_bLockedChars[m_nSelectPlayer[1]])
			m_Sprites.Draw(m_idLocked,GetRenderDevice()->GetWidth()-cCharSpanX,cCharSpanY);	
		else		
			m_Sprites.Draw(m_idFaces[m_nSelectPlayer[1]],GetRenderDevice()->GetWidth()-cCharSpanX,cCharSpanY);	
	}
	
	//draw text
	WORD idText1,idText2;

	idText1 = m_idPlayer1text;
	if (pmMode==CPOLLOPOLLOGame::TwoPlayers )
		idText2 = m_idPlayer2text;
	else
		idText2 = m_idCputext;		

	m_Sprites.Draw(idText1,cCharSpanX,cCharStartY-30);
	if(pmMode!=CPOLLOPOLLOGame::Pollomania)
		m_Sprites.Draw(idText2,GetRenderDevice()->GetWidth()-cCharSpanX,cCharStartY-30);	
}


void CMenuMode::SubMenuOptionsDraw()
{
	m_Sprites.Draw(m_idOptionsText,	m_nCenterPos,70);
	int nId=0;
	switch(m_nGameLevel)
	{
		case 0:
			nId=m_idEasy;
		break;
		case 1:
			nId=m_idMedium;
		break;
		case 2:
			nId=m_idHard;
		break;
	}

	int n=0;
	const int step = 30;
	int y=100;
	m_Sprites.TransDraw	(nId,					m_nCenterPos,y+=step,(m_nSelected==n++)?28:16);
	m_Sprites.TransDraw	(m_idSound[m_nSoundOn],	m_nCenterPos,y+=step,(m_nSelected==n++)?28:16);
	m_Sprites.TransDraw	(m_idMusic[m_nMusicOn],	m_nCenterPos,y+=step,(m_nSelected==n++)?28:16);
	m_Sprites.TransDraw	(m_idrecords,			m_nCenterPos,y+=step,(m_nSelected==n++)?28:16);
	m_Sprites.TransDraw	(m_idBack,				m_nCenterPos,y+=step,(m_nSelected==n++)?28:16);

}

void CMenuMode::SubMenuOptionsClick()
{
	switch(m_nSelected)
	{
		case 0:
			m_nGameLevel++;
			if(m_nGameLevel>2)
				m_nGameLevel=0;
		break;
		case 1:
			m_nSoundOn=m_nSoundOn?0:1;
		break;
		case 2:
			m_nMusicOn=m_nMusicOn?0:1;;
			if(!m_nMusicOn)
			{
				if(m_nIdMusic!=-1)
				{
					GetSoundDevice()->Free(m_nIdMusic);
					m_nIdMusic=-1;
				}
			}		
		break;
		case 3:
			m_nLoopsToChange=cChangeScoreRate;
			m_nState=msDiplayScore;				
			pCurrentScore=m_scScoreSingle;
			pNextScore=m_scScoreStory;
			m_nChangedScore=-1;
			m_nBlinkChar=false;
			m_nActualCycle=0;
		break;
		case 4:
			ChangeMenu(0,4,0);
		break;
	}
}

void CMenuMode::DrawScrollingBG()
{
	int localx=0, localy=0;
	for(int nCounterWidth=0;nCounterWidth<m_nWidthParts;nCounterWidth++)
	{
		localy=0;
		for(int nCounterHeight=0;nCounterHeight<m_nHeightParts;nCounterHeight++)
		{
			m_BackTga.ScreenCopyClip(localx+m_nScrollX-m_nImageHeight,
					localy+m_nScrollY);	
			localy+=m_nImageHeight;
		}		
		localx+=m_nImageWidth;
	}	

	m_nScrollX++;

	if(m_nScrollX>m_nImageWidth)
		m_nScrollX=0;

	m_nScrollY--;

	if(m_nScrollY<-m_nImageHeight)
		m_nScrollY=0;

}

void CMenuMode::AddLogoPiece(int id, int dx, int dy)
{
	m_LogoPieces[m_nLastLogoPiece].id = id;

	m_LogoPieces[m_nLastLogoPiece].dx = dx;
	m_LogoPieces[m_nLastLogoPiece].dy = dy;

	m_LogoPieces[m_nLastLogoPiece].x = dx;
	m_LogoPieces[m_nLastLogoPiece].y = -100-(m_nLastLogoPiece*50);

	m_LogoPieces[m_nLastLogoPiece].ay = 5;

	m_LogoPieces[m_nLastLogoPiece].shakex=0;
	m_LogoPieces[m_nLastLogoPiece].shakey=0;

	m_nLastLogoPiece++;
	m_nCounterPieces = m_nLastLogoPiece;
}

void CMenuMode::DrawMovingTitle()
{
	int id = 0;
	for(int nCounterLogoPiece=0;nCounterLogoPiece<m_nLastLogoPiece;nCounterLogoPiece++)
	{
		id = m_idLogParts[m_LogoPieces[nCounterLogoPiece].id];

		m_Sprites.Draw(id,m_LogoPieces[nCounterLogoPiece].x+m_LogoPieces[nCounterLogoPiece].shakex,
			m_LogoPieces[nCounterLogoPiece].y+m_LogoPieces[nCounterLogoPiece].shakey);
	}
}

void CMenuMode::SubMenuHistoryDraw()
{
	m_Sprites.Draw(m_idStoryMode,m_nCenterPos,10);
	m_Sprites.Draw(m_idChoose2,m_nCenterPos,40);

	int absoluteX = GetRenderDevice()->GetWidth()>>1;	

	int nChar=m_nStoryChar[m_nSelectPlayer[0]];

	m_Sprites.Draw(m_idFaces[nChar],absoluteX,120);


	absoluteX-=(((cTotalStoryChars*cCharWidth)+(cTotalStoryChars*cCharSeparation)))>>1;
	absoluteX+=(cCharWidth>>1);

	int c=0;
	int x=absoluteX,y=210;

	

	for (int col=0;col<cTotalStoryChars;col++)
	{	
		nChar = m_nStoryChar[c];

		m_Sprites.Draw((c!=m_nSelected)?m_idFacesSmOff[nChar]:m_idFacesSm[nChar],x,y);

		if(m_nSelectPlayer[0]==c)
		{
			m_Sprites.InkDraw(m_idMark,x,y,0,0,255,0);
		}	

		x+=(cCharWidth+cCharSeparation);
		c++;
	}	
	

	m_Sprites.TransDraw(m_idStart,m_nCenterPos-60,250,(cTotalStoryChars==m_nSelected)?32:16);
	m_Sprites.TransDraw(m_idBack,m_nCenterPos+60,250,((cTotalStoryChars+1)==m_nSelected)?32:16);
	
}


void CMenuMode::ChooseStoryPlayerLogic()
{
	if(!m_nLoopNoImput)
	{
		bool bPress=false;

		if(GetControl()->m_cuResponse.rigth)
		{
			m_nSelected++;

			if(m_nSelected>(cTotalStoryChars+1))
				m_nSelected=0;

			bPress=true;
		}

		if(GetControl()->m_cuResponse.left)
		{
			m_nSelected--;

			if(m_nSelected<0)
				m_nSelected=cTotalStoryChars+1;

			bPress=true;
		}

		if(GetControl()->m_cuResponse.up)
		{
			if(m_nSelected>=cTotalStoryChars)
				m_nSelected=m_nSelectPlayer[0];
			else
				m_nSelected=cTotalStoryChars;

			bPress=true;
		}

		if(GetControl()->m_cuResponse.down)
		{
			if(m_nSelected>cTotalStoryChars)
				m_nSelected=m_nSelectPlayer[0];
			else
				m_nSelected=cTotalStoryChars;

			bPress=true;
		}

		if(bPress)
		{
			m_nLoopsNoChoice=0;
			m_nLoopNoImput=12;
			if(m_nSoundOn)
				m_Sounds.Play(m_idTranslate);
		}
	}
	else
		m_nLoopNoImput--;

	if(GetControl()->m_cuResponse.buttons[1].wasPressed)
	{
		m_nLoopsNoChoice=0;
		bool bSound = true;		

		//back click, start click, character click
		if(m_nSelected==cTotalStoryChars+1)
		{
			//back to two players or main menu
			if (m_pParent->GetPlayingMode()==CPOLLOPOLLOGame::TwoPlayers )
				ChangeMenu(0,4,0);
			else
				ChangeMenu(1,4,0);
		}
		else if(m_nSelected==cTotalStoryChars)
		{
			switch( m_nSelectPlayer[0])
			{
				//pollajares vs pollobot
				case 0:
					m_pParent->SetPlayersId(0,1);
					m_pParent->SetPlayersId(1,2);
				break;
				//pollodrino vs pollajares
				case 1:
					m_pParent->SetPlayersId(0,3);
					m_pParent->SetPlayersId(1,1);
				break;
				//pollola vs pollozila
				case 2:
					m_pParent->SetPlayersId(0,6);
					m_pParent->SetPlayersId(1,5);
				break;
			}

			m_pParent->SetHistoryGameLevel(0);			
			m_pParent->SetGameLevel(m_nGameLevel);			
			m_pParent->SetSound(m_nSoundOn);
			m_pParent->SetMusic(m_nMusicOn);

			ChangeState(msChangeMenu,CPOLLOPOLLOGame::LoadingMode,CPOLLOPOLLOGame::PlayingMode);

		}
		else
		{

			m_nSelected=cTotalStoryChars;
		}

		if(bSound&&(m_nSoundOn))
			m_Sounds.Play(m_idSelmenu);
	}

	if((m_nSelected>=0)&&(m_nSelected<cTotalStoryChars))
		m_nSelectPlayer[m_nSelecting]=m_nSelected;

}
void CMenuMode::PlayIntro()
{
	ChangeState(msChangeMenu,CPOLLOPOLLOGame::LoadingMode,CPOLLOPOLLOGame::IntroAppMode);				
}

void CMenuMode::GoingToDemo()
{
	int c=0;
	int nRandomChars[]={-1,-1};
	while((nRandomChars[0]==-1)||(nRandomChars[1]==-1))
	{
		for(c=0;c<2;c++)
		{
			if(nRandomChars[c]==-1)
			{
				nRandomChars[c]=rand()%cTotalChars;
				if(m_bLockedChars[nRandomChars[c]])
				{
					nRandomChars[c]=-1;
				}
				else
				{								
					if(nRandomChars[0]==nRandomChars[1])
					{
						nRandomChars[c]=-1;
					}
					
				}
			}
		}								
	}

	m_pParent->SetHistoryGameLevel(-1);
	m_pParent->SetGameLevel(1);
	m_pParent->SetSound(m_nSoundOn);
	m_pParent->SetMusic(m_nMusicOn);
	m_pParent->SetPlayersId(0,nRandomChars[0]+1);
	m_pParent->SetPlayersId(1,nRandomChars[1]+1);
	m_pParent->SetPlayingMode(CPOLLOPOLLOGame::CPUvsCPU);
	ChangeState(msChangeMenu,CPOLLOPOLLOGame::LoadingMode,CPOLLOPOLLOGame::PlayingMode);
}

void CMenuMode::DrawScore(Score* m_scScore)
{	
	POINT pos;
	POINT shift;

	WORD id = m_id1plvscpurecords;

	if(m_scScore==m_scScoreStory)
		id = m_idStorymoderecords;
	else if(m_scScore==m_scScoreSingle)
		id = m_id1plvscpurecords;
	else if(m_scScore==m_scScorePollomania)
		id = m_idpollomaniarecords;

	m_Sprites.Draw(id,m_nCenterPos,40);
	

	pos.x=cScoreTextStartX;
	pos.y=cScoreTextStartY;

	bool nBlinkChar=m_nBlinkChar;

	int nScore = 0;
	for(nScore=0;nScore<cMaxScores;nScore++)
	{			
		if(m_nChangedScore==nScore)
		{
			nBlinkChar=m_nBlinkChar;
		}
		else
		{
			nBlinkChar=true;
		}
		pos.x=cScoreTextStartX;

		if(nBlinkChar)
			m_Sprites.Draw(m_idFacesSm[m_scScore[nScore].nChar-1],pos.x,pos.y);		
		pos.x+=cIconColumWidth;

		if(nBlinkChar)
		{
			if(m_scScore!=m_scScorePollomania)
			{
				switch(m_scScore[nScore].nLevel)
				{
					case 0:
						m_pParent->GetFont()->Write(pos.x,pos.y,"easy");
					break;
					case 1:
						m_pParent->GetFont()->Write(pos.x,pos.y,"medium");
					break;
					case 2:
						m_pParent->GetFont()->Write(pos.x,pos.y,"hard");
					break;
				}			
			}
			else
			{
				switch(m_scScore[nScore].nLevel)
				{
					case 1:
						m_pParent->GetFont()->Write(pos.x,pos.y,"slowest");
					break;
					case 2:
						m_pParent->GetFont()->Write(pos.x,pos.y,"slower");
					break;
					case 3:
						m_pParent->GetFont()->Write(pos.x,pos.y,"slow");
					break;
					case 4:
						m_pParent->GetFont()->Write(pos.x,pos.y,"fast");
					break;
					case 5:
						m_pParent->GetFont()->Write(pos.x,pos.y,"faster");
					break;
					case 6:
						m_pParent->GetFont()->Write(pos.x,pos.y,"fastest");
					break;
				}
			}
			
		}
		pos.x+=cLevelWidth;

		if(nBlinkChar)
			m_pParent->GetFont()->Write(pos.x,pos.y,"%d",nScore+1);
		pos.x+=cNumWidth;
		
		m_pParent->GetFont()->Write(pos.x,pos.y,"%010d",m_scScore[nScore].nPoints);
		pos.x+=cPointWidth;

		if(m_nChangedScore!=nScore)
		{
			if(strlen(m_scScore[nScore].sText)>0)
				m_pParent->GetFont()->Write(pos.x,pos.y,m_scScore[nScore].sText);
		}
		else
		{
			if(strlen(m_sCursorText)>0)
			{
				shift=m_pParent->GetFont()->Write(pos.x,pos.y,m_sCursorText);
				pos.x+=shift.x;
			}

			m_pParent->GetFont()->Write(pos.x,pos.y,m_sSelectedCursor);

			if(nBlinkChar)
				m_pParent->GetFont()->Write(pos.x,pos.y+2,"_");									

		}

		pos.y+=cItemHeight;		
	}
	
}

char CMenuMode::m_sPosibleChars[]={
				"## ABCDEFGHIJKLMNOPQRSTUVWXYZ"
				" abcdefghijklmnopqrstuvwxyz—Ò"
				" !?@#-+.,"};

void CMenuMode::ReadScore(Score* m_scScore,char* sName)
{
	char sBuf[255];
	char sValue[255];	

	int nScore = 0;
	for(nScore=0;nScore<cMaxScores;nScore++)
	{			
		sprintf(sBuf,"score%d_points",nScore);
		m_pApp->GetConfigValue(sName,sBuf,
			sValue,sizeof(sValue),"0");
		m_scScore[nScore].nPoints=atoi(sValue);

		sprintf(sBuf,"score%d_char",nScore);
		m_pApp->GetConfigValue(sName,sBuf,
			sValue,sizeof(sValue),"1");
		m_scScore[nScore].nChar=atoi(sValue);

		sprintf(sBuf,"score%d_level",nScore);
		m_pApp->GetConfigValue(sName,sBuf,
			sValue,sizeof(sValue),"0");
		m_scScore[nScore].nLevel=atoi(sValue);
		
		sprintf(sBuf,"score%d_text",nScore);
		m_pApp->GetConfigValue(sName,sBuf,
			m_scScore[nScore].sText,sizeof(m_scScore[nScore].sText),"MSX");
	}
}
void CMenuMode::SaveScore(Score* m_scScore,char* sName)
{
	char sBuf[255];
	char sValue[255];	

	int nScore = 0;
	for(nScore=0;nScore<cMaxScores;nScore++)
	{			
		sprintf(sBuf,"score%d_points",nScore);
		sprintf(sValue,"%d",m_scScore[nScore].nPoints);
		m_pApp->SetConfigValue(sName,sBuf,sValue);


		sprintf(sBuf,"score%d_char",nScore);
		sprintf(sValue,"%d",m_scScore[nScore].nChar);
		m_pApp->SetConfigValue(sName,sBuf,sValue);

		sprintf(sBuf,"score%d_level",nScore);
		sprintf(sValue,"%d",m_scScore[nScore].nLevel);
		m_pApp->SetConfigValue(sName,sBuf,sValue);
		

		sprintf(sBuf,"score%d_text",nScore);					
		m_pApp->SetConfigValue(sName,sBuf,m_scScore[nScore].sText);
	}
}

void CMenuMode::CheckScore(Score* m_scScore,DWORD nPoints, int idChar, BYTE nLevel)
{
	m_nChangedScore=-1;
	int nScore = 0;
	for(nScore=0;nScore<cMaxScores;nScore++)
	{	
		if(m_scScore[nScore].nPoints<nPoints)
		{

			m_nState=msChangingScore;				
			pCurrentScore=m_scScore;
			m_nChangedScore=nScore;
			m_nBlinkChar=false;
			m_nLoopsToChange=10;
			strcpy(m_sCursorText,"");
			strcpy(m_sSelectedCursor,"a");
			m_nCursorPos=2;
			m_nLoopNoImput=12;
			m_sSelectedCursor[0]=m_sPosibleChars[m_nCursorPos];
			break;
		}
	}
	if(m_nChangedScore!=-1)
	{
		for(nScore=(cMaxScores-1);nScore>m_nChangedScore;nScore--)
		{	
			m_scScore[nScore].nPoints=m_scScore[nScore-1].nPoints;
			m_scScore[nScore].nChar=m_scScore[nScore-1].nChar;
			m_scScore[nScore].nLevel=m_scScore[nScore-1].nLevel;
			
			strcpy(m_scScore[nScore].sText,
				m_scScore[nScore-1].sText);
		}
		m_scScore[m_nChangedScore].nPoints=nPoints;
		strcpy(m_scScore[m_nChangedScore].sText,"");
		m_scScore[m_nChangedScore].nChar = idChar;
		m_scScore[m_nChangedScore].nLevel=nLevel;
	}
}
