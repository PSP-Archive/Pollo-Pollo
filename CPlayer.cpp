
//include class definition
#include "CPlayer.h"

//include some objects declarations
#include "CMGELog.h"

#include "CPlayingMode.h"

#include "CGameGraphic.h"
#include "CPOLLOPOLLOGame.h"

CMGEFont* CPlayer::m_poFont=NULL;
CMGESpriteBank* CPlayer::m_poSprites=NULL;
WORD CPlayer::m_idWinner=0;
WORD CPlayer::m_idLooser=0;

void CPlayer::InitSprites(CMGESpriteBank* pSprites,CMGEFont* pFont)
{
	m_poFont=pFont;

	m_poSprites = pSprites;
	m_idWinner = pSprites->GetId("winner");
	m_idLooser = pSprites->GetId("looser");
}

//init CPlayer
bool CPlayer::Init(CPlayingMode* pParent, int nSide, TPlayerMode pmMode, int idChar, int nGameLevel)
{	
	//finalize
	End();

	//log output
	CMGELog::SysLog(CMGELog::lInfo,"Init CPlayer");
	CMGELog::SysIdent();

	//base initialize
	inherited::Init();

	//are we ok?
	if(IsOk())
	{
		m_idChar = idChar;

		m_pParent = pParent;

		m_pmMode = pmMode;

		m_psState = bsPreReady;

		m_nWaitNoInput=0;

		m_nPoints=0;
		m_bBlinkText = true;

		m_nAcumulatedBlocks=0;
		m_DisapiarBlocks=0;
		m_nLoopUntilDrop=0;
		
		m_pParent->GetControl()->m_cuControl.buttons[2] = PSP_CTRL_TRIANGLE;
		m_pParent->GetControl()->m_cuControl.buttons[3] = PSP_CTRL_SQUARE;
		m_pParent->GetControl()->m_cuControl.buttons[4] = PSP_CTRL_CROSS;		
		m_pParent->GetControl()->m_cuControl.buttons[5] = PSP_CTRL_CIRCLE;		

		m_pParent->GetControl()->m_cuControl.buttons[6] = PSP_CTRL_LTRIGGER;
		m_pParent->GetControl()->m_cuControl.buttons[7] = PSP_CTRL_RTRIGGER;

		m_nSide = nSide;

		m_nGameLevel = nGameLevel;

		m_nShowNextPieces=3-m_nGameLevel;
		
		CPOLLOPOLLOGame* pGame = (CPOLLOPOLLOGame*) CMGEApp::GetApp();
		CPOLLOPOLLOGame::GameLevelConfig config = pGame->GetGameLevelConfig(m_nGameLevel);
		m_nSpeed = config.m_nSpeed;
		
		m_nScrenWidth=pGame->GetRenderDevice()->GetWidth();

		if(pGame->GetPlayingMode()==CPOLLOPOLLOGame::Pollomania)
		{
			m_nSpeed=1+m_nGameLevel;
			m_bPolloManiaGame=true;
			m_nCountToNext=(m_nSpeed*10);
		}
		else
			m_bPolloManiaGame=false;

		m_bOk = m_oArea.Init(this);
		m_bOk &= m_oPiece.Init(0,0,m_nShowNextPieces);
		m_oPiece.Move((CPlayerArea::cnBlockWidth*(m_oArea.GetNumBlocksWidth()>>1)),
									-CPlayerArea::cnBlockHeight);

		m_nNextCharCount=0;
		m_bNextMoveDoChar=false;

	
		m_nLocalSpeed=0;
		m_nLoops=0;

		if(m_pmMode==pmCpu)
		{
			m_oCpuLogic.Init(&m_oArea,&m_oPiece,m_nGameLevel);
			m_oCpuLogic.Reset();
		}
	
		m_bOk = m_oChar.Init(m_pParent,m_idChar);
	}

	//log output
	Log();

	CMGELog::SysUnident();	
	

	//are we ok?
	if (IsOk())
		CMGELog::SysLog(CMGELog::lInfo,"Init CPlayer [Ok]");
	else
		CMGELog::SysLog(CMGELog::lInfo,"Init CPlayer [Fail]");	

	//return initialization status
	return IsOk();
}

//finalize object
void CPlayer::End()
{
	//are we ok?
	if (IsOk())
	{
		m_oArea.End();
		m_oPiece.End();

		//call base finalize
		inherited::End();
	}
}

//log some info
void CPlayer::Log()
{	
	//log output
	CMGELog::SysLog(CMGELog::lInfo,"Logging CPlayer class");
	CMGELog::SysIdent();

	CMGELog::SysUnident();
}


void CPlayer::Logic()
{	

	switch(m_psState)
	{
		case bsPreReady:
			m_psState = bsReady;
			
			m_oArea.AddGraphic(CGameGraphic::gtReadyLogo,
				(((m_oArea.GetNumBlocksWidth())*CPlayerArea::cnBlockWidth)>>1)
				+CPlayerArea::cBlockStartX
				,350,
				0,-4,0,0,
				(DWORD)this,
				(DWORD)m_pParent);

		break;

		case bsPlaying:
			{
				CheckControls();

				/*
				Speeds tables

				Speed 0 -> each 4 loops move 1
				Speed 1 -> each 3 loops move 1
				Speed 2 -> each 2 loops move 1
				Speed 3 -> each 1 loops move 1
				Speed 4 -> each 1 loops move 2
				Speed 5 -> each 1 loops move 3
				Speed 6 -> each 1 loops move 4
				Speed 7 -> each 1 loops move 5
				Speed 8 -> each 1 loops move 6

				*/

				int m_nLoopMatch = 4 - m_nLocalSpeed;				
				if(m_nLoopMatch<1)
					m_nLoopMatch=1;

				if((m_nLoops%m_nLoopMatch)==0)
				{
					int nToMove = (m_nLocalSpeed-3);
					if(nToMove<=0)
						nToMove=1;

					if(m_bNextMoveDoChar)
					{
						m_bNextMoveDoChar=false;
						DoCharacterStep();
					}

					for(int nMove=0;nMove<nToMove;nMove++)
					{
						m_oPiece.Move(0,1);

						if((m_oPiece.GetY()%CPlayerArea::cnBlockHeight)==0)
						{
							if ( Collide() )
							{
								m_nNextCharCount++;
								if(m_nNextCharCount>(cCharCountToPlay-1))
								{
									m_nNextCharCount=0;
									m_bNextMoveDoChar=true;
								}

								m_oPiece.End();

								
								if((m_bPolloManiaGame)&&(m_nSpeed<cMaxPlayableSpeed))
								{			
									m_nCountToNext--;
									if(m_nCountToNext==0)
									{
										m_nSpeed++;									
								
										m_nCountToNext=(m_nSpeed*10);
									}
								}

								m_oPiece.Init(0,0,m_nShowNextPieces);
								m_oPiece.Move((CPlayerArea::cnBlockWidth*(m_oArea.GetNumBlocksWidth()>>1)),
									-CPlayerArea::cnBlockHeight);

								if(!m_oArea.IsEmpty((m_oArea.GetNumBlocksWidth()>>1),0))
								{
									m_psState=bsStoped;
									m_pParent->SetLooser(GetSide());
									break;
								}

								if(m_pmMode==pmCpu)
								{								
									m_oCpuLogic.Reset();
								}

								if(m_oArea.GetTotalBlink()==0)
								{
									if(m_nAcumulatedBlocks)
										DropAcumulated();
								}
								break;
							}
						}
					}
				}
				m_oPiece.Logic();
			}
		break;
		case bsWaitingFalling:
			if (m_oArea.GetFalling()==0)
			{
				if(m_nAcumulatedBlocks)
					DropAcumulated();
				else
				{
					if((m_psState!=bsLoosing)&&(m_psState!=bsWinning))
					{
						m_psState = bsPlaying;
						m_nLoops=0;
					}
				}
			}
		break;
		case bsWaitingBlink:
			if(m_oArea.GetTotalBlink()==0)
			{
				if(m_oArea.GetFalling())
				{
					if((m_psState!=bsLoosing)&&(m_psState!=bsWinning))
						m_psState = bsWaitingFalling;
				}
				else
				{
					m_DisapiarBlocks+=m_oArea.GetTotalDisapiar();

					if((m_psState!=bsLoosing)&&(m_psState!=bsWinning))
						m_psState = bsWaitingDisapiar;
				}
			}
		break;
		case bsWaitingDisapiar:
			if(m_oArea.GetTotalDisapiar()==0)
			{				
				m_nAcumulatedBlocks -= (m_DisapiarBlocks>>2);
				if((m_nAcumulatedBlocks<0)&&(!m_bPolloManiaGame))
				{
					signed int send = -m_nAcumulatedBlocks;					
					m_nAcumulatedBlocks-=-send;

					m_oArea.AddGraphic(CGameGraphic::gtLigth,
						CPlayerArea::cnCharacterX-
						(GetSide()*
							(								
								CPlayerArea::cDistanceFromPlayer
							)
						)
						+(GetSide()*CPlayerArea::cDistanceNextPieces)
						,CPlayerArea::cnCharacterY,
						(GetSide()?-CPlayerArea::cnStartVX:CPlayerArea::cnStartVX),-CPlayerArea::cnStartVY,
						0+(GetSide()*CPlayerArea::cBlockStartX),
						CPlayerArea::cBlockStartY,
						(DWORD)m_pParent,
						(DWORD)GetSide(),
						(DWORD)send);

					//m_pParent->SendBlocksFromPlayer(GetSide(),send);
					

				}
				m_DisapiarBlocks=0;
				m_nLoops=0;

				if(m_nAcumulatedBlocks)
				{					
					DropAcumulated();					
				}
				else
				{
					if((m_psState!=bsLoosing)&&(m_psState!=bsWinning))
						m_psState = bsPlaying;
				}
			}
			if(m_oArea.GetFalling())
			{
				if((m_psState!=bsLoosing)&&(m_psState!=bsWinning))
					m_psState = bsWaitingFalling;
			}
		break;
		case bsLoosing:
		case bsWinning:
			if((m_nLoops%8)==0)
				m_bBlinkText=!m_bBlinkText;
		break;
	}

	if((m_psState!=bsLoosing)&&(m_psState!=bsWinning))
		m_nPoints+=m_oArea.Logic();	

	if(m_oArea.GetTotalBlink()>0)
	{
		if((m_psState!=bsLoosing)&&(m_psState!=bsWinning))
			m_psState=bsWaitingBlink;
	}

	m_nLoopUntilDrop--;

	m_nLoops++;
	if(m_nLoops>1000)
		m_nLoops=0;

	m_oChar.Logic();	
}

void CPlayer::Draw()
{
	if(!m_bPolloManiaGame)
		m_oPiece.DrawNextPieces(CPlayerArea::cNexPiecesStar+
			(CPlayerArea::cDistanceNextPieces*GetSide()),
			CPlayerArea::cBlockStartY);	
	else
	{
		m_oPiece.DrawNextPieces(m_nScrenWidth-CPlayerArea::cBlockStartX -
			(4*CPlayerArea::cNextPieceSeparationX),
			CPlayerArea::cBlockStartY);	
	}

	if(m_psState==bsPlaying)
		m_oPiece.Draw(CPlayerArea::cBlockStartX+
		(CPlayerArea::cDistanceFromPlayer*GetSide()),
		CPlayerArea::cBlockStartY);	

	m_oArea.Draw();
	
	/*
	switch(m_psState)
	{	
		case bsLoosing:		
			if(m_bBlinkText)
				m_poSprites->Draw(m_idLooser,CPlayerArea::cnCenter+(CPlayerArea::cDistanceFromPlayer*GetSide()),60);
				*m_poFont->Write(CPlayerArea::cBlockStartX+(CPlayerArea::cDistanceFromPlayer*GetSide()),
					60,"You Loose");

		break;

		case bsWinning:
			if(m_bBlinkText)
				m_poFont->Write(CPlayerArea::cBlockStartX+(CPlayerArea::cDistanceFromPlayer*GetSide()),
					60,"You Win");
		break;
	}
	*/

	if( (m_psState==bsLoosing) || (m_psState==bsWinning) )
	{
		int idSprite = (m_psState==bsLoosing)?m_idLooser:m_idWinner;
		m_poSprites->TransDraw(idSprite,
			(((m_oArea.GetNumBlocksWidth())*CPlayerArea::cnBlockWidth)>>1)
			+CPlayerArea::cBlockStartX+(CPlayerArea::cDistanceFromPlayer*GetSide()),
			60,
			m_bBlinkText?32:16);
	}

	DrawAcumulated(CPlayerArea::cBlockStartX+
		(CPlayerArea::cDistanceFromPlayer*GetSide()),0);	


	if(!m_bPolloManiaGame)
	{		
		m_poFont->Write(64+(CPlayerArea::cDistanceFromPlayer*GetSide()),260,
			"%11d",m_nPoints);
		m_oChar.Draw(CPlayerArea::cnCharacterX+(CPlayerArea::cDistanceNextPieces*GetSide()),
				CPlayerArea::cnCharacterY);
	}
	else
	{
		m_poFont->Write(64+CPlayerArea::cDistanceFromPlayer,260,
			"%11d",m_nPoints);

		m_oChar.Draw(m_nScrenWidth-CPlayerArea::cBlockStartX-(CPlayerArea::cnCharacterWidth>>1),
			CPlayerArea::cnCharacterY);

		m_poFont->Write(m_nScrenWidth-CPlayerArea::cBlockStartX-(CPlayerArea::cnCharacterWidth>>1)+16
			,158,"%d",m_nSpeed);

		m_poFont->Write(m_nScrenWidth-CPlayerArea::cBlockStartX-(CPlayerArea::cnCharacterWidth>>1)+16
			,177,"%d",m_nCountToNext);
		
	}
}

bool CPlayer::Collide(bool bOnlyTest/*=false*/)
{
	bool bResult = false;

	bool bCollideCenter = CollideBlock(CPiece::cCenterBlock);
	bool bCollideStick = CollideBlock(CPiece::cStickBlock);

	if(bCollideCenter||bCollideStick)
	{
		if (	(m_oPiece.GetCicle()==CPiece::tcTOP) 
			||	(m_oPiece.GetCicle()==CPiece::tcDOWN) )
		{
			bCollideCenter=true;
			bCollideStick=true;
		}
		
	}

	if((bCollideCenter)&&(!bOnlyTest))
		SetBlock(CPiece::cCenterBlock);

	if((bCollideStick)&&(!bOnlyTest))
		SetBlock(CPiece::cStickBlock);

	bResult = (bCollideCenter||bCollideStick);

	//check if we need a falling block
	if((bResult)&&(bCollideCenter!=bCollideStick)&&(!bOnlyTest))
	{
		//stick to center its the falling block		
		if(bCollideCenter)
		{
			AddFallinBlock(CPiece::cStickBlock);
		}
		//center its the falling block
		else
		{
			AddFallinBlock(CPiece::cCenterBlock);
		}
	}

	if((bResult)&&(!bOnlyTest))
		GroundBlocks(true);

	return bResult;
}

bool CPlayer::CollideBlock(BYTE block)
{
	bool bResult = false;

	CBlock* pBlock = m_oPiece.GetBlock(block);

	int width = m_oPiece.GetX()/CPlayerArea::cnBlockWidth;
	int height = m_oPiece.GetY()/CPlayerArea::cnBlockHeight;

	width += pBlock->GetX()/CPlayerArea::cnBlockWidth;
	height += pBlock->GetY()/CPlayerArea::cnBlockHeight;

	if(height>=0)
	{
		if (height==(CPlayerArea::cnNumBlocksHeight-1))
		{
			bResult=true;
		}
		else
		{
			bResult=!m_oArea.IsEmpty(width,height+1);
		}		
	}

	return bResult;
}

void CPlayer::SetBlock(BYTE block)
{
	CBlock* pBlock = m_oPiece.GetBlock(block);

	int width = m_oPiece.GetX()/CPlayerArea::cnBlockWidth;
	int height = m_oPiece.GetY()/CPlayerArea::cnBlockHeight;

	width += pBlock->GetX()/CPlayerArea::cnBlockWidth;
	height += pBlock->GetY()/CPlayerArea::cnBlockHeight;

	if(height<0)
	{
		m_psState=bsStoped;
		m_pParent->SetLooser(GetSide());
	}
	
	m_nPoints+=m_oArea.SetBlock(width,height,m_oPiece.GetBlock(block));
}

bool CPlayer::CheckControls()
{
	bool bResult=false;


	m_oPlayerResponse.left=false;
	m_oPlayerResponse.rigth=false;
	m_oPlayerResponse.up=false;
	m_oPlayerResponse.down=false;
	m_oPlayerResponse.buttons[0].wasPressed=false;
	m_oPlayerResponse.buttons[0].bePushed=false;

	switch(m_pmMode)
	{
		case pmSolo:
			m_oPlayerResponse.left=m_pParent->GetControl()->m_cuResponse.left;
			m_oPlayerResponse.rigth=m_pParent->GetControl()->m_cuResponse.rigth;
			m_oPlayerResponse.up=m_pParent->GetControl()->m_cuResponse.up;
			m_oPlayerResponse.down=m_pParent->GetControl()->m_cuResponse.down;

			m_oPlayerResponse.buttons[0].wasPressed=m_pParent->GetControl()->m_cuResponse.buttons[4].wasPressed;
		break;

		case pmFirst:

			#if defined (WIN_VER)
				m_oPlayerResponse.up=m_pParent->GetControl()->m_cuResponse.buttons[2].bePushed;
				m_oPlayerResponse.left=m_pParent->GetControl()->m_cuResponse.buttons[3].bePushed;
				m_oPlayerResponse.down=m_pParent->GetControl()->m_cuResponse.buttons[4].bePushed;
				m_oPlayerResponse.rigth=m_pParent->GetControl()->m_cuResponse.buttons[5].bePushed;						
			#else
				m_oPlayerResponse.left=m_pParent->GetControl()->m_cuResponse.left;
				m_oPlayerResponse.rigth=m_pParent->GetControl()->m_cuResponse.rigth;
				m_oPlayerResponse.up=m_pParent->GetControl()->m_cuResponse.up;
				m_oPlayerResponse.down=m_pParent->GetControl()->m_cuResponse.down;
			#endif

			m_oPlayerResponse.buttons[0].wasPressed=m_pParent->GetControl()->m_cuResponse.buttons[6].wasPressed;
		break;

		case pmSecond:

			#if defined (WIN_VER)
				m_oPlayerResponse.left=m_pParent->GetControl()->m_cuResponse.left;
				m_oPlayerResponse.rigth=m_pParent->GetControl()->m_cuResponse.rigth;
				m_oPlayerResponse.up=m_pParent->GetControl()->m_cuResponse.up;
				m_oPlayerResponse.down=m_pParent->GetControl()->m_cuResponse.down;
			#else
				m_oPlayerResponse.up=m_pParent->GetControl()->m_cuResponse.buttons[2].bePushed;
				m_oPlayerResponse.left=m_pParent->GetControl()->m_cuResponse.buttons[3].bePushed;
				m_oPlayerResponse.down=m_pParent->GetControl()->m_cuResponse.buttons[4].bePushed;
				m_oPlayerResponse.rigth=m_pParent->GetControl()->m_cuResponse.buttons[5].bePushed;						
			#endif

			m_oPlayerResponse.buttons[0].wasPressed=m_pParent->GetControl()->m_cuResponse.buttons[7].wasPressed;
		break;

		case pmCpu:
			m_oCpuLogic.Calculate();

			m_oPlayerResponse.left=m_oCpuLogic.GetResponse()->left;
			m_oPlayerResponse.rigth=m_oCpuLogic.GetResponse()->rigth;
			m_oPlayerResponse.up=m_oCpuLogic.GetResponse()->up;
			m_oPlayerResponse.down=m_oCpuLogic.GetResponse()->down;
			m_oPlayerResponse.buttons[0].wasPressed=m_oCpuLogic.GetResponse()->buttons[0].wasPressed;
		break;
	}

	if (m_oPlayerResponse.buttons[0].wasPressed)
	{
		if(CouldRotate())
		{
			m_oPiece.Rotate();
			m_pParent->TurnSound();
		}
	}

	if(m_nWaitNoInput==0)
	{		
		if (m_oPlayerResponse.left)
		{			
			if (CouldMoveToLeft())
			{
				m_oPiece.Move(-CPlayerArea::cnBlockWidth,0);
				m_nWaitNoInput=cWaitNoInput;
				bResult=true;
				m_pParent->TraslateSound();
			}
		}
		if (m_oPlayerResponse.rigth)
		{
			if (CouldMoveToRigth())
			{
				m_oPiece.Move(CPlayerArea::cnBlockWidth,0);
				m_nWaitNoInput=cWaitNoInput;
				bResult=true;
				m_pParent->TraslateSound();
			}
		}
	}
	else
		m_nWaitNoInput--;

	if(m_oPlayerResponse.down)
	{
		m_nLocalSpeed=cMaxSpeed;
	}
	else
		m_nLocalSpeed=m_nSpeed;

	return bResult;
}

void CPlayer::AddFallinBlock(BYTE block)
{
	CBlock* pBlock = m_oPiece.GetBlock(block);

	int width = m_oPiece.GetX()/CPlayerArea::cnBlockWidth;
	int height = m_oPiece.GetY()/CPlayerArea::cnBlockHeight;

	width += pBlock->GetX()/CPlayerArea::cnBlockWidth;
	height += pBlock->GetY()/CPlayerArea::cnBlockHeight;

	m_psState=bsWaitingFalling;

	m_oArea.AddFallinBlock(width,height,m_oPiece.GetBlock(block));
}

bool CPlayer::CouldMoveToLeft()
{
	bool bResult=true;

	int width = m_oPiece.GetX();
	int height = m_oPiece.GetY();

	int bwidth=0;
	int bheight=0;

	CBlock* pBlock = NULL;

	for(int block=0;block<CPiece::cnMaxBlocks;block++)
	{
		pBlock = m_oPiece.GetBlock(block);

		bwidth = width + pBlock->GetX();
		bheight = height + pBlock->GetY();

		if((bheight%CPlayerArea::cnBlockHeight)==0)
		{
			bwidth /= CPlayerArea::cnBlockWidth;
			bheight /= CPlayerArea::cnBlockHeight;
		}
		else
		{
			bwidth /= CPlayerArea::cnBlockWidth;
			bheight /= CPlayerArea::cnBlockHeight;

			bheight++;
		}

		if(bwidth<=0)
			bResult=false;
		else
			bResult=m_oArea.IsEmpty(bwidth-1,bheight);

		if(!bResult)
			break;
	}

	return bResult;
}
bool CPlayer::CouldMoveToRigth()
{
	bool bResult=true;

	int width = m_oPiece.GetX();
	int height = m_oPiece.GetY();

	int bwidth=0;
	int bheight=0;

	CBlock* pBlock = NULL;

	for(int block=0;block<CPiece::cnMaxBlocks;block++)
	{
		pBlock = m_oPiece.GetBlock(block);

		bwidth = width + pBlock->GetX();
		bheight = height + pBlock->GetY();

		if((bheight%CPlayerArea::cnBlockHeight)==0)
		{
			bwidth /= CPlayerArea::cnBlockWidth;
			bheight /= CPlayerArea::cnBlockHeight;
		}
		else
		{
			bwidth /= CPlayerArea::cnBlockWidth;
			bheight /= CPlayerArea::cnBlockHeight;

			bheight++;
		}

		if(bwidth>=((m_oArea.GetNumBlocksWidth())-1))
			bResult=false;
		else
			bResult=m_oArea.IsEmpty(bwidth+1,bheight);

		if(!bResult)
			break;
	}

	return bResult;
}

bool CPlayer::CouldRotate()
{
	bool bResult=true;

	int width = m_oPiece.GetX();
	int height = m_oPiece.GetY();

	int bwidth=0;
	int bheight=0;

	CBlock* pBlock = NULL;

	for(int block=0;block<CPiece::cnMaxBlocks;block++)
	{
		pBlock = m_oPiece.GetBlock(block);

		bwidth = width + pBlock->GetX();
		bheight = height + pBlock->GetY();

		if((bheight%CPlayerArea::cnBlockHeight)==0)
		{
			bwidth /= CPlayerArea::cnBlockWidth;
			bheight /= CPlayerArea::cnBlockHeight;
		}
		else
		{
			bwidth /= CPlayerArea::cnBlockWidth;
			bheight /= CPlayerArea::cnBlockHeight;

			bheight++;
		}

		if(bheight>=(CPlayerArea::cnNumBlocksHeight-1))
			bResult=false;

		if(bwidth>=((m_oArea.GetNumBlocksWidth())-1))
		{
			if(m_oPiece.GetCicle()==CPiece::tcDOWN)
				bResult=false;
		}

		if(bwidth<=0)
		{
			if(m_oPiece.GetCicle()==CPiece::tcTOP)
				bResult=false;
		}

		if(!bResult)
			break;
	}

	if(bResult)
	{
		m_oPiece.Rotate();

		bResult=!Collide(true);

		m_oPiece.Rotate();	
		m_oPiece.Rotate();
		m_oPiece.Rotate();
	}

	if(!bResult)
	{
		if( (m_oPiece.GetCicle()==CPiece::tcTOP) )
		{
			if(bwidth<((m_oArea.GetNumBlocksWidth())-1))
			{
				if(CouldMoveToRigth())
				{
					m_oPiece.Move(CPlayerArea::cnBlockWidth,0);
					bResult=CouldRotate();

					if(!bResult)
						m_oPiece.Move(-CPlayerArea::cnBlockWidth,0);
				}
			}
		}

		if( (m_oPiece.GetCicle()==CPiece::tcDOWN) )
		{
			if(bwidth>0)
			{
				if (CouldMoveToLeft())
				{
					m_oPiece.Move(-CPlayerArea::cnBlockWidth,0);
					bResult=CouldRotate();

					if(!bResult)
						m_oPiece.Move(CPlayerArea::cnBlockWidth,0);
				}
			}
		}

	}

	return bResult;
}

void CPlayer::AddBlocks(BYTE nBlocks)
{	
	if((m_psState!=bsLoosing)&&(m_psState!=bsWinning))
	{
		m_nAcumulatedBlocks+=nBlocks;

		if(m_nLoopUntilDrop<=0)
			m_nLoopUntilDrop=(CPlayerArea::cnLoopUntilDrop);
	}
}

void CPlayer::DropAcumulated()
{
	if(m_nLoopUntilDrop<0)
	{
		int m_nAcumulatedBlocksActual = m_nAcumulatedBlocks;
		m_nAcumulatedBlocks-=m_nAcumulatedBlocks;

		if((m_psState!=bsLoosing)&&(m_psState!=bsWinning))
			m_psState = bsWaitingFalling;

		CBlock m_bTempBlock;
		m_bTempBlock.Init(0,0);
		m_bTempBlock.SetType(6);

		for(int nAcumulate=0;nAcumulate<m_nAcumulatedBlocksActual;nAcumulate++)
		{
			m_oArea.AddFallinBlock(rand()%(m_oArea.GetNumBlocksWidth()),
				0,&m_bTempBlock);
		}
	}
}

void CPlayer::DrawAcumulated(int x, int y)
{
	int nLocalX = 0;

	int m_nAcumulatedBlocksActualSmall = m_nAcumulatedBlocks;
	int m_nAcumulatedBlocksActualBig = m_nAcumulatedBlocksActualSmall>>2;
	m_nAcumulatedBlocksActualSmall-=(m_nAcumulatedBlocksActualBig<<2);


	int nAcumulate=0;
	CBlock m_bTempBlock;
	m_bTempBlock.Init(x,y);
	m_bTempBlock.SetState(CBlock::bsNoEyes);

	m_bTempBlock.SetType(6);	
	for(nAcumulate=0;nAcumulate<m_nAcumulatedBlocksActualBig;nAcumulate++)
	{
		m_bTempBlock.Draw(nLocalX,0);
		nLocalX+=CPlayerArea::cnBlockWidth;
	}
	m_bTempBlock.SetType(7);
	for(nAcumulate=0;nAcumulate<m_nAcumulatedBlocksActualSmall;nAcumulate++)
	{
		m_bTempBlock.Draw(nLocalX,0);
		nLocalX+=CPlayerArea::cnBlockWidth>>1;
	}
}

void CPlayer::BingoSound(){m_pParent->BingoSound();}
void CPlayer::HitgroundSound(){m_pParent->HitgroundSound();}

void CPlayer::GroundBlocks(bool bEmotion)
{
	HitgroundSound();

	//CMGELog::SysLog(CMGELog::lAll,"player %d : relative %d total %d",GetSide(),relativeline,total);
}

void CPlayer::DoCharacterStep()
{
	if((m_psState!=bsLoosing)&&(m_psState!=bsWinning))
	{
		int total = m_oArea.GetTotalBlocks();

		int relativeline = total / (m_oArea.GetNumBlocksWidth());
		if(relativeline==3)
		{
			m_oChar.SetMode("normal");
		}
		else if((relativeline>=0)&&(relativeline<=2))
		{
			m_oChar.SetMode("winning");
		}
		else 
		{
			m_oChar.SetMode("loosing");
		}

	}
}

void CPlayer::SetDemoPlay()
{
	SetSate(CPlayer::bsPlaying);
	m_oArea.GenerateRandom();
	m_nPoints=15000+(rand()%15000);
}
