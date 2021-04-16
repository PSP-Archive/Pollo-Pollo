
//include class definition
#include "CCharacter.h"

//include some objects declarations
#include "CMGELog.h"

#include "TinyXML/tinyxml.h"

#if defined(WIN_VER)
	#include <MMSystem.h>
#else
	#include <sys/time.h>
#endif

#include "CPlayingMode.h"
#include "CPOLLOPOLLOGame.h"

//init CCharacter
bool CCharacter::Init(CPlayingMode* pParent, int idChar)
{	
	//finalize
	End();

	//log output
	CMGELog::SysLog(CMGELog::lInfo,"Init CCharacter");
	CMGELog::SysIdent();

	//base initialize
	inherited::Init();

	//are we ok?
	if(IsOk())
	{
		CPOLLOPOLLOGame* pGame = (CPOLLOPOLLOGame*) CMGEApp::GetApp();
		if ( pGame->GetPlayersId(0) == idChar)
			m_nSide=0;
		else
			m_nSide=1;

		m_nLoopText=0;

		m_pParent = pParent;
		m_bSound = pParent->GetSound();
        m_idChar = idChar;

		m_nTotalModes=0;
		m_nActualMode=0;

		m_bOk=Load();

		if(IsOk())
		{
			int idMode = GetModeId("normal");
			if(idMode==-1)
			{
				CMGELog::SysLog(CMGELog::lError,"Error no 'normal' mode for char = %d",m_idChar);		
				
				m_bOk=false;
			}
			else
			{
				SetMode("normal");
				m_dwLastTime=GetAbsTime();
			}			
		}
	}

	//log output
	Log();

	CMGELog::SysUnident();	
	

	//are we ok?
	if (IsOk())
		CMGELog::SysLog(CMGELog::lInfo,"Init CCharacter [Ok]");
	else
		CMGELog::SysLog(CMGELog::lInfo,"Init CCharacter [Fail]");	

	//return initialization status
	return IsOk();
}

//finalize object
void CCharacter::End()
{
	//are we ok?
	if (IsOk())
	{
		m_oSounds.End();
		m_oAnims.End();
		m_oSprites.End();

		//call base finalize
		inherited::End();
	}
}

//log some info
void CCharacter::Log()
{	

	//log output
	CMGELog::SysLog(CMGELog::lInfo,"Logging CCharacter class");
	CMGELog::SysIdent();

	CMGELog::SysUnident();
}

void CCharacter::InkDrawBig(int x, int y,int alfa,BYTE r/*=255*/, BYTE g/*=255*/, BYTE b/*=255*/)
{
	m_oSprites.InkDraw(m_nIdBigChar,x,y,r,g,b,alfa);	
}

void CCharacter::DrawBig(int x, int y)
{
	m_oSprites.Draw(m_nIdBigChar,x,y);
}


void CCharacter::InkDraw(int x, int y,int alfa,BYTE r/*=255*/, BYTE g/*=255*/, BYTE b/*=255*/)
{
	m_oCurAnim.InkDraw(x,y,r,g,b,alfa);	
	DrawSentence(x,y);
}

void CCharacter::Draw(int x, int y)
{
	m_oCurAnim.Draw(x,y);	
	DrawSentence(x,y);
}

void CCharacter::DrawSentence(int x, int y)
{
	if(m_nLoopText)
	{
		int nActualAnim=m_cmModes[m_nActualMode].nActualAnim;
		char* sSentence = m_cmModes[m_nActualMode].sSentences[nActualAnim];

		if(strlen(sSentence)>0)
		{
			int posX = x - (GetWidth()>>1);
			int posY = y - (GetHeigth()>>1);
			posX+=m_ptTalk.x;
			posY+=m_ptTalk.y;

			m_pParent->DrawTextDialog(posX,posY,sSentence,m_nSide?0:1);
		}
	}
}


void CCharacter::Logic()
{
	DWORD eplapsed = GetEplapsedTime();		
	m_oCurAnim.Run(eplapsed);

	//need to change anim?
	int iDelay = m_cmModes[m_nActualMode].iDelay;
	if(iDelay)
	{
		m_iCurrentTime+=eplapsed;
		if(m_iCurrentTime>=iDelay)
		{		
			m_iCurrentTime-=iDelay;
			SetMode(m_nActualMode);
		}
	}
	if(m_nLoopText)
		m_nLoopText--;
}

bool CCharacter::Load()
{
	bool bResult = true;

	char sConfigPath[255];

	sprintf(sConfigPath,"dat/game/chars/char%d/char.xml",m_idChar);
	
	TiXmlDocument oDocument;

	//We load the file, and check Error
	if (!oDocument.LoadFile(sConfigPath)) 
	{								
		CMGELog::SysLog(CMGELog::lError,"Error loading char file = \"%s\" : %s.", 
			sConfigPath,oDocument.ErrorDesc());		

		bResult=false;		
	}
	else
	{
		char sDirectoryName[255];
		strcpy(sDirectoryName,sConfigPath);

		//try to find the last /
		char *p= strrchr (sDirectoryName, '/');

		//isn't found, cutting at the beginning (empty)
		if(p==NULL)
			p=&sDirectoryName[0];

		//end the string
		*++p = '\0';	

		TiXmlElement* pRoot = oDocument.RootElement();

		if(pRoot!=NULL)
		{	
			const char* sSpritesPath = pRoot->Attribute("sprites");
			const char* sAnimsPath = pRoot->Attribute("anims");
			const char* sSoundsPath = pRoot->Attribute("sounds");
			const char* sTalkX = pRoot->Attribute("talkx");
			const char* sTalkY = pRoot->Attribute("talky");

			m_ptTalk.x = atoi(sTalkX);
			m_ptTalk.y = atoi(sTalkY);

			if((sSpritesPath==NULL)||(sAnimsPath==NULL)||(sSoundsPath==NULL))
			{
				CMGELog::SysLog(CMGELog::lError,"Error no sprites, anims or sounds defined for char %d", 
					m_idChar,oDocument.ErrorDesc());		

				bResult=false;
			}
			else
			{
				char sPath[255];

				strcpy(sPath,sDirectoryName);
				strcat(sPath,sSoundsPath);
				bResult = m_oSounds.Init(sPath);
				if(bResult)
				{
					m_nIdName = m_oSounds.GetId("name");

					strcpy(sPath,sDirectoryName);
					strcat(sPath,sSpritesPath);

					bResult = m_oSprites.Init(sPath);
					if(bResult)
					{
						char sTemp[32];
						sprintf(sTemp,"face0%d",m_idChar);
						m_nIdBigChar = m_oSprites.GetId(sTemp);

						strcpy(sPath,sDirectoryName);
						strcat(sPath,sAnimsPath);

						bResult = m_oAnims.Init(sPath,&m_oSprites);
						if(bResult)
						{
							int idMode=-1;
							const char* sModeName=NULL;
							const char* sChange=NULL;
							int nChange=0;

							TiXmlElement* pModeNode = pRoot->FirstChildElement("mode");						

							while(pModeNode!=NULL)
							{
								sModeName = pModeNode->Attribute("name");
								sChange = pModeNode->Attribute("change");

								if(sChange!=NULL)
									nChange=atoi(sChange);
								else
									nChange=0;

								if(sModeName!=NULL)
								{
									idMode=AddMode(sModeName,nChange);
									if(idMode!=-1)
									{
										int nTotalAnims = 0;
										int idAnim = -1;

										const char* sAnimName=NULL;
										const char* sSoundName=NULL;
										const char* sSentence=NULL;

										TiXmlElement* pAnimNode = pModeNode->FirstChildElement("anim");
										while(pAnimNode!=NULL)
										{
											sAnimName = pAnimNode->Attribute("name");
											sSoundName = pAnimNode->Attribute("sound");
											sSentence = pAnimNode->Attribute("sentence");

											if(sAnimName!=NULL)
											{
												idAnim=AddAnimToMode(idMode,sAnimName,sSoundName,sSentence);
												if(idAnim!=-1)
												{
													nTotalAnims++;
												}
											}
											pAnimNode = pAnimNode->NextSiblingElement("anim");
										}
										if(!nTotalAnims)
										{
											CMGELog::SysLog(CMGELog::lError,"Error not anims defined for mode %s", 
												sModeName,oDocument.ErrorDesc());		

											bResult=false;
										}
									}
								}
								if(bResult)
									pModeNode=pModeNode->NextSiblingElement("mode");
								else
									pModeNode=NULL;
							}
							if(!m_nTotalModes)
							{
								CMGELog::SysLog(CMGELog::lError,"Error not modes defined for char %d", 
									m_idChar,oDocument.ErrorDesc());		

								bResult=false;
							}
						}
					}
				}
			}
		}
	}

	return bResult;
}

int CCharacter::AddMode(const char* sName, int nChange)
{
	int nid = -1;
	
	m_nTotalModes++;

	if(m_nTotalModes<cMaxModes)
	{
		nid = m_nTotalModes-1;

		strcpy(m_cmModes[nid].sName,sName);
		m_cmModes[nid].nTotalAnims=0;
		m_cmModes[nid].nActualAnim=0;
		if(nChange)
			m_cmModes[nid].iDelay=(int)(1000*nChange/60.0f); //Milisecs
		else
			m_cmModes[nid].iDelay=0;
	}
	else
		m_nTotalModes--;

	return nid;
}

int CCharacter::AddAnimToMode(int idMode, const char* sName, const char* sSound, const char* sSentence)
{
	int nid = -1;

	WORD idAnimInstanceId = m_oAnims.GetId(sName);
	WORD idSoundId = (sSound==NULL)?0:m_oSounds.GetId(sSound)+1;

	m_cmModes[idMode].nTotalAnims++;

	if(m_cmModes[idMode].nTotalAnims<cMaxAnimsPerMode)
	{
		nid=m_cmModes[idMode].nTotalAnims-1;
		m_cmModes[idMode].idAnims[nid]=idAnimInstanceId;		
		m_cmModes[idMode].idSounds[nid]=idSoundId;	
		if(sSentence!=NULL)
		{
			strcpy(m_cmModes[idMode].sSentences[nid],sSentence);
		}
		else
			strcpy(m_cmModes[idMode].sSentences[nid],"");
	}
	else
		m_cmModes[idMode].nTotalAnims--;

	return nid;
}

int CCharacter::GetModeId(const char* sMode)
{
	int nid=-1;

	for(int cmode=0;cmode<cMaxModes;cmode++)
	{
		if (strcmp(m_cmModes[cmode].sName,sMode)==0)
		{
			nid = cmode;
			break;
		}
	}

	return nid;
}

void CCharacter::SetMode(const char* sMode)
{
	int nid = GetModeId(sMode);

	if(nid!=-1)
	{
		SetMode(nid);
	}
}

void CCharacter::SetMode(int idMode)
{
	m_nActualMode = idMode;
	
	ChangeAnim(rand()%m_cmModes[idMode].nTotalAnims);	

	m_iCurrentTime=0;
}

void CCharacter::ChangeAnim(int nAnim)
{
	m_cmModes[m_nActualMode].nActualAnim=nAnim;
	
	int idAnimInstance = m_cmModes[m_nActualMode].idAnims[nAnim];
	m_oCurAnim.Init(m_oAnims.Get(idAnimInstance));

	int idSound = m_cmModes[m_nActualMode].idSounds[nAnim];
	
	if(idSound)
	{
		m_nLoopText=90;
		if(m_bSound)
			m_oSounds.Play(idSound-1);
	}
		
}
DWORD CCharacter::GetAbsTime()
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
} 

DWORD CCharacter::GetEplapsedTime()
{

	DWORD dwTime=GetAbsTime();

	DWORD dwEplapsed = dwTime-m_dwLastTime;

	m_dwLastTime=dwTime;

	return dwEplapsed;
}
