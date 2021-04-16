
//include class definition
#include "CCPULogic.h"

//include some objects declarations
#include "CMGELog.h"
#include "CPOLLOPOLLOGame.h"

//init CCPULogic
bool CCPULogic::Init(CPlayerArea* pArea,CPiece* pPiece,int nGameLevel)
{	
	//finalize
	End();

	//log output
	CMGELog::SysLog(CMGELog::lInfo,"Init CCPULogic");
	CMGELog::SysIdent();

	//base initialize
	inherited::Init();

	//are we ok?
	if(IsOk())
	{
		m_nGameLevel = nGameLevel;
		m_pPiece = pPiece;
        m_pArea  = pArea;

		m_WantedRotate=CPiece::tcDOWN;
		m_WantedWidth=3;

		m_nLoops=0;				

		/*
		switch(m_nGameLevel)
		{
			case 0:
				m_nLoopsPressingDown=0;
				m_nLoopsReleasingDown=0;
			break;

			case 1:
				m_nLoopsPressingDown=20;
				m_nLoopsReleasingDown=60;
			break;

			case 2:
				m_nLoopsPressingDown=60;
				m_nLoopsReleasingDown=20;
			break;
		}*/

		CPOLLOPOLLOGame* pGame = (CPOLLOPOLLOGame*) CMGEApp::GetApp();
		CPOLLOPOLLOGame::GameLevelConfig config = pGame->GetGameLevelConfig(m_nGameLevel);
		m_nLoopsPressingDown= config.m_nCpuPressingDown;
		m_nLoopsReleasingDown= config.m_nCpuReleasingDown;

		m_nLoopsTo=m_nLoopsReleasingDown;
		m_bPressingDown=false;
	}

	//log output
	Log();

	CMGELog::SysUnident();	
	

	//are we ok?
	if (IsOk())
		CMGELog::SysLog(CMGELog::lInfo,"Init CCPULogic [Ok]");
	else
		CMGELog::SysLog(CMGELog::lInfo,"Init CCPULogic [Fail]");	

	//return initialization status
	return IsOk();
}

//finalize object
void CCPULogic::End()
{
	//are we ok?
	if (IsOk())
	{

		//call base finalize
		inherited::End();
	}
}

//log some info
void CCPULogic::Log()
{	
	//log output
	CMGELog::SysLog(CMGELog::lInfo,"Logging CCPULogic class");
	CMGELog::SysIdent();

	CMGELog::SysUnident();
}

void CCPULogic::Calculate()
{
	m_nLoops++;

	m_oResponse.left=false;
	m_oResponse.rigth=false;
	m_oResponse.up=false;
	m_oResponse.down=false;
	m_oResponse.buttons[0].wasPressed=false;
	m_oResponse.buttons[0].bePushed=false;

	bool skipMove=true;

	int height = m_pPiece->GetY();
	if((height%(CPlayerArea::cnBlockHeight>>1))==0)
	{
		skipMove=false;
	}

	height/= CPlayerArea::cnBlockHeight;

	int width = m_pPiece->GetX();
	width/= CPlayerArea::cnBlockWidth;

	if (height>=0)
	{
		if(m_pPiece->GetCicle()!=m_WantedRotate)
		{
			if(!skipMove)
				m_oResponse.buttons[0].wasPressed=true;
		}

		if(width!=m_WantedWidth)
		{
			if(m_WantedWidth>width)
			{
				if(!skipMove)
					m_oResponse.rigth=true;
			}
			else
			{
				if(!skipMove)
					m_oResponse.left=true;
			}
		}
		else
		{
			/*
			int nLoopMatch = 1;

			switch(m_nGameLevel)
			{
				case 0:
					nLoopMatch=0;
				break;

				case 1:
					nLoopMatch=4;
				break;

				case 2:
					nLoopMatch=2;
				break;
			}
			if(nLoopMatch)
				if((m_nLoops%nLoopMatch)==0)
					m_oResponse.down=true;*/

			if(m_bPressingDown)
			{
				if(m_nLoopsTo>0)
					m_nLoopsTo--;
				else
				{
					m_bPressingDown=false;
					m_nLoopsTo=m_nLoopsReleasingDown;
				}
			}
			else
			{
				if(m_nLoopsTo>0)
					m_nLoopsTo--;
				else
				{
					if(m_nLoopsPressingDown>0)
					{
						m_bPressingDown=true;
						m_nLoopsTo=m_nLoopsPressingDown;
					}
					else
						m_nLoopsTo=1;
				}
			}
			m_oResponse.down=m_bPressingDown;
		}
	}
}

void CCPULogic::CopyPlayerArea()
{
	//copy area
	int width, height;
	for(width=0;width<CPlayerArea::cnNumBlocksWidth;width++)
	{
		for(height=0;height<CPlayerArea::cnNumBlocksHeight;height++)
		{
			if(m_pArea->IsEmpty(width,height))
				m_SavedLogicArea[width][height]=255;
			else
				m_SavedLogicArea[width][height] = m_pArea->GetType(width,height);
		}
	}
}

void CCPULogic::SavedPlayerAreaToLogic(int level)
{
	if(level==0)
		memcpy(&m_LogicArea[level],&m_SavedLogicArea,
			(CPlayerArea::cnNumBlocksWidth*CPlayerArea::cnNumBlocksHeight));	
	else
		memcpy(&m_LogicArea[level],&m_LogicArea[level-1],
			(CPlayerArea::cnNumBlocksWidth*CPlayerArea::cnNumBlocksHeight));	
	
}

void CCPULogic::Reset()
{
	CopyPlayerArea();

	DoStep(0);
}

int CCPULogic::TestDropPiece(int rotation, int width, BYTE btCenter, BYTE btStick, int level)
{

	//we set to delete none
	memset(&m_ToDelete,0,
		(CPlayerArea::cnNumBlocksWidth*CPlayerArea::cnNumBlocksHeight));

	//dont need to delete
	m_bToDelete=false;

	int total=0;
	switch(rotation)
	{
		case 0:
			//tcDOWN;
			total+=TesDropBlock(width,btStick,level);			
			total+=TesDropBlock(width,btCenter,level);						
		break;
		case 1:
			//tcLEFT;
			total+=TesDropBlock(width,btCenter,level);
			total+=TesDropBlock(width-1,btStick,level);
		break;
		case 2:
			//tcRIGHT;
			total+=TesDropBlock(width,btCenter,level);
			total+=TesDropBlock(width+1,btStick,level);
		break;
		case 3:
			//tcTOP;
			total+=TesDropBlock(width,btCenter,level);				
			total+=TesDropBlock(width,btStick,level);					
		break;
	}	
	
	while(m_bToDelete!=false)
	{
		total+=DeleteMarked(level);
	}

	return total;
}

int CCPULogic::TesDropBlock(int width, BYTE type, int level)
{
	int total=0;

	if((width>=0)&&(width<(CPlayerArea::cnNumBlocksWidth)))
	{
		int height;
		for(height=CPlayerArea::cnNumBlocksHeight-1;height>=0;height--)
		{
			if(m_LogicArea[level][width][height]==255)
			{
				total+=SetBlock(width,height,type,level);			
				break;
			}
		}
	}

	return total;
}

int CCPULogic::SetBlock(int width, int height, BYTE type,int level)
{
	int total=0;

	int subtotal;

	if((height>=0)&&(type!=255))
	{		
		total+=((CPlayerArea::cnNumBlocksHeight)*10)-((CPlayerArea::cnNumBlocksHeight-height)*10);

		memset(&m_Checked,0,
			(CPlayerArea::cnNumBlocksWidth*CPlayerArea::cnNumBlocksHeight));

		m_LogicArea[level][width][height]=type;
		
		subtotal=CheckSameType(width,height,type,level);
		total+=(subtotal*1000);

		if(subtotal>3)
		{
			m_bToDelete=true;

			memset(&m_Checked,0,
				(CPlayerArea::cnNumBlocksWidth*CPlayerArea::cnNumBlocksHeight));

			MarkToDeleteSameType(width,height,type,level);


		}
	}	
	
	return total;
}

int CCPULogic::CheckSameType(int width, int height,BYTE type,int level)
{
	int total=0;

	if(type!=255)
	{
		if(m_LogicArea[level][width][height]==type)
		{
			if (!m_Checked[width][height])
			{
				m_Checked[width][height]=true;
				total=1;
			}
		}

		if(total)
		{
			if(width<(CPlayerArea::cnNumBlocksWidth-1))
				total+=CheckSameType(width+1,height,type,level);

			if(height<(CPlayerArea::cnNumBlocksHeight-1))
				total+=CheckSameType(width,height+1,type,level);

			if(width>0)
				total+=CheckSameType(width-1,height,type,level);

			if(height>0)
				total+=CheckSameType(width,height-1,type,level);	
		}
	}
	return total;
}

int CCPULogic::MarkToDeleteSameType(int width, int height,BYTE type,int level)
{
	int total=0;

	if(type!=255)
	{
		if(m_LogicArea[level][width][height]==type)
		{
			if (!m_Checked[width][height])
			{
				m_Checked[width][height]=true;
				m_ToDelete[width][height]=true;
				total=1;
			}
		}

		if(total)
		{
			if(width<(CPlayerArea::cnNumBlocksWidth-1))
				total+=MarkToDeleteSameType(width+1,height,type,level);

			if(height<(CPlayerArea::cnNumBlocksHeight-1))
				total+=MarkToDeleteSameType(width,height+1,type,level);

			if(width>0)
				total+=MarkToDeleteSameType(width-1,height,type,level);

			if(height>0)
				total+=MarkToDeleteSameType(width,height-1,type,level);	
		}
	}
	return total;
}

int CCPULogic::DeleteMarked(int level)
{
	int total=0;

	//dont need to delete more
	m_bToDelete=false;

	//area for deleting pieces
	BYTE m_LocalToDelete[CPlayerArea::cnNumBlocksWidth][CPlayerArea::cnNumBlocksHeight];

	//copy for local logic
	memcpy(m_LocalToDelete,m_ToDelete,
		(CPlayerArea::cnNumBlocksWidth*CPlayerArea::cnNumBlocksHeight));

	//we set to delete none
	memset(&m_ToDelete,0,
		(CPlayerArea::cnNumBlocksWidth*CPlayerArea::cnNumBlocksHeight));

	//go through deletion list
	for(BYTE nWidth=0;nWidth<CPlayerArea::cnNumBlocksWidth;nWidth++)
	{
		for(int nHeight=(CPlayerArea::cnNumBlocksHeight-1);nHeight>=0;nHeight--)
		{
			//delete this block
			if(m_LocalToDelete[nWidth][nHeight])
			{
				//this is deleted
				m_LocalToDelete[nWidth][nHeight]=false;

				//this is free
				m_LogicArea[level][nWidth][nHeight]=255;

				//check to fall others blocks
				if ((nHeight<=(CPlayerArea::cnNumBlocksHeight-1))&&(nHeight>0))
				{
					//move all blacks at once
					for(int nFallinHeight=nHeight-1;nFallinHeight>=0;nFallinHeight--)
					{
						m_LocalToDelete[nWidth][nFallinHeight]=m_LocalToDelete[nWidth][nFallinHeight+1];						
						m_LogicArea[level][nWidth][nFallinHeight]=m_LogicArea[level][nWidth][nFallinHeight+1];
					}
					//now set the blocks
					for(int nFallinHeight=nHeight-1;nFallinHeight>=0;nFallinHeight--)
					{
						total+=SetBlock(nWidth,nFallinHeight+1,m_LogicArea[level][nWidth][nFallinHeight+1],level);
					}					
				}
			}
		}
	}
	return total;
}

int CCPULogic::DoStep(int level)
{
	//CMGELog::SysLog(CMGELog::lAll,"DEBUG step %d", level);

	int nBasePiece = m_pPiece->GetNextPieceNum()-1;

	if(nBasePiece<0)
		nBasePiece=(CPiece::cMaxNextPieces-1);

	int nNumPiece = m_pPiece->GetNextPiece(nBasePiece,level);

	BYTE btCenter = m_pPiece->GetNextPieceBlockType(nNumPiece,CPiece::cCenterBlock);
	BYTE btStick = m_pPiece->GetNextPieceBlockType(nNumPiece,CPiece::cStickBlock);

	//[level]

	int bestPoints=0;
	int points=0;

	int width, rotation;
	for(rotation=0;rotation<4;rotation++)
	{		
		for(width=0;width<CPlayerArea::cnNumBlocksWidth;width++)
		{
			SavedPlayerAreaToLogic(level);

			//if we already reach near height, don't try this
			if( (m_LogicArea[level][width][0]!= 255) ||
				(m_LogicArea[level][width][1]!= 255)
			)
			{
				points = 0;
			}
			else
			{
				points = TestDropPiece(rotation,width,btCenter,btStick,level);

				if(level<(cnMaxTurns-2))
				{
					points+=DoStep(level+1);				
				}
			}

			if(points>=bestPoints)
			{
				bestPoints=points;				

				if(level==0)
				{
					m_WantedWidth=width;
					switch(rotation)
					{
						case 0:
							m_WantedRotate=CPiece::tcDOWN;
						break;
						case 1:
							m_WantedRotate=CPiece::tcLEFT;
							if(m_WantedWidth==0)
								m_WantedWidth++;
						break;
						case 2:
							m_WantedRotate=CPiece::tcRIGHT;
							if(m_WantedWidth==(CPlayerArea::cnNumBlocksWidth-1))
								m_WantedWidth--;
						break;
						case 3:
							m_WantedRotate=CPiece::tcTOP;
						break;
					}			
				}				
			}
		}
	}

	return bestPoints;

}
