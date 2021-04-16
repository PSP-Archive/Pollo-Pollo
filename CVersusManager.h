//////////////////////////////////////////////////////////////////////////
// Micro Game Engine - CVersusManager class
//
// Tbis class implements 
//////////////////////////////////////////////////////////////////////////
#if !defined(_CVersusManager_)
#define _CVersusManager_

//include some objects deteclarations
#include "CMGEObject.h"
#include "CMGETgaFile.h"
#include "CMGESprite.h"
#include "CMGEFont.h"
#include "CMGESpriteBank.h"
#include "CMGEMusicSample.h"

class CPlayingMode;
class CCharacter;
class CPOLLOPOLLOGame;

class CVersusManager : public CMGEObject 
{
	//this inherits from?
	typedef CMGEObject inherited;

	public:

		//base object style of constructor & destructor
		CVersusManager():m_pFirstSentence(NULL),m_pLastSentence(NULL),m_pCurrentSentence(NULL),m_pParent(NULL),m_pApp(NULL) { m_bOk=false;};
		virtual ~CVersusManager()	{End();};
		
		//init & end protocol functions
		virtual bool Init(CPlayingMode* pParent,int idLeft, int idRight);
		virtual void End();
		
		//get info into log
		void Log();

		void Logic();
		void Draw(int xoffset,int yoffset);
		
		inline bool IsReady(){return m_bReady;}
		void SkipTalk();

    protected:

		CPlayingMode* m_pParent;

		bool Load(const char* sFileName, bool bReverse=false);

		enum 
		{
			cPlayers = 2,
			cNumImages = 3,
			cNumGameLevels = 3,

			vsLeft = 0,
			vsRight = 1,

			imgVs = 0,
			imgTalk = 1,
			imgPlay = 2,
			

			tallkOffsetX = 50,
			tallkOffsetY = 110,

			
			tallkOffsetBigX = 100,
			tallkOffsetBigY = 110,

			tallkOffsetModeY = 200,
		};

		typedef struct SentenceType
		{
			int nId;
			int nDelay;
			char sTalk[255];
			SentenceType* pNext;
			SentenceType* pPrior;
		}Sentence;

		Sentence* m_pCurrentSentence;
		Sentence* m_pFirstSentence;
		Sentence* m_pLastSentence;

		int m_nTotalSentences;

		void AddSentence(int nId,int nDelay, const char* sTalk);
		int m_nDelayToNextSentence;

		enum VersusState
		{
			vsPresentation,
			vsFadeToTalking,
			vsTalking,
			vsFadeToPlay,
			vsPlaying
		};

		VersusState m_vsState;
		
		CCharacter* m_pChars[cPlayers];

		POINT m_nTalkOffest[cPlayers];

		CMGETgaFile m_oTga[cNumImages];
		CMGEMusicSample m_oMusic;

		int m_nActualImg;

		bool m_bReady;

		int m_nLoops;
		int m_nAlfa;

		int m_nScreenWidth;
		int m_nScreenHeight;

		int m_nCharWidth;
		int m_nCharHeight;

		CMGESpriteBank m_oSprites;
		int m_nIdVersus;
		int m_nIdGameLevel[cNumGameLevels];
		int m_nGameLevel;
		CPOLLOPOLLOGame* m_pApp;
};

#endif // !defined(_CVersusManager_)
