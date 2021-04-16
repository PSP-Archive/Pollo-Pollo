
//include class definition
#include "CBlock.h"
#include "CPlayerArea.h"

//include some objects declarations
#include "CMGELog.h"

WORD CBlock::m_idBasicType[cnMaxBlocks];
WORD CBlock::m_idEyes=0;
WORD CBlock::m_idEyes2=0;
WORD CBlock::m_idEyesLeft=0;
WORD CBlock::m_idEyesRigth=0;
WORD CBlock::m_idMark=0;
WORD CBlock::m_idCross=0;
WORD CBlock::m_idEyesClosed=0;

CMGESpriteBank* CBlock::m_pSprites=NULL;

//init CBlock
bool CBlock::Init(int nX, int nY)
{	
	//finalize
	End();

	//log output
	CMGELog::SysLog(CMGELog::lInfo,"Init CBlock");
	CMGELog::SysIdent();

	//base initialize
	inherited::Init();

	//are we ok?
	if(IsOk())
	{
        m_bsState = bsEmpty;		

		m_nType = 255;
		m_nX = nX;
		m_nY = nY;

		for(int args=0;args<cnMaxArgs;args++)
		{
			m_nStateArgs[args]=0;
		}
		
		m_nLoops=0;

		//SetState(bsNormal);
	}

	//log output
	Log();

	CMGELog::SysUnident();	
	

	//are we ok?
	if (IsOk())
		CMGELog::SysLog(CMGELog::lInfo,"Init CBlock [Ok]");
	else
		CMGELog::SysLog(CMGELog::lInfo,"Init CBlock [Fail]");	

	//return initialization status
	return IsOk();
}

//finalize object
void CBlock::End()
{
	//are we ok?
	if (IsOk())
	{
		//call base finalize
		inherited::End();
	}
}

//log some info
void CBlock::Log()
{	
	//log output
	CMGELog::SysLog(CMGELog::lInfo,"Logging CBlock class");
	CMGELog::SysIdent();

	CMGELog::SysUnident();
}

void CBlock::Draw(int xoffset,int yoffset)
{
	if(m_bsState!=bsEmpty)
	{
		bool bDrawBasic = true;

		if(m_bsState==bsBlink)
		{
			bDrawBasic=!m_nStateArgs[ARG_BLINK];
		}

		if(m_bsState==bsDisapiar)
		{
			bDrawBasic=false;
		}
		
		//Draw basic block
		if(bDrawBasic)
		{
			//Draw block
			m_pSprites->Draw(m_idBasicType[m_nType],m_nX+xoffset,m_nY+yoffset);
		}


		if(m_bsState!=bsNoEyes)
		{
			//Draw eyes		
			if(m_bsState!=bsDisapiar)
			{
				m_pSprites->Draw(m_nStateArgs[ARG_EYE],m_nX+xoffset,m_nY+yoffset);
			}
			else
			{
				m_pSprites->TransDraw(m_nStateArgs[ARG_EYE],m_nX+xoffset,m_nY+yoffset,m_nStateArgs[ARG_ALFA]);
			}
		}


		switch(m_bsState)
		{		
			case bsCenter:
				//draw center mark
				m_pSprites->TransDraw(m_idMark,m_nX+xoffset,m_nY+yoffset,m_nStateArgs[ARG_ALFA]);			
			break;			
			case bsFalling:
				//draw falling marks
				//m_pSprites->TransDraw(m_idLines,m_nX+xoffset+(-3+rand()%6),
				//m_nY+yoffset-(CPlayerArea::cnBlockHeight);			
			break;
		}

	}
}

void CBlock::InitSprites(CMGESpriteBank* pBank)
{
	m_pSprites = pBank;

	char sName[255];
	for(BYTE nTypes=0;nTypes<cnMaxBlocks;nTypes++)
	{
		sprintf(sName,"block%02d",nTypes);
		m_idBasicType[nTypes] = m_pSprites->GetId(sName);
	}

	m_idEyes=m_pSprites->GetId("eyes");
	m_idEyes2=m_pSprites->GetId("eyes2");
	m_idEyesLeft=m_pSprites->GetId("eyesl");
	m_idEyesRigth=m_pSprites->GetId("eyesr");
	m_idEyesClosed=m_pSprites->GetId("eyesclosed");
	m_idMark=m_pSprites->GetId("mark");	
	m_idCross=m_pSprites->GetId("cross");	
	
}

void CBlock::Logic()
{	
	switch(m_bsState)
	{
		case bsNoEyes:
			m_nLoops++;
		break;

		case bsCenter:
			m_nLoops++;
			if(m_nLoops>m_nStateArgs[ARG_LOOP_CHANGE])
			{
				SetState(m_bsState);
			}
			m_nStateArgs[ARG_ALFA_PASS]++;
			if(m_nStateArgs[ARG_ALFA_PASS]>64)
			{
				m_nStateArgs[ARG_ALFA_PASS]=0;
			}

			//from -32,-31,-30.. 32 ..30,31,32;
			m_nStateArgs[ARG_ALFA] = m_nStateArgs[ARG_ALFA_PASS]- 32;

			//from 0,1,2.. 32 ..1,2,0;
			m_nStateArgs[ARG_ALFA]=32-abs(m_nStateArgs[ARG_ALFA]);			

		break;
		case bsNormal:
			m_nLoops++;
			if(m_nLoops>m_nStateArgs[ARG_LOOP_CHANGE])
			{
				SetState(m_bsState);
			}
		break;
		case bsFalling:
			m_nLoops++;
			if((m_nLoops%8)==0)
			{
				if(m_nStateArgs[ARG_EYE]==m_idEyes2)
					m_nStateArgs[ARG_EYE]=m_idEyes;
				else
					m_nStateArgs[ARG_EYE]=m_idEyes2;
			}			
		break;
		case bsSteped:
			m_nLoops--;
			if(m_nLoops==0)
			{
				SetState(bsNormal);
			}			
		break;
		case bsBlink:
			m_nLoops++;
			if(m_nLoops>15)
			{
				if(m_nStateArgs[ARG_BLINK])
					m_nStateArgs[ARG_BLINK]=0;
				else
					m_nStateArgs[ARG_BLINK]=1;

				m_nLoops=0;
			}		
		break;
		case bsDisapiar:
			m_nLoops++;
			
			if(m_nLoops>32)
				m_nStateArgs[ARG_ALFA]=0;
			else
			{
				if((m_nLoops%2)==0)
					m_nStateArgs[ARG_ALFA]--;
			}

		break;
	}
}


void CBlock::SetType(BYTE nType)
{
	m_nType=nType;
	SetState(m_bsState);	
}


void CBlock::SetState(TBlockStates bsState)
{
	m_bsState=bsState;

	switch(bsState)
	{
		case bsNoEyes:
			m_nStateArgs[ARG_EYE]=-1;
			m_nLoops=0;
		break;

		case bsCenter:
		case bsNormal:
			//which eye?
			if(m_nStateArgs[ARG_EYE]==m_idEyes)
				switch(rand()%3)
				{
					case 0:
						m_nStateArgs[ARG_EYE]=m_idEyesLeft;
					break;
					case 1:
						m_nStateArgs[ARG_EYE]=m_idEyesRigth;
					break;
					case 2:
						m_nStateArgs[ARG_EYE]=m_idEyesClosed;
					break;
				}															
			else
				m_nStateArgs[ARG_EYE]=m_idEyes;

			//loops until eyes change
			m_nStateArgs[ARG_LOOP_CHANGE]=15+(rand()%15);

			//if we have stooped eyes, we will wait more
			if(m_nStateArgs[ARG_EYE]==m_idEyes)
				m_nStateArgs[ARG_LOOP_CHANGE]+=25;

			m_nLoops=0;
			if(bsState==bsCenter)
				m_nStateArgs[ARG_ALFA_PASS]=0;
		break;
		case bsFalling:
			m_nLoops=0;
			m_nStateArgs[ARG_EYE]=m_idEyes2;
		break;
		case bsSteped:	
			m_nLoops=60;
			m_nStateArgs[ARG_EYE]=m_idCross;
		break;
		case bsBlink:
			m_nLoops=0;
			m_nStateArgs[ARG_BLINK]=0;
			m_nStateArgs[ARG_EYE]=m_idEyes2;
		break;
		case bsDisapiar:
			m_nLoops=0;
			m_nStateArgs[ARG_EYE]=m_idEyes2;
			m_nStateArgs[ARG_ALFA]=32;
		break;
	}
}
