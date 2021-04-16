
//include class definition
#include "CPlayingMode.h"

//include some objects declarations
#include "CMGELog.h"

#include "CPOLLOPOLLOGame.h"
#include "CBlock.h"
#include "CGameGraphic.h"

#if defined(WIN_VER)
	#include <MMSystem.h>
#else
	#include <sys/time.h>
	#include <psppower.h>
#endif

DWORD CPlayingMode::m_nSavePoints[cnMaxPlayers]={0,0};

//init CPlayingMode
bool CPlayingMode::Init(CMGEApp* pApp,int AppMode)
{	
	//finalize
	End();

	//log output
	CMGELog::SysLog(CMGELog::lInfo,"Init CPlayingMode");
	CMGELog::SysIdent();

	//base initialize
	inherited::Init(pApp,AppMode);

	//are we ok?
	if(IsOk())
	{
		m_pParent = (CPOLLOPOLLOGame*)pApp;
		m_nIdMus = -1;
	}

	//log output
	Log();

	CMGELog::SysUnident();	
	

	//are we ok?
	if (IsOk())
		CMGELog::SysLog(CMGELog::lInfo,"Init CPlayingMode [Ok]");
	else
		CMGELog::SysLog(CMGELog::lInfo,"Init CPlayingMode [Fail]");	

	//return initialization status
	return IsOk();
}

//finalize object
void CPlayingMode::End()
{
	//are we ok?
	if (IsOk())
	{
		//call base finalize
		inherited::End();
	}
}

//log some info
void CPlayingMode::Log()
{	
	//log output
	CMGELog::SysLog(CMGELog::lInfo,"Logging CPlayingMode class");
	CMGELog::SysIdent();

	CMGELog::SysUnident();
}

/*
DWORD GetAbsTime()
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
} */

//load app mode
bool CPlayingMode::Load()
{
	/*DWORD time = GetAbsTime();

	CMGELog::SysLog(CMGELog::lAll,"Profile, Before load");*/

	//m_psState=psPreReady;

	m_psState=psVersusTalk;

	m_nLoops=0;
 
	bool bResult = true;

	//configure playable control, button 0 (Exit)
	GetControl()->m_cuControl.buttons[0]=PSP_CTRL_SELECT;
	
	bResult = m_Sounds.Init("dat/game/sound/game.xml");
	if(bResult)
	{

		m_idBingo = m_Sounds.GetId("bingo");
		m_idTranslate = m_Sounds.GetId("translate");
		m_idHitground = m_Sounds.GetId("hitground");
		m_idTurn = m_Sounds.GetId("turn");
		m_idReady = m_Sounds.GetId("ready");
		m_idGo = m_Sounds.GetId("go");
		m_idKonami = m_Sounds.GetId("konami");
		m_idVs = m_Sounds.GetId("vs");

		char sBuff[255];
		static int sw = 1;	

		sprintf(sBuff,"dat/game/sound/music_%02d.ogg",sw);
		CMGEDataFile* hFile=CMGEApp::GetApp()->GetDataManager()->OpenFile(sBuff);
		if(hFile==NULL)
		{
			sw=1;
			sprintf(sBuff,"dat/game/sound/music_%02d.ogg",sw);
		}
		else
		{
			sw++;
			CMGEApp::GetApp()->GetDataManager()->CloseFile(hFile);
		}

		bResult = m_oMusic.Init(sBuff);

		if(bResult)
		{
			bResult = m_sbSpriteBlocks.Init("dat/game/sprites/blocks.xml");

			if (bResult)
			{
				m_idDemo = m_sbSpriteBlocks.GetId("demo");
				m_idBack = m_sbSpriteBlocks.GetId("back");
				m_idExit = m_sbSpriteBlocks.GetId("exit");

				bResult = m_oNumberFont.Init("dat/font/numbers/font.xml");
				m_opTextFont=m_pParent->GetFont();
				if(bResult)
					CPlayer::InitSprites(&m_sbSpriteBlocks,&m_oNumberFont);

				if (bResult)
				{
					m_bSound = m_pParent->GetSound();
					CGameGraphic::InitSprites(&m_sbSpriteBlocks);
					CBlock::InitSprites(&m_sbSpriteBlocks);
					

					idGameOver = m_sbSpriteBlocks.GetId("game_over");
					idPressStart = m_sbSpriteBlocks.GetId("press_start");
					idGameOverBack = m_sbSpriteBlocks.GetId("overback");

					char sSprite[255];
					for(int cCount=0;cCount<cnMaxCountNumber;cCount++)
					{
						sprintf(sSprite,"%02d",cCount);
						idCount[cCount] = m_sbSpriteBlocks.GetId(sSprite);
					}

					idCornerRD = m_sbSpriteBlocks.GetId("cornerRD");
					idCornerRU = m_sbSpriteBlocks.GetId("cornerRU");
					idCornerLD = m_sbSpriteBlocks.GetId("cornerLD");
					idCornerLU = m_sbSpriteBlocks.GetId("cornerLU");

					idSideU = m_sbSpriteBlocks.GetId("sideU");
					idSideD = m_sbSpriteBlocks.GetId("sideD");
					idSideL = m_sbSpriteBlocks.GetId("sideL");
					idSideR = m_sbSpriteBlocks.GetId("sideR");

					idFill = m_sbSpriteBlocks.GetId("fill");

					idPointR = m_sbSpriteBlocks.GetId("pointR");
					idPointL = m_sbSpriteBlocks.GetId("pointL");

					m_nSideWidth = m_sbSpriteBlocks.Get(idSideU)->GetWidth();
					m_nSideHeight = m_sbSpriteBlocks.Get(idSideL)->GetHeigth();
					m_nPointHeight = m_sbSpriteBlocks.Get(idPointL)->GetHeigth();					

					CPOLLOPOLLOGame::TPlayingMode pmMode = m_pParent->GetPlayingMode();
										
					int nLevel = m_pParent->GetGameLevel();
					m_bPolloManiaGame=false;
					switch(pmMode)
					{
						
						case CPOLLOPOLLOGame::Pollomania:
							bResult &= m_oPlayers[0].Init(this,0,CPlayer::pmSolo,m_pParent->GetPlayersId(0),nLevel);
							m_bPolloManiaGame=true;
						break;
						case CPOLLOPOLLOGame::History:
						case CPOLLOPOLLOGame::PlayerVsCPU:						
							bResult &= m_oPlayers[0].Init(this,0,CPlayer::pmSolo,m_pParent->GetPlayersId(0),nLevel);
							bResult &= m_oPlayers[1].Init(this,1,CPlayer::pmCpu,m_pParent->GetPlayersId(1),nLevel);
						break;

						case CPOLLOPOLLOGame::TwoPlayers:
							bResult &= m_oPlayers[0].Init(this,0,CPlayer::pmFirst,m_pParent->GetPlayersId(0),nLevel);
							bResult &= m_oPlayers[1].Init(this,1,CPlayer::pmSecond,m_pParent->GetPlayersId(1),nLevel);
						break;

						case CPOLLOPOLLOGame::CPUvsCPU:
							bResult &= m_oPlayers[0].Init(this,0,CPlayer::pmCpu,m_pParent->GetPlayersId(0),nLevel);
							bResult &= m_oPlayers[1].Init(this,1,CPlayer::pmCpu,m_pParent->GetPlayersId(1),nLevel);							
							SetState(psPlaying);
							m_oPlayers[0].SetDemoPlay();
							m_oPlayers[1].SetDemoPlay();
						break;
					}					

					CPiece::InitNextPieces(m_bPolloManiaGame);

					m_oPlayers[0].SetPoints(m_nSavePoints[0]);
					if(!m_bPolloManiaGame)
						m_oPlayers[1].SetPoints(m_nSavePoints[1]);
					
					bResult &= m_oVersus.Init(this,m_pParent->GetPlayersId(0),m_pParent->GetPlayersId(1));
					if(pmMode==CPOLLOPOLLOGame::CPUvsCPU)
						m_oVersus.SkipTalk();

					//configure playable control, button 1 (Pause)
					GetControl()->m_cuControl.buttons[1]=PSP_CTRL_START;
					GetControl()->m_cuControl.buttons[2]=PSP_CTRL_CROSS;
				}
			}
		}			
	}
	//CMGELog::SysLog(CMGELog::lAll,"Profile, After load. Miliseconds %d",GetAbsTime()-time);

	return bResult;
}

//unload app mode
void CPlayingMode::Unload()
{

	if(m_nIdMus!=-1)
	{
		m_pApp->GetSoundDevice()->Free(m_nIdMus);
		m_nIdMus=-1;
	}

	m_pApp->GetSoundDevice()->StopSound();

	m_oPlayers[0].End();
	if(!m_bPolloManiaGame)
		m_oPlayers[1].End();
	

	m_Sounds.End();

	m_sbSpriteBlocks.End();

	m_oVersus.End();

	m_oNumberFont.End();

	m_oMusic.End();	

}

//do app mode logic
void CPlayingMode::Logic()
{
	switch(m_psState)
	{
		case psPlaying:
			if(m_pParent->GetEasyWin())
			{
				SetLooser(1);
			}
			if(m_pParent->GetClearAll())
			{
				//clear all
				m_oPlayers[0].GetArea()->Clear(CPlayerArea::cnClearAll);
			}
			if(m_pParent->GetClearGhost())
			{
				//clear ghost
				m_oPlayers[0].GetArea()->Clear(6);
			}
			if(m_pParent->GetClearLast())
			{
				//clear last
				m_oPlayers[0].GetArea()->Clear(CPlayerArea::cnClearLast);
			}
			if( (m_nIdMus==-1)&&(m_pParent->GetMusic()) )
			{
				GetSoundDevice()->StopSound();
				m_nIdMus=PlaySound(&m_oMusic);				
			}

			//button 1 (Pause)
			if(GetControl()->m_cuResponse.buttons[1].wasPressed)
			{
				if(m_pParent->GetPlayingMode()!=CPOLLOPOLLOGame::CPUvsCPU)
				{
					if(m_nIdMus!=-1)
						GetSoundDevice()->Pause(m_nIdMus);

					KonamiSound();
					SetState(psPause);
					m_nOptionSelected=0;
					m_nLoopNoImput=0;
				}
				else
				{
					SetState(psExiting);
				}
			}
			if(m_pParent->GetPlayingMode()==CPOLLOPOLLOGame::CPUvsCPU)
			{
				if( (GetControl()->m_cuResponse.buttons[2].wasPressed)
					||
					(GetControl()->m_cuResponse.buttons[3].wasPressed)
					||
					(GetControl()->m_cuResponse.buttons[4].wasPressed)
				)
				{					
					SetState(psExiting);	
				}

				if(m_nLoops>60*30)
				{
					SetState(psExiting);					
				}
			}
		break;

		case psExiting:
			if(m_nNextCountAt>0)
				m_nNextCountAt--;
			else 
			{
				SetAppMode(CPOLLOPOLLOGame::LoadingMode,CPOLLOPOLLOGame::MenuMode);
			}
		break;
		case psEndGame:
			if(m_nNextCountAt>0)
				m_nNextCountAt--;
			else 
			{
				//if we end in two players, o demo, going to menu
				if( (m_pParent->GetPlayingMode()==CPOLLOPOLLOGame::TwoPlayers) ||
					(m_pParent->GetPlayingMode()==CPOLLOPOLLOGame::CPUvsCPU) )
				{
					//going to menu
					SetAppMode(CPOLLOPOLLOGame::LoadingMode,CPOLLOPOLLOGame::MenuMode);
				}
				//if we lost, in player vs cpu or history mode, going to 'continue'
				else if(m_oPlayers[0].GeState()==CPlayer::bsLoosing)
				{
					//do continue step
					SetState(psContinue);
				}		
				//we win
				else
				{
					//if we are in history
					if(m_pParent->GetPlayingMode()==CPOLLOPOLLOGame::History)
					{
						//check to go to next level
						bool bFinishGame=false;
						switch(m_pParent->GetPlayersId(0))
						{
							//pollajares
							case 1:
								switch(m_pParent->GetPlayersId(1))
								{
									// vs pollobot
									case 2:
										//vs pollozila
										m_pParent->SetPlayersId(1,5);
										m_pParent->SetHistoryGameLevel(1);
									break;
									// vs pollozila
									case 5:
										//vs arquiño
										m_pParent->SetPlayersId(1,8);
										m_pParent->SetHistoryGameLevel(2);
									break;
									// vs arquiño
									case 8:
										//end game
										bFinishGame=true;
										m_pParent->SetConfigValue("characters","char8","normal");
									break;
								}
							break;
							//pollodrino
							case 3:
								switch(m_pParent->GetPlayersId(1))
								{
									// vs pollajares
									case 1:
										//vs pollola
										m_pParent->SetPlayersId(1,6);
										m_pParent->SetHistoryGameLevel(1);
									break;
									// vs pollola
									case 6:
										//vs pacopepe
										m_pParent->SetPlayersId(1,4);
										m_pParent->SetHistoryGameLevel(2);
									break;
									// vs pacopepe
									case 4:
										//end game
										bFinishGame=true;
										m_pParent->SetConfigValue("characters","char4","normal");
									break;
								}
							break;
							//pollola
							case 6:
								switch(m_pParent->GetPlayersId(1))
								{
									// vs pollozila
									case 5:
										//vs pollobot
										m_pParent->SetPlayersId(1,2);
										m_pParent->SetHistoryGameLevel(1);
									break;
									// vs pollobot
									case 2:
										//vs diana
										m_pParent->SetPlayersId(1,7);
										m_pParent->SetHistoryGameLevel(2);
									break;
									// vs diana
									case 7:
										//end game
										bFinishGame=true;
										m_pParent->SetConfigValue("characters","char7","normal");										
									break;
								}
							break;
						}
						//next level or game ending
						if(bFinishGame)
						{
							m_pParent->SetConfigValue("game","pollomania","unlocked");
							SetAppMode(CPOLLOPOLLOGame::LoadingMode,CPOLLOPOLLOGame::EndAppMode);							
						}
						else
						{
							//SetAppMode(CPOLLOPOLLOGame::LoadingMode,CPOLLOPOLLOGame::PlayingMode);
							SetAppMode(CPOLLOPOLLOGame::CongratsMode,CPOLLOPOLLOGame::PlayingMode);
						}
						m_nSavePoints[0]=m_oPlayers[0].GetPoints();
					}
					//we win in player vs cpu
					else
					{
						//going to menu
						//SetAppMode(CPOLLOPOLLOGame::LoadingMode,CPOLLOPOLLOGame::MenuMode);
						SetAppMode(CPOLLOPOLLOGame::CongratsMode,CPOLLOPOLLOGame::MenuMode);
					}
				}
			}
		break;

		case psContinue:	
		{
			//if we left to count to next number...
			if(m_nNextCountAt>0)
				m_nNextCountAt--;
			//discount one number
			else
			{
				//next count at
				m_nNextCountAt=cnCountRate;
			
				//discount & check if whe finishing count
				nCount++;
				if(nCount>(cnMaxCountNumber-1))	
				{
					//going to menu
					nCount=cnMaxCountNumber-1;
					SetAppMode(CPOLLOPOLLOGame::LoadingMode,CPOLLOPOLLOGame::MenuMode);
				}
			}

			//button 1 (Pause)
			if(GetControl()->m_cuResponse.buttons[1].wasPressed)
			{
				SetAppMode(CPOLLOPOLLOGame::LoadingMode,CPOLLOPOLLOGame::PlayingMode);			
			}
			//button 2 (cross) : quick discount pressing
			if(GetControl()->m_cuResponse.buttons[2].wasPressed)
			{
				//discount & check if whe finishing count
				nCount+=2;
				if(nCount>(cnMaxCountNumber-1))
				{
					//going to menu
					nCount=cnMaxCountNumber-1;
					SetAppMode(CPOLLOPOLLOGame::LoadingMode,CPOLLOPOLLOGame::MenuMode);
				}
			}
		}
		break;

		case psPause:
			//button 0 (Pause)
			if(GetControl()->m_cuResponse.buttons[1].wasPressed)
			{
				KonamiSound();

				if(m_nIdMus!=-1)
					GetSoundDevice()->Pause(m_nIdMus);

				SetState(psPlaying);
			}

			if(GetControl()->m_cuResponse.buttons[2].wasPressed)
			{
				if(m_nOptionSelected==0)
				{
					KonamiSound();

					if(m_nIdMus!=-1)
						GetSoundDevice()->Pause(m_nIdMus);

					SetState(psPlaying);
				}
				else
				{				
					SetState(psExiting);
				}
			}
			
			if(m_nLoopNoImput==0)
			{
				bool bPress=false;

				if((GetControl()->m_cuResponse.up)||(GetControl()->m_cuResponse.left))
				{
					m_nOptionSelected--;
					bPress=true;
				}

				if((GetControl()->m_cuResponse.down)||(GetControl()->m_cuResponse.rigth))
				{
					m_nOptionSelected++;
					bPress=true;
				}

				if(bPress)
				{
					m_nLoopNoImput=12;
					
					TraslateSound();

					if(m_nOptionSelected<0)
						m_nOptionSelected=1;
					if(m_nOptionSelected>1)
						m_nOptionSelected=0;
				}
			}
			else
			{
				m_nLoopNoImput--;
			}

		break;
	}
	//if we aren't in versus talking mode
	if(m_psState!=psVersusTalk)
	{
		if((m_psState!=psPause)&&(m_psState!=psExiting))
		{
			m_oPlayers[0].Logic();
			if(!m_bPolloManiaGame)
				m_oPlayers[1].Logic();		
		}
	}
	else
	{
		if(!GetControl()->m_cuResponse.buttons[1].wasPressed)
		{
			m_oVersus.Logic();
	
			if(m_oVersus.IsReady())
			{
				m_psState = psPreReady;
			}
		}
		else
		{
			m_oVersus.SkipTalk();
			m_psState = psPreReady;
		}
	}

	m_nLoops++;

	if(m_pParent->GetPlayingMode()==CPOLLOPOLLOGame::CPUvsCPU)
	{
		#if defined(PSP_VER)
			//cause PSP to not be off
			scePowerTick(0);
		#endif
	}

}

//draw app mode
void CPlayingMode::Draw()
{
	bool bDoContinue = false;
	if(m_psState == psContinue)
	{
		bDoContinue = true;
	}

	//erase background	
	if (!bDoContinue)
	{
		m_oVersus.Draw(0,0);

		if(m_psState != psVersusTalk)
		{			
			m_oPlayers[0].Draw();
			if(!m_bPolloManiaGame)
				m_oPlayers[1].Draw();
		}
	}
	else
	{
		GetRenderDevice()->GetBackBuffer()->Clear();
	}

	switch(m_psState)
	{
		case psPause:
			m_sbSpriteBlocks.TransDraw(m_idBack,GetRenderDevice()->GetWidth()>>1,110,(m_nOptionSelected==0)?32:16);
			m_sbSpriteBlocks.TransDraw(m_idExit,GetRenderDevice()->GetWidth()>>1,150,(m_nOptionSelected==1)?32:16);

		break;

		case psExiting:
		break;
		case psEndGame:
		break;

		case psContinue:

			if( bDoContinue )
			{
				m_sbSpriteBlocks.Draw(idGameOverBack,0,0);
				m_sbSpriteBlocks.Draw(idGameOver,28,26);			
				m_sbSpriteBlocks.Draw(idPressStart,GetRenderDevice()->GetWidth()>>1,90);			
				m_sbSpriteBlocks.Draw(idCount[nCount],GetRenderDevice()->GetWidth()>>1,190);			
			}
		break;
	}

	if(m_pParent->GetPlayingMode()==CPOLLOPOLLOGame::CPUvsCPU)
	{
		static int nLoop = 0;

		//from -32,-31,-30.. 32 ..30,31,32;
		signed int iAlfa = nLoop - 32;

		//from 0,1,2.. 32 ..1,2,0;
		iAlfa=32-abs(iAlfa);

		m_sbSpriteBlocks.TransDraw(m_idDemo,GetRenderDevice()->GetWidth()>>1,
				GetRenderDevice()->GetHeight()>>1,iAlfa);

		nLoop++;
		if(nLoop>64)
		{
			nLoop=0;
		}

	}	
}

void CPlayingMode::SetLooser(BYTE nPlayer)
{	
	BYTE nWinner = (nPlayer)?0:1;
	BYTE nLooser = (nPlayer)?1:0;

	DWORD nPoints = m_oPlayers[nWinner].GetPoints();
	m_oPlayers[nWinner].SetPoints(nPoints+(50000*(m_pParent->GetGameLevel()+1)));

	switch(m_pParent->GetPlayingMode())
	{
		case CPOLLOPOLLOGame::History:
		case CPOLLOPOLLOGame::PlayerVsCPU:
		case CPOLLOPOLLOGame::Pollomania:
			m_oPlayers[0].SetSate((nWinner==0)?CPlayer::bsWinning:CPlayer::bsLoosing);
			m_oPlayers[1].SetSate((nWinner==1)?CPlayer::bsWinningNoSound:CPlayer::bsLoosingNoSound);
		break;

		case CPOLLOPOLLOGame::TwoPlayers:
		case CPOLLOPOLLOGame::CPUvsCPU:
			m_oPlayers[nWinner].SetSate(CPlayer::bsWinning);
			m_oPlayers[nLooser].SetSate(CPlayer::bsLoosingNoSound);
		break;
		
	}

	SetState(psEndGame);	
}

void CPlayingMode::SetState(TPlayingState psState)
{	
	if(m_psState!=psState)
	{
		m_psState=psState;

		switch(m_psState)
		{
			case psExiting:
				m_nLoops=0;
				nCount=0;
				m_nNextCountAt=60;
			break;

			case psEndGame:
				m_nLoops=0;
				nCount=0;
				m_nNextCountAt=120;
			break;

			case psContinue:
				m_nLoops=0;
				nCount=0;
				m_nNextCountAt=cnCountRate;
			break;
		}
	}
}

void CPlayingMode::SendBlocksFromPlayer(BYTE nPlayer,BYTE nBlocks)
{
	BYTE nOtherPlayer = (nPlayer)?0:1;
	m_oPlayers[nOtherPlayer].AddBlocks(nBlocks);
}

void CPlayingMode::DrawTextDialog(int x, int y, const char *sText, int nSide)
{
	POINT talk;
	POINT ext = m_opTextFont->GetExtPoint(sText);
	
	talk.x = x;
	
	talk.y = y+m_nPointHeight;

	if(nSide==vsRight)
	{
		talk.x-=(ext.x-m_nSideWidth);
	}
	else
	{
		talk.x-=m_nSideWidth;
	}
	
	DrawDialog(talk.x,talk.y,ext.x,ext.y,nSide);				

	m_opTextFont->Write(talk.x,talk.y,sText);
}

void CPlayingMode::DrawDialog(int x, int y, int width, int height, int nSide)
{
	int contWidth=0;
	int contHeight=0;

	int limitX = (x+width);
	int limitY = (y+height);

	for(contWidth=x;contWidth<limitX;contWidth+=m_nSideWidth)
		m_sbSpriteBlocks.Draw(idSideU,contWidth,y);	
	
	for(contHeight=y;contHeight<limitY;contHeight+=m_nSideHeight)
		m_sbSpriteBlocks.Draw(idSideL,x,contHeight);

	for(contWidth=x;contWidth<limitX;contWidth+=m_nSideWidth)
		m_sbSpriteBlocks.Draw(idSideD,contWidth,contHeight);

	for(contHeight=y;contHeight<limitY;contHeight+=m_nSideHeight)
		m_sbSpriteBlocks.Draw(idSideR,contWidth,contHeight);

	for(contWidth=x;contWidth<limitX;contWidth+=m_nSideWidth)
		for(contHeight=y;contHeight<limitY;contHeight+=m_nSideHeight)
			m_sbSpriteBlocks.Draw(idFill,contWidth,contHeight);

	if(nSide==vsLeft)
		m_sbSpriteBlocks.Draw(idPointL,x+m_nSideWidth,y-m_nSideHeight);
	else
		m_sbSpriteBlocks.Draw(idPointR,contWidth-m_nSideWidth,y-m_nSideHeight);

	m_sbSpriteBlocks.Draw(idCornerLU,x,y);
	m_sbSpriteBlocks.Draw(idCornerLD,x,contHeight);
	m_sbSpriteBlocks.Draw(idCornerRU,contWidth,y);
	m_sbSpriteBlocks.Draw(idCornerRD,contWidth,contHeight);	

}

void CPlayingMode::SetAppMode(int num,int next/*=-1*/)
{
	int toNum = num;
	int toNext = next;

	CPOLLOPOLLOGame::TPlayingMode pmMode = m_pParent->GetPlayingMode();

	/*if(toNext==CPOLLOPOLLOGame::PlayingMode)
	{
		switch(pmMode)
		{
			case CPOLLOPOLLOGame::History:				
				if(m_oPlayers[0].GeState()==CPlayer::bsWinning)
					m_nSavePoints[0]=m_oPlayers[0].GetPoints();
				else
				{
					m_nSavePoints[0]=0;
					m_nSavePoints[1]=0;
				}
			break;

			default:
				m_nSavePoints[0]=0;
				m_nSavePoints[1]=0;
			break;
		}
	}
	else */if((toNext==CPOLLOPOLLOGame::MenuMode)||(toNext==CPOLLOPOLLOGame::EndAppMode))
	{
		for(int nPlayer=0;nPlayer<cnMaxPlayers;nPlayer++)
		{
			m_nSavePoints[nPlayer]=0;
			m_pParent->SetPosibleRecord(nPlayer,m_oPlayers[nPlayer].GetPoints());
			m_pParent->SetPosibleRecordId(nPlayer,m_pParent->GetPlayersId(nPlayer));
		}
		
		m_pParent->SetPosibleRecordMode(pmMode);
		if(pmMode!=CPOLLOPOLLOGame::Pollomania)
			m_pParent->SetPosibleRecordLevel(m_pParent->GetLevel());
		else
			m_pParent->SetPosibleRecordLevel(m_oPlayers[0].GetSpeed());
	}

	inherited::SetAppMode(toNum,toNext);
}
