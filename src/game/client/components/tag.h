#ifndef GAME_CLIENT_COMPONENTS_TAG_H
#define GAME_CLIENT_COMPONENTS_TAG_H

#include <vector>

#include <game/client/component.h>
#define MAX_LABELS 16
class CTag
{
	private:
		bool m_Active;
		unsigned m_Color;
		const char *m_pLabels[MAX_LABELS];
	public:
		CTag() : m_Active(false), m_Color(0xffffffu)
		{
			for(int i = 0; i < MAX_LABELS; i++)
				m_pLabels[i] = 0;
		}

		void SetColor(unsigned col)
		{
			m_Color = col;
		}

		void SetActive(bool Active)
		{
			m_Active = Active;
		}

		bool Active()
		{
			return m_Active;
		}

		void AddLabel(const char *pLabel)
		{
			int i = 0;
			for(; i < MAX_LABELS; i++)
				if (!m_pLabels[i])
					break;
			if (i == MAX_LABELS)
				return;

			m_pLabels[i] = pLabel;
		}

		void DelLabel(const char *pLabel)
		{
			for(int i = 0; i < MAX_LABELS; i++)
				if (m_pLabels[i] && str_comp(m_pLabels[i], pLabel) == 0)
				{
					m_pLabels[i] = 0;
					break;
				}
		}

		bool HasLabel(const char *pLabel)
		{
			for(int i = 0; i < MAX_LABELS; i++)
				if (m_pLabels[i] && str_comp(m_pLabels[i], pLabel) == 0)
					return true;
			return false;
		}

		void Reset()
		{
			m_Color = 0xffffff;

		}

		void GetLabelStr(char *pDst, size_t DstSz)
		{
			*pDst = '\0';
			for(int i = 0; i < MAX_LABELS; i++)
				if (m_pLabels[i])
				{
					str_append(pDst, m_pLabels[i], DstSz);
					str_append(pDst, ", ", DstSz);
				}
			int len = str_length(pDst);
			if (len >= 2)
				pDst[len-2] = '\0';
		}

		unsigned GetColor()
		{
			return m_Color;
		}
};


class CTagSys : public CComponent
{
private:
	int m_SelectedID; //client id
	int m_InSight[MAX_CLIENTS];
	int m_NumInSight;
	int m_CurMenuInd;	

	vec2 m_TeePos[MAX_CLIENTS];

	CTag m_aTags[MAX_CLIENTS];

	bool InRange();
	void UpdateInSight();
	void UpdatePositions();
	void UpdateTags();

	class CTagMenuNode *m_pMenuRoot;
	class CTagMenuNode *m_pCurMenuNode;
	void BuildMenu();
	void RenderMenu();
	void RenderSelector();
	void RenderTags();
	void MenuLabel(const char *tag);
	void MenuColor(unsigned col);
public:
	CTagSys();
	virtual ~CTagSys();

	virtual void OnStateChange(int NewState, int OldState);
	virtual void OnConsoleInit();
	virtual void OnInit();
	virtual void OnReset();
	virtual void OnRender();
	virtual void OnRelease();
	virtual void OnMapLoad();
	virtual void OnMessage(int Msg, void *pRawMsg);
	virtual bool OnMouseMove(float x, float y);
	virtual bool OnInput(IInput::CEvent e);

	void SelNext();
	void SelPrev();
	void SelHit();
	int GetSel();
	static void ConSelNext(IConsole::IResult *pResult, void *pUserData);
	static void ConSelPrev(IConsole::IResult *pResult, void *pUserData);
	static void ConSelHit(IConsole::IResult *pResult, void *pUserData);
	static void MenuCBLabel(CTagSys *pThis, const void *pUserData);
	static void MenuCBColor(CTagSys *pThis, const void *pUserData);
};

#endif

