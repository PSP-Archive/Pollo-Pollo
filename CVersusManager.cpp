
//include class definition
#include "CVersusManager.h"

#include "CMGELog.h"

#include "TinyXML/tinyxml.h"

#include "CMGEApp.h"
#include "CPlayingMode.h"
#include "CPOLLOPOLLOGame.h"

//init CVersusManager
bool CVersusManager::Init(CPlayingMode* pParent,int idLeft, int idRight)
{	
	//finalize
	End();

	//log output
	CMGELog::SysLog(CMGELog::lInfo,"Init CVersusManager");
	CMGELog::SysIdent();

	//base initialize
	inherited::Init();

	//are we ok?
	if(IsOk())
	{
		m_pParent = pParent;
		m_pApp = (CPOLLOPOLLOGame*) CMGEApp::GetApp();
		
		m_nScreenWidth = m_pParent->GetRenderDevice()->GetWidth();
		m_nScreenHeight= m_pParent->GetRenderDevice()->GetHeight();

		m_pChars[vsLeft] = m_pParent->GetPlayer(vsLeft)->GetChar();
		m_pChars[vsRight] = m_pParent->GetPlayer(vsRight)->GetChar();

		m_nActualImg = imgVs;
		m_bReady = false;
		m_nLoops=0;
		m_nAlfa=32;
		m_vsState = vsPresentation;
		m_nTotalSentences=0;
		m_pFirstSentence=NULL;
		m_pLastSentence=NULL;
		m_pCurrentSentence=NULL;
		m_nDelayToNextSentence=0;

		char sFileName[255];

		CPOLLOPOLLOGame* pApp = (CPOLLOPOLLOGame*) CMGEApp::GetApp();
		if ( pApp->GetPlayingMode()==CPOLLOPOLLOGame::History)
			sprintf(sFileName,"dat/game/versus/%02d_%02d_sto.xml",idLeft,idRight);
		else
			sprintf(sFileName,"dat/game/versus/%02d_%02d.xml",idLeft,idRight);

		m_bOk = Load(sFileName);

		if(m_bOk)
		{
			m_bOk=m_oMusic.Init("dat/game/sound/loop_talk.ogg");
			if(!m_bOk)
			{
				CMGELog::SysLog(CMGELog::lError,"Error loading loop for talking");	
			}
		}

	}

	//log output
	Log();

	CMGELog::SysUnident();	
	

	//are we ok?
	if (IsOk())
		CMGELog::SysLog(CMGELog::lInfo,"Init CVersusManager [Ok]");
	else
		CMGELog::SysLog(CMGELog::lInfo,"Init CVersusManager [Fail]");	

	//return initialization status
	return IsOk();
}

//finalize object
void CVersusManager::End()
{
	//are we ok?
	if (IsOk())
	{
		Sentence* pSentence = m_pLastSentence;
		Sentence* pToDelete = NULL;
		while(pSentence!=NULL)
		{
			pToDelete = pSentence;
			pSentence = pSentence->pPrior;

			delete pToDelete;
		}
		m_pLastSentence=NULL;
		m_pFirstSentence=NULL;

		m_oSprites.End();

		for(int c=0;c>cNumImages;c++)
		{
			m_oTga[c].End();
		}

		m_oMusic.End();

		//call base finalize
		inherited::End();
	}
}

//log some info
void CVersusManager::Log()
{	
	//log output
	CMGELog::SysLog(CMGELog::lInfo,"Logging CVersusManager class");
	CMGELog::SysIdent();

	CMGELog::SysUnident();
}

bool CVersusManager::Load(const char* sFileName, bool bReverse/*=false*/)
{
	bool bResult = true;

	TiXmlDocument oDocument;

	//store file name and file path
	char sDirectoryName[255];
	strcpy(sDirectoryName,sFileName);
	char sFileToLoad[255];

	//try to find the last /
	char *p= strrchr (sDirectoryName, '/');
	
	//isn't found, cutting at the beginning (empty)
	if(p==NULL)
		p=&sDirectoryName[0];

	//end the string
	*++p = '\0';	

	//We load the file, and check Error
	if (!oDocument.LoadFile(sFileName)) 
	{								
		CMGELog::SysLog(CMGELog::lError,"Error loading versus file = \"%s\" : %s.", 
			sFileName,oDocument.ErrorDesc());		
		bResult=false;		
	}
	else
	{
		bResult=m_oTga[imgVs].Init("dat/game/versus/images/stback.tga");
		if(bResult)
		{
			bResult=m_oSprites.Init("dat/game/versus/sprites/sprites.xml");
			if(bResult)
			{
				m_nIdVersus = m_oSprites.GetId("vs");

				m_nIdGameLevel[0] = m_oSprites.GetId("easy");
				m_nIdGameLevel[1] = m_oSprites.GetId("medium");
				m_nIdGameLevel[2] = m_oSprites.GetId("hard");
				m_nGameLevel = m_pApp->GetGameLevel();

				m_nCharWidth = m_pChars[vsLeft]->GetWidth();
				m_nCharHeight = m_pChars[vsLeft]->GetHeigth();

				TiXmlElement* pRoot = oDocument.RootElement();

				if(pRoot!=NULL)
				{
					const char* psIncludeFile = pRoot->Attribute("include");

					if(psIncludeFile!=NULL)
					{
						sprintf(sFileToLoad,"dat/game/versus/%s",psIncludeFile);
						bResult = Load(sFileToLoad,true);
					}
					else
					{
						TiXmlElement* pInterface = pRoot->FirstChildElement("interface");
						if(pInterface!=NULL)
						{							
							static int num = 0;
							if(!num)
								num = (rand()%4)+1;

							sprintf(sFileToLoad,"%simages/pollo_talk_%02d.tga",sDirectoryName,num);

							bResult=m_oTga[imgTalk].Init(sFileToLoad);
							if(bResult)
							{
								if(m_pApp->GetPlayingMode()!=CPOLLOPOLLOGame::Pollomania)
								{
									sprintf(sFileToLoad,"%simages/pollo_play_%02d.tga",sDirectoryName,num);
								}
								else
								{																				
									sprintf(sFileToLoad,"%simages/pollomania_play_%02d.tga",sDirectoryName,num);		
								}

								bResult=m_oTga[imgPlay].Init(sFileToLoad);

								if(bResult)
								{
									TiXmlElement* pConversation = pRoot->FirstChildElement("conversation");

									if(pConversation!=NULL)
									{
										const char* sId=NULL;
										const char* sDelay=NULL;
										const char* sText=NULL;

										int nId=0;
										int nDelay=0;

										TiXmlElement* pSentence = pConversation->FirstChildElement("sentence");
										while(pSentence!=NULL)
										{
											sId = pSentence->Attribute("id");
											sDelay = pSentence->Attribute("delay");
											sText = pSentence->Attribute("talk");
											
											if( (sId!=NULL)&&(sDelay!=NULL)&&(sText!=NULL) )
											{
												if(strcmpi(sId,"left")==0)
													nId=vsLeft;
												else
													nId=vsRight;
												
												if(bReverse)
													nId=(nId==vsLeft)?vsRight:vsLeft;

												nDelay=atoi(sDelay);

												AddSentence(nId,nDelay,sText);

												pSentence = pSentence->NextSiblingElement("sentence");
											}
											else
											{
												CMGELog::SysLog(CMGELog::lError,"Error loading versus file = \"%s\" : wrong sentence tag.", 
													sFileName);		
												bResult=false;	
												pSentence=NULL;
											}
											
										}
										m_pCurrentSentence = m_pFirstSentence;
										if(m_pCurrentSentence!=NULL)
										{										
											m_nDelayToNextSentence = m_pCurrentSentence->nDelay;
										}
									}
									else
									{
										CMGELog::SysLog(CMGELog::lError,"Error loading versus file = \"%s\" : no conversation tag.", 
											sFileName);		
										bResult=false;	
									}
								}
								else
								{
									CMGELog::SysLog(CMGELog::lError,"Error loading play image %s",sFileToLoad);		
								}
							}
							else
							{
								CMGELog::SysLog(CMGELog::lError,"Error loading talk image %s",sFileToLoad);		
							}
							num++;
							if(num>4) num=1;
						}
						else
						{
							CMGELog::SysLog(CMGELog::lError,"Error loading versus file = \"%s\" : no interface tag.", 
								sFileName);		

							bResult=false;		
						}
					}
				}
			}
			else
			{
				CMGELog::SysLog(CMGELog::lError,"Error loading versus file = \"%s\" : No valid file.", 
					sFileName);		

				bResult=false;		
			}
		}
		else
		{
			CMGELog::SysLog(CMGELog::lError,"Error loading versus image file.");		
			bResult=false;		
		}
	}

	return bResult;
}

void CVersusManager::Draw(int xoffset,int yoffset)
{
	switch(m_vsState)
	{
		case vsPresentation:
		{

			m_oTga[m_nActualImg].ScreenCopy(xoffset,yoffset);

			if(m_pApp->GetPlayingMode()!=CPOLLOPOLLOGame::Pollomania)
			{
				m_pChars[vsLeft]->DrawBig(tallkOffsetBigX,tallkOffsetBigY);
				m_pChars[vsRight]->DrawBig(m_nScreenWidth-tallkOffsetBigX,tallkOffsetBigY);
				m_oSprites.Draw(m_nIdVersus,m_nScreenWidth>>1,m_nScreenHeight>>1);

				if(m_pApp->GetPlayingMode()!=CPOLLOPOLLOGame::TwoPlayers)
					m_oSprites.Draw(m_nIdGameLevel[m_nGameLevel],m_nScreenWidth>>1,tallkOffsetModeY);
			}
			else
			{
				m_pChars[vsLeft]->DrawBig(m_nScreenWidth>>1,m_nScreenHeight>>1);
			}
		}
		break;
		case vsFadeToTalking:
			/*m_pParent->GetRenderDevice()->GetBackBuffer()->Clear(0);*/

			m_oTga[m_nActualImg].ScreenCopy(xoffset,yoffset);

			if(m_pApp->GetPlayingMode()!=CPOLLOPOLLOGame::Pollomania)
			{
				m_pChars[vsLeft]->InkDrawBig(tallkOffsetBigX,tallkOffsetBigY,m_nAlfa,0,0,0);
				m_pChars[vsRight]->InkDrawBig(m_nScreenWidth-tallkOffsetBigX,tallkOffsetBigY,m_nAlfa,0,0,0);
				m_oSprites.InkDraw(m_nIdVersus,m_nScreenWidth>>1,m_nScreenHeight>>1,0,0,0,m_nAlfa);

				if(m_pApp->GetPlayingMode()!=CPOLLOPOLLOGame::TwoPlayers)
					m_oSprites.InkDraw(m_nIdGameLevel[m_nGameLevel],m_nScreenWidth>>1,tallkOffsetModeY,0,0,0,m_nAlfa);							
				m_oTga[imgTalk].ScreenCopyTrans(xoffset,yoffset,32-m_nAlfa);
			}
			else
			{
				m_pChars[vsLeft]->InkDrawBig(m_nScreenWidth>>1,m_nScreenHeight>>1,m_nAlfa,0,0,0);
				m_oTga[imgPlay].ScreenCopyTrans(xoffset,yoffset,32-m_nAlfa);
			}
			
		break;

		case vsTalking:		
			m_oTga[m_nActualImg].ScreenCopy(xoffset,yoffset);			

			/*m_oSprite[vsLeft].Draw(tallkOffsetX,tallkOffsetY);
			m_oSprite[vsRight].Draw(m_nScreenWidth-tallkOffsetX,tallkOffsetY);	*/
			m_pChars[vsLeft]->Draw(tallkOffsetX,tallkOffsetY);
			m_pChars[vsRight]->Draw(m_nScreenWidth-tallkOffsetX,tallkOffsetY);

			if(m_pCurrentSentence!=NULL)
			{				
				POINT talk;	
				if(m_pCurrentSentence->nId==vsRight)
					talk.x=m_nScreenWidth-tallkOffsetX;
				else
					talk.x=tallkOffsetX;

				talk.y = tallkOffsetY;

				talk.x-=(m_nCharWidth>>1);
				talk.y-=(m_nCharHeight>>1);
				
				talk.x += m_pChars[m_pCurrentSentence->nId]->GetTalkPos()->x;
				talk.y += m_pChars[m_pCurrentSentence->nId]->GetTalkPos()->y;

				m_pParent->DrawTextDialog(talk.x,talk.y,m_pCurrentSentence->sTalk,m_pCurrentSentence->nId);
			}
		break;

		case vsPlaying:	
			m_oTga[m_nActualImg].ScreenCopy(xoffset,yoffset);			
		break;

		case vsFadeToPlay:
			m_oTga[m_nActualImg].ScreenCopyInk(255,255,255,m_nAlfa,xoffset,yoffset);
			/*m_oSprite[vsLeft].InkDraw(tallkOffsetX,tallkOffsetY,255,255,255,m_nAlfa);
			m_oSprite[vsRight].InkDraw(m_nScreenWidth-tallkOffsetX,tallkOffsetY,255,255,255,m_nAlfa);	*/
			m_pChars[vsLeft]->InkDraw(tallkOffsetX,tallkOffsetY,m_nAlfa);
			m_pChars[vsRight]->InkDraw(m_nScreenWidth-tallkOffsetX,tallkOffsetY,m_nAlfa);
		break;
	}		
}

void CVersusManager::Logic()
{
	m_nLoops++;

	switch(m_vsState)
	{
		case vsTalking:			
			if(m_pCurrentSentence==NULL)
			{
				m_vsState = vsFadeToPlay;	
				m_pChars[vsRight]->SetMode("normal");
				m_pChars[vsLeft]->SetMode("normal");
			}
			else
			{
				m_nDelayToNextSentence--;
				if(m_nDelayToNextSentence<0)
				{
					if(m_pCurrentSentence!=NULL)
					{
						m_pChars[m_pCurrentSentence->nId]->SetMode("normal");
					}

					m_pCurrentSentence = m_pCurrentSentence->pNext;
					if(m_pCurrentSentence!=NULL)
					{	
						m_pChars[m_pCurrentSentence->nId]->SetMode("talking");
						m_nDelayToNextSentence = m_pCurrentSentence->nDelay;
					}
					
				}
				m_pChars[vsRight]->Logic();
				m_pChars[vsLeft]->Logic();

			}
		break;

		case vsFadeToPlay:
			m_nAlfa-=2;

			if(m_nAlfa<0)
			{
				m_nActualImg = imgPlay;
				m_vsState=vsPlaying;
				m_bReady = true;
				m_pApp->GetSoundDevice()->StopSound();
			}
		break;
		case vsPresentation:
			
			if(m_pApp->GetPlayingMode()!=CPOLLOPOLLOGame::Pollomania)
			{
				if(m_nLoops==5)
				{
					m_pChars[vsLeft]->NameSound();
				}

				if(m_nLoops==65)
				{
					m_pParent->VsSound();
				}
				if(m_nLoops==125)
				{
					m_pChars[vsRight]->NameSound();
				}
				if(m_nLoops>180)
				{
					m_nLoops=0;
					m_nAlfa=32;

					m_vsState=vsFadeToTalking;
				}
			}
			else
			{
				if(m_nLoops==5)
				{
					m_pChars[vsLeft]->NameSound();
				}
				if(m_nLoops>65)
				{
					m_nLoops=0;
					m_nAlfa=32;

					m_vsState=vsFadeToTalking;
				}
			}
		break;
		case vsFadeToTalking:
			m_nAlfa-=2;

			if(m_nAlfa<0)
			{
				m_nLoops=0;
				m_nAlfa=32;
				m_vsState=vsTalking;
				m_nActualImg = imgTalk;
				if(m_pApp->GetPlayingMode()==CPOLLOPOLLOGame::Pollomania)
				{
					m_nActualImg = imgPlay;
					m_vsState=vsPlaying;
					m_bReady = true;
				}
				else
					m_pApp->GetSoundDevice()->Play(&m_oMusic);
			}
		break;
	}
}

void CVersusManager::AddSentence(int nId,int nDelay, const char* sTalk)
{
	Sentence* pNew = new Sentence();
	memset(pNew,0,sizeof(Sentence));

	pNew->nId = nId;
	pNew->nDelay = nDelay;
	strcpy(pNew->sTalk,sTalk);

	if(m_pFirstSentence!=NULL)
	{
		m_pLastSentence->pNext = pNew;
		pNew->pPrior=m_pLastSentence;

		m_pLastSentence = pNew;
	}
	else
	{
		m_pFirstSentence = pNew;
		m_pLastSentence = pNew;
		//add extra delay to first sentence
		pNew->nDelay+=90;
	}

	m_nTotalSentences++;
}


void CVersusManager::SkipTalk()
{
	m_nActualImg = imgPlay;
	m_vsState=vsPlaying;
	m_bReady = true;

	m_pChars[vsRight]->SetMode("normal");
	m_pChars[vsLeft]->SetMode("normal");
	m_pApp->GetSoundDevice()->StopSound();
}
