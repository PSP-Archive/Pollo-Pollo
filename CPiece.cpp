
//include class definition
#include "CPiece.h"

//include some objects declarations
#include "CMGELog.h"

#include "CPlayerArea.h"

BYTE CPiece::m_saNextPieces[cMaxNextPieces][cnMaxBlocks];

void CPiece::InitNextPieces(bool bPolloMania)
{
	int toMacth=6;
	if(bPolloMania)
		toMacth=3;
	for(int cnpieces=0;cnpieces<cMaxNextPieces;cnpieces++)
	{
		for(int cnblock=0;cnblock<cnMaxBlocks;cnblock++)
		{
			m_saNextPieces[cnpieces][cnblock]=rand()%toMacth;
		}
		if(bPolloMania)
		{
			if(cnpieces==25)
			{
				toMacth=4;
			}
			else if(cnpieces==50)
			{
				toMacth=5;
			}
			else if(cnpieces==70)
			{
				toMacth=6;
			}
		}
	}
}

//init CPiece
bool CPiece::Init(int nX,int nY,int nShowPieces)
{	
	//finalize
	End();

	//log output
	CMGELog::SysLog(CMGELog::lInfo,"Init CPiece");
	CMGELog::SysIdent();

	//base initialize
	inherited::Init();

	//are we ok?
	if(IsOk())
	{
		m_nShowPieces=nShowPieces;
        m_nX = nX;
		m_nY = nY;

		m_nBlocks[cCenterBlock].Init(0,0);
		m_nBlocks[cStickBlock].Init(0,-CPlayerArea::cnBlockHeight);		

		m_nBlocks[cCenterBlock].SetType(m_saNextPieces[m_nNextPiece][cCenterBlock]);
		m_nBlocks[cStickBlock].SetType(m_saNextPieces[m_nNextPiece][cStickBlock]);
		
		m_nNextPiece=GetNextPiece(m_nNextPiece);

		m_nBlocks[cCenterBlock].SetState(CBlock::bsCenter);
		m_nBlocks[cStickBlock].SetState(CBlock::bsNormal);

		m_pcCicle=tcTOP;
	}

	//log output
	Log();

	CMGELog::SysUnident();	
	

	//are we ok?
	if (IsOk())
		CMGELog::SysLog(CMGELog::lInfo,"Init CPiece [Ok]");
	else
		CMGELog::SysLog(CMGELog::lInfo,"Init CPiece [Fail]");	

	//return initialization status
	return IsOk();
}

//finalize object
void CPiece::End()
{
	//are we ok?
	if (IsOk())
	{
		m_nBlocks[cCenterBlock].End();
		m_nBlocks[cStickBlock].End();

		//call base finalize
		inherited::End();
	}
}

//log some info
void CPiece::Log()
{	
	//log output
	CMGELog::SysLog(CMGELog::lInfo,"Logging CPiece class");
	CMGELog::SysIdent();

	CMGELog::SysUnident();
}

void CPiece::DrawNextPieces(int xoffset, int yoffset)
{
	int nextPiece=m_nNextPiece;

	CBlock mTempBlocks[cnMaxBlocks];

	int startX;
	int startY;

	for(int coNextPieces=cNextPiecesShow-1;coNextPieces>=0;coNextPieces--)
	{
		nextPiece=GetNextPiece(m_nNextPiece,coNextPieces);

		if(m_nShowPieces>coNextPieces)
		{
			/*startX=xoffset+((CPlayerArea::cnBlockWidth>>1)*coNextPieces);
			startY=yoffset+((CPlayerArea::cnBlockHeight>>1)*coNextPieces);*/

			startX=xoffset+((CPlayerArea::cNextPieceSeparationX)*coNextPieces);
			startY=yoffset+((CPlayerArea::cNextPieceSeparationY)*coNextPieces);

			mTempBlocks[cCenterBlock].Init(0,CPlayerArea::cnBlockHeight);
			mTempBlocks[cStickBlock].Init(0,0);

			mTempBlocks[cCenterBlock].SetType(m_saNextPieces[nextPiece][cCenterBlock]);
			mTempBlocks[cStickBlock].SetType(m_saNextPieces[nextPiece][cStickBlock]);

			mTempBlocks[cCenterBlock].SetState(CBlock::bsNormal);
			mTempBlocks[cStickBlock].SetState(CBlock::bsNormal);

			mTempBlocks[cCenterBlock].Draw(startX,startY);
			mTempBlocks[cStickBlock].Draw(startX,startY);
		}

	}
}

void CPiece::Draw(int xoffset, int yoffset)
{
	m_nBlocks[cCenterBlock].Draw(m_nX+xoffset,m_nY+yoffset);
	m_nBlocks[cStickBlock].Draw(m_nX+xoffset,m_nY+yoffset);
}

void CPiece::Logic()
{
	m_nBlocks[cCenterBlock].Logic();
	m_nBlocks[cStickBlock].Logic();
}



void CPiece::Rotate()
{
	switch(m_pcCicle)
	{
		//from top to left
		case tcTOP:
			m_nBlocks[cStickBlock].Move(	-CPlayerArea::cnBlockWidth,
											+CPlayerArea::cnBlockHeight);
			m_pcCicle = tcLEFT;
		break;

		//from left to down
		case tcLEFT:
			m_nBlocks[cStickBlock].Move(	+CPlayerArea::cnBlockWidth,
											+CPlayerArea::cnBlockHeight);
			m_pcCicle = tcDOWN;
		break;

		//from down to right
		case tcDOWN:
			m_nBlocks[cStickBlock].Move(	+CPlayerArea::cnBlockWidth,
											-CPlayerArea::cnBlockHeight);
			m_pcCicle = tcRIGHT;
		break;

		//from right to top
		case tcRIGHT:
			m_nBlocks[cStickBlock].Move(	-CPlayerArea::cnBlockWidth,
											-CPlayerArea::cnBlockHeight);
			m_pcCicle = tcTOP;
		break;
	}
}
