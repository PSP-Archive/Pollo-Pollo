//////////////////////////////////////////////////////////////////////////
// Micro Game Engine - CCharacter class
//
// Tbis class implements 
//////////////////////////////////////////////////////////////////////////
#if !defined(_CCharacter_)
#define _CCharacter_

//include some objects deteclarations
#include "CMGEObject.h"

#include "CMGESoundBank.h"
#include "CMGESpriteBank.h"
#include "CMGEAnimBank.h"
#include "CMGEAnimInstance.h"
class CPlayingMode;
class CCharacter : public CMGEObject 
{
	//this inherits from?
	typedef CMGEObject inherited;

	public:

		//base object style of constructor & destructor
		CCharacter():m_pParent(NULL)			{ m_bOk=false;};
		virtual ~CCharacter()	{End();};
		
		//init & end protocol functions
		virtual bool Init(CPlayingMode* pParent,int idChar);
		virtual void End();
		
		//get info into log
		void Log();

		void DrawSentence(int x, int y);
		void Draw(int x, int y);
		void InkDraw(int x, int y,int alfa,BYTE r=255, BYTE g=255, BYTE b=255);

		void DrawBig(int x, int y);
		void InkDrawBig(int x, int y,int alfa,BYTE r=255, BYTE g=255, BYTE b=255);

		void Logic();

		bool Load();

		void SetMode(const char* sMode);
		inline WORD GetWidth(){return m_oSprites.Get(0)->GetWidth();};
		inline WORD GetHeigth(){return m_oSprites.Get(0)->GetHeigth();};

		inline POINT* GetTalkPos(){return &m_ptTalk;};

		inline void NameSound(){if(m_bSound) m_oSounds.Play(m_nIdName);}

    protected:
		int m_nLoopText;
		bool m_bSound;
		void SetMode(int idMode);

		CMGESoundBank m_oSounds;
		CMGESpriteBank m_oSprites;
		CMGEAnimBank m_oAnims;
		CMGEAnimInstance m_oCurAnim;

		WORD m_idChar;

		enum
		{
			cMaxModes			= 10,
			cMaxAnimsPerMode	= 10
		};

		struct TCharMode
		{
			char sName[255];
			char sSentences[cMaxAnimsPerMode][80];
			WORD idAnims[cMaxAnimsPerMode];
			WORD idSounds[cMaxAnimsPerMode];
			int	nTotalAnims;
			int nActualAnim;
			int iDelay;
		};

		TCharMode m_cmModes[cMaxModes];

		WORD m_nTotalModes;
		WORD m_nActualMode;

		int AddMode(const char* sName, int nChange);
		int AddAnimToMode(int idMode, const char* sName, const char* sSound, const char* sSentence);

		int GetModeId(const char* sMode);		
		void ChangeAnim(int nAnim);

		DWORD GetAbsTime();
		DWORD GetEplapsedTime();
		DWORD m_dwLastTime;

		POINT m_ptTalk;

		//time for change anim in mode
		int m_iCurrentTime;

		CPlayingMode* m_pParent;		
		int m_nSide;

		int m_nIdBigChar;
		int m_nIdName;
};

#endif // !defined(_CCharacter_)
