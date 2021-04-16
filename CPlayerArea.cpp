
//include class definition
#include "CPlayerArea.h"

//include some objects declarations
#include "CMGELog.h"

#include "CPlayer.h"
#include "CPOLLOPOLLOGame.h"

//init CPlayerArea
bool CPlayerArea::Init(CPlayer* pPlayer)
{	
	//finalize
	End();

	//log output
	CMGELog::SysLog(CMGELog::lInfo,"Init CPlayerArea");
	CMGELog::SysIdent();

	//base initialize
	inherited::Init();

	//are we ok?
	if(IsOk())
	{		
		CPOLLOPOLLOGame* pGame = (CPOLLOPOLLOGame*) CMGEApp::GetApp();

		if(pGame->GetPlayingMode()==CPOLLOPOLLOGame::Pollomania)
			m_nNumBlocksWidth=cnPolloManiaNumBlocksWidth;
		else
			m_nNumBlocksWidth=cnNumBlocksWidth;

		m_pPlayer=pPlayer;
		m_nTotalFalling=0;
		m_nTotalBlink=0;
		m_nLoops=0;

		m_nLoopsShaking=0;
		m_nShakeX=0;
		m_nShakeY=0;

		for(BYTE nWidth=0;nWidth<m_nNumBlocksWidth;nWidth++)
		{
			for(BYTE nHeight=0;nHeight<cnNumBlocksHeight;nHeight++)
			{
				m_bOk=m_oBlocks[nWidth][nHeight].Init((cnBlockWidth*nWidth),
					(cnBlockHeight*nHeight));
				
				if(!IsOk())
				{
					break;
				}
			}
		}
		for(int nFalls=0;nFalls<cnMaxFallinBlocks;nFalls++)
		{
			m_bOk=m_oBlocksFalling[nFalls].Init(0,0);

			if(!IsOk())
			{
				break;
			}
		}
		
		for(int nGraphic=0;nGraphic<cnMaxCustomGraphics;nGraphic++)
		{
			m_bOk=m_oGraphics[nGraphic].Init();

			if(!IsOk())
			{
				break;
			}
		}		

	}

	//log output
	Log();

	CMGELog::SysUnident();	
	

	//are we ok?
	if (IsOk())
		CMGELog::SysLog(CMGELog::lInfo,"Init CPlayerArea [Ok]");
	else
		CMGELog::SysLog(CMGELog::lInfo,"Init CPlayerArea [Fail]");	

	//return initialization status
	return IsOk();
}

//finalize object
void CPlayerArea::End()
{
	//are we ok?
	if (IsOk())
	{
		for(BYTE nWidth=0;nWidth<m_nNumBlocksWidth;nWidth++)
		{
			for(BYTE nHeight=0;nHeight<cnNumBlocksHeight;nHeight++)
			{
				m_oBlocks[nWidth][nHeight].End();
			}
		}

		for(int nFalls=0;nFalls<cnMaxFallinBlocks;nFalls++)
		{
			m_oBlocksFalling[nFalls].End();
		}

		for(int nGraphic=0;nGraphic<cnMaxCustomGraphics;nGraphic++)
		{
			m_oGraphics[nGraphic].End();
		}

		//call base finalize
		inherited::End();
	}
}

//log some info
void CPlayerArea::Log()
{	
	//log output
	CMGELog::SysLog(CMGELog::lInfo,"Logging CPlayerArea class");
	CMGELog::SysIdent();

	CMGELog::SysUnident();
}

void CPlayerArea::Draw()
{
	for(BYTE nWidth=0;nWidth<m_nNumBlocksWidth;nWidth++)
	{
		for(BYTE nHeight=0;nHeight<cnNumBlocksHeight;nHeight++)
		{
			m_oBlocks[nWidth][nHeight].Draw(cBlockStartX+
				(cDistanceFromPlayer*m_pPlayer->GetSide())+m_nShakeX,
				cBlockStartY+m_nShakeY);
		}
	}
	for(int nFalls=0;nFalls<cnMaxFallinBlocks;nFalls++)
	{
		m_oBlocksFalling[nFalls].Draw(cBlockStartX+
				(cDistanceFromPlayer*m_pPlayer->GetSide())+m_nShakeX,
				cBlockStartY+m_nShakeY);		
	}

	DrawGraphics(m_nShakeX,m_nShakeY);
}

int CPlayerArea::Logic()
{
	int total=0;

	for(BYTE nWidth=0;nWidth<m_nNumBlocksWidth;nWidth++)
	{
		for(BYTE nHeight=0;nHeight<cnNumBlocksHeight;nHeight++)
		{
			m_oBlocks[nWidth][nHeight].Logic();
		}
	}
	int width = 0;
	int height = 0;

	bool bDoShake = false;

	bool bSound = false;
	for(int nFalls=0;nFalls<cnMaxFallinBlocks;nFalls++)
	{
		if (!m_oBlocksFalling[nFalls].IsEmpty())
		{
			for(int nSpeed=0;nSpeed<cxFallinBlocksSpeed;nSpeed++)
			{
				width = m_oBlocksFalling[nFalls].GetX()/CPlayerArea::cnBlockWidth;
				height = m_oBlocksFalling[nFalls].GetY()/CPlayerArea::cnBlockHeight;

				if (!IsEmpty(width,height+1)||(height==(CPlayerArea::cnNumBlocksHeight-1)))
				{
					if(m_oBlocksFalling[nFalls].GetType()==6)
						bDoShake=true;				

					total+=SetBlock(width,height,&m_oBlocksFalling[nFalls]);
					m_oBlocksFalling[nFalls].SetState(CBlock::bsEmpty);
					m_nTotalFalling--;

					bSound=true;

					break;
				}			
				m_oBlocksFalling[nFalls].Move(0,1);
			}
			m_oBlocksFalling[nFalls].Logic();
		}
	}

	if(bSound)
		m_pPlayer->GroundBlocks(false);

	if(bDoShake)
		m_nLoopsShaking+=30;

	if(GetFalling()==0)
	{
		if(GetTotalBlink())
		{
			m_nLoops++;

			if(m_nLoops==60)
			{
				total+=DisapiarBlocks();
				m_nLoops=0;
			}
		}
		if(GetTotalDisapiar())
		{
			DisapiarLogic();
		}
	}

	GraphicsLogic();
	
	if(m_nLoopsShaking<=0)
	{
		m_nShakeX=0;
		m_nShakeY=0;
	}
	else
	{
		m_nLoopsShaking--;

		m_nShakeX+=3-rand()%6;
		m_nShakeY+=3-rand()%6;

		if(abs(m_nShakeX)>6)
			m_nShakeX=6*m_nShakeX/abs(m_nShakeX);		

		if(abs(m_nShakeY)>6)
			m_nShakeY=4*m_nShakeY/abs(m_nShakeY);
	}

	return total;
}

int CPlayerArea::SetBlock(int width, int height,CBlock* pBlock)
{
	int total=0;

	if((height>=0)&&(width>=0))
	{
		m_oBlocks[width][height].SetState(CBlock::bsNormal);
		m_oBlocks[width][height].SetType(pBlock->GetType());	

		total+=CheckElminations(width,height);

		if(height<(cnNumBlocksHeight-1))
		{
			if(m_oBlocks[width][height+1].GetState()==CBlock::bsNormal)
				m_oBlocks[width][height+1].SetState(CBlock::bsSteped);	
		}		
	}

	total+=((cnNumBlocksHeight)*10)-((cnNumBlocksHeight-height)*10);

	return total;
}

void CPlayerArea::AddFallinBlock(int width, int height,CBlock* pBlock)
{

	for(int nFalls=0;nFalls<cnMaxFallinBlocks;nFalls++)
	{
		if(m_oBlocksFalling[nFalls].IsEmpty())
		{						
			m_oBlocksFalling[nFalls].SetType(pBlock->GetType());
			m_oBlocksFalling[nFalls].SetX(cnBlockWidth*width);
			m_oBlocksFalling[nFalls].SetY(cnBlockHeight*height);
			m_oBlocksFalling[nFalls].SetState(CBlock::bsFalling);
			m_nTotalFalling++;
			break;
		}
	}
}

int CPlayerArea::CheckElminations(int width, int height)
{
	int total=0;

	memset(m_oChecked,0,sizeof(m_oChecked));

	int same = CheckSameType(width,height,m_oBlocks[width][height].GetType());
	if(same)
	{
		if(same>3)
		{			
			memset(m_oChecked,0,sizeof(m_oChecked));
			MarkSameType(width,height,m_oBlocks[width][height].GetType());
		}
		else
		{
			total+=(1000*total);
		}
	}
	
	return total;
}

int  CPlayerArea::CheckSameType(int width, int height,BYTE type)
{

	int total=0;
	if(m_oBlocks[width][height].GetType()==type)
	{
		if (!m_oChecked[width][height])
		{
			m_oChecked[width][height]=true;
			total=1;
		}
	}

	if(total)
	{
		if(width<(m_nNumBlocksWidth-1))
			total+=CheckSameType(width+1,height,type);

		if(height<(cnNumBlocksHeight-1))
			total+=CheckSameType(width,height+1,type);

		if(width>0)
			total+=CheckSameType(width-1,height,type);

		if(height>0)
			total+=CheckSameType(width,height-1,type);	
	}

	return total;
}

int CPlayerArea::MarkSameType(int width, int height,BYTE type)
{
	int total=0;

	if(m_oBlocks[width][height].GetType()==type)
	{
		if (!m_oChecked[width][height])
		{
			m_oChecked[width][height]=true;
			total=1;

			m_oBlocks[width][height].SetState(CBlock::bsBlink);
			m_nTotalBlink++;
		}
	}

	if(total)
	{
		if(width<(m_nNumBlocksWidth-1))
			total+=MarkSameType(width+1,height,type);

		if(height<(cnNumBlocksHeight-1))
			total+=MarkSameType(width,height+1,type);

		if(width>0)
			total+=MarkSameType(width-1,height,type);

		if(height>0)
			total+=MarkSameType(width,height-1,type);	
	}

	return total;
}

void CPlayerArea::DisapiarLogic()
{
	bool bErasing=false;

	for(BYTE nWidth=0;nWidth<m_nNumBlocksWidth;nWidth++)
	{
		for(int nHeight=(cnNumBlocksHeight-1);nHeight>=0;nHeight--)
		{
			if (m_oBlocks[nWidth][nHeight].GetState()==CBlock::bsDisapiar)
			{
				if(m_oBlocks[nWidth][nHeight].GetStateArg(CBlock::ARG_ALFA)!=0)
					return;

				m_oBlocks[nWidth][nHeight].SetType(CBlock::cnBlockEmptyType);
				m_oBlocks[nWidth][nHeight].SetState(CBlock::bsEmpty);				
				bErasing=true;
			}
			else
			{
				if(m_oBlocks[nWidth][nHeight].GetState()!=CBlock::bsEmpty)
				{
					if(bErasing)
					{
						AddFallinBlock(nWidth,nHeight,&m_oBlocks[nWidth][nHeight]);
						m_oBlocks[nWidth][nHeight].SetType(CBlock::cnBlockEmptyType);
						m_oBlocks[nWidth][nHeight].SetState(CBlock::bsEmpty);
					}
				}
			}
		}
		bErasing = false;
	}
	m_nTotalDisapiar=0;
}

int CPlayerArea::DisapiarBlocks()
{
	int total = 0;

	bool bSound = false;

	for(BYTE nWidth=0;nWidth<m_nNumBlocksWidth;nWidth++)
	{
		for(int nHeight=(cnNumBlocksHeight-1);nHeight>=0;nHeight--)
		{
			if (m_oBlocks[nWidth][nHeight].GetState()==CBlock::bsBlink)
			{
				m_oBlocks[nWidth][nHeight].SetType(CBlock::cnBlockEmptyType);
				m_oBlocks[nWidth][nHeight].SetState(CBlock::bsDisapiar);
				m_nTotalDisapiar++;

				total+=(5000);
				
				AddGraphic(CGameGraphic::gtStar,
					m_oBlocks[nWidth][nHeight].GetX()+cBlockStartX+(cnBlockWidth>>1),
					m_oBlocks[nWidth][nHeight].GetY()+cBlockStartY+(cnBlockHeight>>1));
				AddGraphic(CGameGraphic::gtStar,
					m_oBlocks[nWidth][nHeight].GetX()+cBlockStartX+(cnBlockWidth>>1),
					m_oBlocks[nWidth][nHeight].GetY()+cBlockStartY+(cnBlockHeight>>1));

				bSound=true;
			}
		}
	}

	if(bSound)
		m_pPlayer->BingoSound();
	

	m_nTotalBlink=0;

	return total;
}

void CPlayerArea::AddGraphic(CGameGraphic::TGraphicsType gtType, int x, int y,int vx/*=999*/,
							 int vy/*=999*/, int dx/*=999*/, int dy/*=999*/,
							 DWORD arg0/*=0*/,DWORD arg1/*=0*/,DWORD arg2/*=0*/,DWORD arg3/*=0*/)
{
	for(int nGraphic=0;nGraphic<cnMaxCustomGraphics;nGraphic++)
	{
		if(!(m_oGraphics[nGraphic].IsAlive()))
		{
			m_oGraphics[nGraphic].Set(gtType,x,y);

			if(vx!=999)
				m_oGraphics[nGraphic].SetVxVy(vx,vy);
			
			if(dx!=999)
				m_oGraphics[nGraphic].SetDxDy(dx,dy);

			m_oGraphics[nGraphic].SetArg(0,arg0);
			m_oGraphics[nGraphic].SetArg(1,arg1);
			m_oGraphics[nGraphic].SetArg(2,arg2);
			m_oGraphics[nGraphic].SetArg(3,arg3);
			break;
		}
	}
}

void CPlayerArea::DrawGraphics(int x,int y)
{
	for(int nGraphic=0;nGraphic<cnMaxCustomGraphics;nGraphic++)
	{
		if(m_oGraphics[nGraphic].IsAlive())
		{
			m_oGraphics[nGraphic].Draw(x+(cDistanceFromPlayer*m_pPlayer->GetSide()),y);
		}
	}
}

void CPlayerArea::GraphicsLogic()
{
	for(int nGraphic=0;nGraphic<cnMaxCustomGraphics;nGraphic++)
	{
		if(m_oGraphics[nGraphic].IsAlive())
		{
			m_oGraphics[nGraphic].Step();
		}
	}		
}

void CPlayerArea::GenerateRandom()
{
	for(BYTE nWidth=0;nWidth<m_nNumBlocksWidth;nWidth++)
	{
		for(BYTE nHeight=(cnNumBlocksHeight-1);nHeight>=(cnNumBlocksHeight-rand()%3);nHeight--)
		{
			m_oBlocks[nWidth][nHeight].SetType(rand()%5);
			m_oBlocks[nWidth][nHeight].SetState(CBlock::bsNormal);
		}
	}
}

void CPlayerArea::Clear(BYTE nBlockType)
{
	BYTE nWidth,nHeight;

	if(nBlockType!=cnClearLast)
	{
		for(nWidth=0;nWidth<m_nNumBlocksWidth;nWidth++)
		{
			for(nHeight=0;nHeight<cnNumBlocksHeight;nHeight++)
			{
				if( (!m_oBlocks[nWidth][nHeight].IsEmpty()) &&
					( (nBlockType==cnClearAll) || 
					  (m_oBlocks[nWidth][nHeight].GetType()==nBlockType) 
					)
				)
				{
					m_oBlocks[nWidth][nHeight].SetState(CBlock::bsBlink);
					m_nTotalBlink++;
				}
			}
		}
	}
	else
	{
		nHeight=cnNumBlocksHeight-1;
		for(nWidth=0;nWidth<m_nNumBlocksWidth;nWidth++)
		{
			if( !m_oBlocks[nWidth][nHeight].IsEmpty() ) 
			{
				m_oBlocks[nWidth][nHeight].SetState(CBlock::bsBlink);
				m_nTotalBlink++;
			}		
		}
	}

}
