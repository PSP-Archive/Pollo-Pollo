
//include class definition
#include "CGameGraphic.h"

//include some objects declarations
#include "CMGELog.h"
#include "CPlayingMode.h"

#include <math.h>


CMGESpriteBank* CGameGraphic::m_pBank=NULL;
int CGameGraphic::m_nIdSprite[gtMax][cnMaxFrames];


void CGameGraphic::InitSprites(CMGESpriteBank* pBank)
{
	m_pBank=pBank;

	//init star graphics
	int idStar=m_pBank->GetId("star00");
	for(int nCounterStar=0;nCounterStar<cnStarMaxFrames;nCounterStar++)
		m_nIdSprite[gtStar][nCounterStar]=idStar+nCounterStar;

	m_nIdSprite[gtLigth][0]=m_pBank->GetId("light");
	
	m_nIdSprite[gtReadyLogo][0]=m_pBank->GetId("ready");
	m_nIdSprite[gtGoLogo][0]=m_pBank->GetId("go");
	
}

//init CGameGraphic
bool CGameGraphic::Init()
{	
	//finalize
	End();

	//log output
	CMGELog::SysLog(CMGELog::lInfo,"Init CGameGraphic");
	CMGELog::SysIdent();

	//base initialize
	inherited::Init();

	//are we ok?
	if(IsOk())
	{
		m_x=0;
		m_y=0;
		m_gtType=gtEmpty;
		m_ActualSprite = -1;
	}

	//log output
	Log();

	CMGELog::SysUnident();	
	

	//are we ok?
	if (IsOk())
		CMGELog::SysLog(CMGELog::lInfo,"Init CGameGraphic [Ok]");
	else
		CMGELog::SysLog(CMGELog::lInfo,"Init CGameGraphic [Fail]");	

	//return initialization status
	return IsOk();
}

//finalize object
void CGameGraphic::End()
{
	//are we ok?
	if (IsOk())
	{
		//call base finalize
		inherited::End();
	}
}

//log some info
void CGameGraphic::Log()
{	
	//log output
	CMGELog::SysLog(CMGELog::lInfo,"Logging CGameGraphic class");
	CMGELog::SysIdent();

	CMGELog::SysUnident();
}

void CGameGraphic::Set(TGraphicsType gtType, int x/*=0*/, int y/*=0*/)
{
	m_x=x;
	m_y=y;
	m_gtType=gtType;
	m_ActualSprite=0;
	m_alfa=32;
	m_nLoop=0;

	switch(m_gtType)
	{
		case gtStar:
			m_x+=2-(rand()%4);
			m_y+=2-(rand()%4);

			m_vx = 2-(rand()%4);
			m_vy = -2+(-(rand()%2));
		break;
	}
}

void CGameGraphic::Draw(int x, int y)
{
	if(m_ActualSprite!=-1)	
	{	
		switch(m_gtType)
		{
			case gtLigth:
				m_pBank->AddDraw(m_nIdSprite[m_gtType][m_ActualSprite],m_x+x,m_y+y,m_alfa);
			break;

			case gtReadyLogo:
			case gtGoLogo:
			case gtStar:

				if(m_alfa==32)
					m_pBank->Draw(m_nIdSprite[m_gtType][m_ActualSprite],m_x+x,m_y+y);
				else
					m_pBank->TransDraw(m_nIdSprite[m_gtType][m_ActualSprite],m_x+x,m_y+y,m_alfa);

			break;
		}		
	}
}

void CGameGraphic::Step()
{

	switch(m_gtType)
	{
		case gtGoLogo:
			if(m_nLoop>30)
			{
				m_alfa--;
				if(m_alfa<0)
				{
					Set(gtEmpty);

					CPlayer* m_pPlayer = (CPlayer*)m_nArgs[0];
					m_pPlayer->SetSate(CPlayer::bsPlaying);

					CPlayingMode* m_pPlaying = (CPlayingMode*)m_nArgs[1];
					m_pPlaying->SetState(CPlayingMode::psPlaying);
					
				}
			}
		break;

		case gtReadyLogo:							

			m_x+=m_vx;
			m_y+=m_vy;

			if(m_y<=((CPlayerArea::cnNumBlocksHeight*CPlayerArea::cnBlockHeight)>>1)+CPlayerArea::cBlockStartY)
			{				
				if(m_vy!=0)
				{
					m_nLoop=0;

					CPlayer* m_pPlayer = (CPlayer*)m_nArgs[0];
					if(m_pPlayer->GetSide()==0)
					{
						CPlayingMode* m_pPlaying = (CPlayingMode*)m_nArgs[1];
						m_pPlaying->ReadySound();
					}
				}

				m_vy=0;
			}

			if(m_vy==0)
			{
				if(m_nLoop>60)
				{
					m_alfa--;
				}
			}
			if(m_alfa<0)
			{
				m_alfa=32;
				m_nLoop=0;
				m_gtType=gtGoLogo;

				CPlayer* m_pPlayer = (CPlayer*)m_nArgs[0];
				if(m_pPlayer->GetSide()==0)
				{
					CPlayingMode* m_pPlaying = (CPlayingMode*)m_nArgs[1];
					m_pPlaying->GoSound();
				}
			}

		break;

		case gtStar:			
			m_x+=m_vx;
			m_y+=m_vy;

			if((m_ActualSprite%8)==0)
				m_vy++;

			if(m_ActualSprite>(cnStarMaxFrames-32))
				m_alfa--;

			m_ActualSprite++;
			if(m_ActualSprite>(cnStarMaxFrames-1))
			{
				Set(gtEmpty);
			}
		break;

		case gtLigth:			

			if(m_y>m_dy)
			{
				m_y+=m_vy;

				if(m_x!=m_dx)
					m_x+=m_vx;				
			}
			else
			{					
				m_alfa--;

				if(m_alfa<0)
				{
					Set(gtEmpty);			
					CPlayingMode* m_pPlaying = (CPlayingMode*)m_nArgs[0];
					BYTE side = (BYTE)m_nArgs[1];
					signed int send = (signed int)m_nArgs[2];
					m_pPlaying->SendBlocksFromPlayer(side,send);
				}
			}			
		break;
	}
	m_nLoop++;
}
