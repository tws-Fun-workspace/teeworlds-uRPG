
#include <base/system.h>
#include <base/vmath.h>

#include <engine/textrender.h>
#include <engine/shared/config.h>
#include <engine/graphics.h>

#include <game/generated/client_data.h>

#include "tag.h"


typedef void (*TFPMenuCB)(CTagSys*, const void*);

class CTagMenuNode
{
private:
	char m_aName[64];

	std::vector<class CTagMenuNode*> m_Children; //oh unethical me.
	bool m_Leaf;
	TFPMenuCB m_CB;
	const void *m_pUserData;
	class CTagSys *m_pTagSys;

public:
	CTagMenuNode(const char *pName, TFPMenuCB Cb, class CTagSys *pTag, const void *pUserData)
	: m_Leaf(true), m_CB(Cb), m_pUserData(pUserData), m_pTagSys(pTag)
	{
		str_copy(m_aName, pName, sizeof m_aName);
	}

	CTagMenuNode(const char *pName)
	: m_Leaf(false), m_CB(0), m_pUserData(0), m_pTagSys(0)
	{
		str_copy(m_aName, pName, sizeof m_aName);
	}

	void AddChild(class CTagMenuNode *pNode) { m_Children.push_back(pNode); }

	bool Leaf() { return m_Leaf; }
	const char *Name() { return m_aName; }
	void Call() { m_CB(m_pTagSys, m_pUserData); }
	CTagMenuNode *Child(size_t i) { return m_Children[i]; }
	int NumChildren() { return m_Children.size(); }
};


CTagSys::CTagSys()
: m_SelectedID(-1), m_NumInSight(0), m_CurMenuInd(0), m_pMenuRoot(0), m_pCurMenuNode(0)
{
	BuildMenu();
}

CTagSys::~CTagSys()
{
}

void CTagSys::MenuLabel(const char *pLabel)
{
	if (m_SelectedID == -1)
		return;

	if (m_aTags[m_SelectedID].HasLabel(pLabel))
		m_aTags[m_SelectedID].DelLabel(pLabel);
	else
		m_aTags[m_SelectedID].AddLabel(pLabel);
	
}

void CTagSys::MenuColor(unsigned col)
{
	if (m_SelectedID == -1)
		return;

	m_aTags[m_SelectedID].SetColor(col);
}



bool InRect(vec2 P, vec2 UL, vec2 LR)
{
	return UL.x <= P.x && P.x <= LR.x 
	    && UL.y <= P.y && P.y <= LR.y; 
}

void CTagSys::UpdateInSight()
{
	int c = 0;
	vec2 MyPos = m_pClient->m_LocalCharacterPos;
	for(int i = 0; i < MAX_CLIENTS; i++)
	{
		if(m_pClient->m_Snap.m_aCharacters[i].m_Active
			    && i != m_pClient->m_Snap.m_LocalClientID
			    && InRect(m_TeePos[i],
			             vec2(MyPos.x - g_Config.m_ClSelAreaX/2,
			                  MyPos.y - g_Config.m_ClSelAreaY/2),
				     vec2(MyPos.x + g_Config.m_ClSelAreaX/2,
                                          MyPos.y + g_Config.m_ClSelAreaY/2)))
			m_InSight[c++] = i;
	}

	m_NumInSight = c;
	dbg_msg("TAG", "%d in sight!", m_NumInSight);
}

void CTagSys::OnStateChange(int NewState, int OldState)
{
	dbg_msg("TAG", "invoke: OnStateChange()");
}

void CTagSys::ConSelNext(IConsole::IResult *pResult, void *pUserData)
{
	((CTagSys *)pUserData)->SelNext();
}

void CTagSys::ConSelPrev(IConsole::IResult *pResult, void *pUserData)
{
	((CTagSys *)pUserData)->SelPrev();
}

void CTagSys::ConSelHit(IConsole::IResult *pResult, void *pUserData)
{
	((CTagSys *)pUserData)->SelHit();
}

bool CTagSys::InRange()
{
	bool Found = false;
	if (m_SelectedID == -1)
		return false;

	for(int i = 0; !Found && i < m_NumInSight; i++)
		if (m_InSight[i] == m_SelectedID)
			Found = true;
	return Found;
}

void CTagSys::SelNext()
{
	UpdatePositions();
	UpdateInSight();

	if (!InRange())
	{
		m_SelectedID = -1;
		m_pCurMenuNode = 0;
	}

	if (m_pCurMenuNode)
	{
		if (++m_CurMenuInd >= m_pCurMenuNode->NumChildren())
			m_CurMenuInd = 0;
	}
	else
	{
		if (!m_NumInSight)
			return;

		if (m_SelectedID == -1)
			m_SelectedID = m_InSight[m_NumInSight - 1];
		
		bool Found = false;
		do {
			if (++m_SelectedID >= MAX_CLIENTS)
				m_SelectedID = 0;

			for(int j = 0; !Found && j < m_NumInSight; ++j)
				if (m_InSight[j] == m_SelectedID)
					Found = true;
		} while(!Found);

		dbg_msg("TAG", "selected %d", m_SelectedID);
	}
}

void CTagSys::SelPrev()
{
	UpdatePositions();
	UpdateInSight();

	if (!InRange())
	{
		m_SelectedID = -1;
		m_pCurMenuNode = 0;
	}

	if (m_pCurMenuNode)
	{
		if (--m_CurMenuInd < 0)
			m_CurMenuInd = m_pCurMenuNode->NumChildren() - 1;
	}
	else
	{
		if (!m_NumInSight)
			return;

		if (m_SelectedID == -1)
			m_SelectedID = m_InSight[0];
		
		bool Found = false;
		do {
			if (--m_SelectedID < 0)
				m_SelectedID = MAX_CLIENTS - 1;

			for(int j = 0; !Found && j < m_NumInSight; ++j)
				if (m_InSight[j] == m_SelectedID)
					Found = true;
		} while(!Found);

		dbg_msg("TAG", "selected %d", m_SelectedID);
	}
}

void CTagSys::SelHit()
{
	if (m_SelectedID == -1)
		return;
	
	if (!m_pCurMenuNode)
	{
		m_pCurMenuNode = m_pMenuRoot;
		m_CurMenuInd = 0;
	}
	else
	{
		m_pCurMenuNode = m_pCurMenuNode->Child(m_CurMenuInd);
		m_CurMenuInd = 0;
		if (m_pCurMenuNode->Leaf())
		{
			m_pCurMenuNode->Call();
			m_pCurMenuNode = 0;
		}
	}
}

int CTagSys::GetSel()
{
	return m_SelectedID;
}

void CTagSys::OnConsoleInit()
{
	dbg_msg("TAG", "invoke: OnConsoleInit()");
	Console()->Register("selnext", "", CFGFLAG_CLIENT, ConSelNext, this, "");
	Console()->Register("selprev", "", CFGFLAG_CLIENT, ConSelPrev, this, "");
	Console()->Register("selhit", "", CFGFLAG_CLIENT, ConSelHit, this, "");
}

void CTagSys::OnInit()
{
	dbg_msg("TAG", "invoke: OnInit()");
}

void CTagSys::OnReset()
{
	dbg_msg("TAG", "invoke: OnReset()");
}

void CTagSys::UpdateTags()
{
	for(int i = 0; i < MAX_CLIENTS; i++)
	{
		if(m_pClient->m_aClients[i].m_Active)
		{
			if (!m_aTags[i].Active())
			{
				m_aTags[i].Reset();
				m_aTags[i].SetActive(true);
			}
		}
		else
		{
			if (m_aTags[i].Active())
			{
				m_aTags[i].SetActive(false);
			}

		}
	}
}

void CTagSys::UpdatePositions()
{
	for(int i = 0; i < MAX_CLIENTS; i++)
	{
		if(!m_pClient->m_Snap.m_aCharacters[i].m_Active)
			continue;

		const void *pPrevInfo = Client()->SnapFindItem(IClient::SNAP_PREV, NETOBJTYPE_PLAYERINFO, i);
		const void *ppInfo = Client()->SnapFindItem(IClient::SNAP_CURRENT, NETOBJTYPE_PLAYERINFO, i);

		if(!pPrevInfo || !ppInfo)
			continue;

		CNetObj_Character Prev = m_pClient->m_Snap.m_aCharacters[i].m_Prev;
		CNetObj_Character Player = m_pClient->m_Snap.m_aCharacters[i].m_Cur;
		CNetObj_PlayerInfo pInfo = *(CNetObj_PlayerInfo*)ppInfo;
		CTeeRenderInfo RenderInfo = m_pClient->m_aClients[pInfo.m_ClientID].m_RenderInfo;

		float IntraTick = Client()->IntraGameTick();


		// use preditect players if needed
		if(pInfo.m_Local && g_Config.m_ClPredict)
		{
			if(m_pClient->m_Snap.m_pLocalCharacter && !(m_pClient->m_Snap.m_pGameInfoObj && m_pClient->m_Snap.m_pGameInfoObj->m_GameStateFlags&GAMESTATEFLAG_GAMEOVER))
			{
				// apply predicted results
				m_pClient->m_PredictedChar.Write(&Player);
				m_pClient->m_PredictedPrevChar.Write(&Prev);
				IntraTick = Client()->PredIntraGameTick();
			}
		}

		m_TeePos[i] = mix(vec2(Prev.m_X, Prev.m_Y), vec2(Player.m_X, Player.m_Y), IntraTick);
	}

}

/*
	if (TickDiff > 0)
		UpdateInSight();

	bool Found = false;


	for(int i = 0; !Found && i < m_NumInSight; i++)
		if (m_InSight[i] == m_SelectedID)
			Found = true;

	if (!Found)
	{
		dbg_msg("TAG", "deselected!");
		m_SelectedID = -1;
	}

	if (m_SelectedID == -1)
		return;
*/
void CTagSys::OnRender()
{
	UpdateTags();
	UpdatePositions();
	UpdateInSight();

	RenderSelector();

	RenderTags();

	RenderMenu();
}

void CTagSys::RenderSelector()
{
	if (m_SelectedID == -1)
		return;

	static float s_Angle = 0.f;
	static int s_LastTick = 0;
	int TickDiff = Client()->GameTick() - s_LastTick;

	vec2 SelTeePos = m_TeePos[m_SelectedID];

	if (TickDiff > 0)
	{
		s_LastTick = Client()->GameTick();
		s_Angle += (((float)TickDiff/Client()->GameTickSpeed()) * (2.f*pi)) / 2.f;
		if (s_Angle >= 2.f*pi)
			s_Angle = 0.f;
	}

	Graphics()->TextureSet(g_pData->m_aImages[IMAGE_GAME].m_Id);
	Graphics()->QuadsBegin();
	Graphics()->QuadsSetRotation(s_Angle);

	if (m_pCurMenuNode)
		Graphics()->SetColor(0.2f,0.2f,0.8f, 1.0f);
	else
		Graphics()->SetColor(0.0f,0.7f,0.0f, 1.0f);

	RenderTools()->SelectSprite(SPRITE_TAGSEL);
	IGraphics::CQuadItem QuadItem(SelTeePos.x, SelTeePos.y-4, 96, 96);
	Graphics()->QuadsDraw(&QuadItem, 1);
	Graphics()->QuadsEnd();
}

void CTagSys::RenderMenu()
{
	if (m_SelectedID == -1 || !m_pCurMenuNode)
		return;

	vec2 Position = m_TeePos[m_SelectedID];
	
	float FontSize = 18.0f + 20.0f * g_Config.m_ClNameplatesSize / 100.0f;

	char aBuf[64];
	str_format(aBuf, sizeof aBuf, "%s", m_pCurMenuNode->Child(m_CurMenuInd)->Name());

	float tw = TextRender()->TextWidth(0, FontSize, aBuf, -1);

	TextRender()->TextOutlineColor(0.0f, 0.0f, 0.0f, 0.5f);
	TextRender()->TextColor(0.8f, 0.8f, 0.8f, 1.0f);

	TextRender()->Text(0, Position.x-tw/2.0f, Position.y-2*FontSize-44.0f, FontSize, aBuf, -1);

}

void CTagSys::RenderTags()
{
	for(int i = 0; i < MAX_CLIENTS; i++)
	{
		if (i == m_SelectedID && m_pCurMenuNode)
			continue;

		if (!m_aTags[i].Active())
		{
			dbg_msg("tag", "%i aint active", i);
			continue;
		}

		const void *pInfo = Client()->SnapFindItem(IClient::SNAP_CURRENT, NETOBJTYPE_PLAYERINFO, i);

		if(!pInfo)
		{
			dbg_msg("tag", "%i no info", i);
			continue;
		}
			
		const CNetObj_PlayerInfo *pPlayerInfo = (const CNetObj_PlayerInfo *)pInfo;

		if(pPlayerInfo->m_Local)
		{
			dbg_msg("tag", "%i is local", i);
			continue;
		}

		vec2 Position = m_TeePos[i];

		float FontSize = 18.0f + 20.0f * g_Config.m_ClNameplatesSize / 100.0f;

		char aBuf[64];
		m_aTags[i].GetLabelStr(aBuf, sizeof aBuf);

		dbg_msg("tag", "%i rendering '%s'", i, aBuf);
		float tw = TextRender()->TextWidth(0, FontSize, aBuf, -1);

		TextRender()->TextOutlineColor(0.0f, 0.0f, 0.0f, 0.5f);

		unsigned Col = m_aTags[i].GetColor();

		TextRender()->TextColor(Col&0xffu, (Col&0xff00u)>>8, (Col&0xff0000u)>>16, 1.0);

		TextRender()->Text(0, Position.x-tw/2.0f, Position.y-2*FontSize-44.0f, FontSize, aBuf, -1);

	}
	TextRender()->TextColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void CTagSys::OnRelease()
{
	dbg_msg("TAG", "invoke: OnRelease()");
}

void CTagSys::OnMapLoad()
{
	dbg_msg("TAG", "invoke: OnMapLoad()");
}

void CTagSys::OnMessage(int Msg, void *pRawMsg)
{
}

bool CTagSys::OnMouseMove(float x, float y)
{
	dbg_msg("TAG", "invoke: OnMouseMove()");
	return false;
}

bool CTagSys::OnInput(IInput::CEvent e)
{

	dbg_msg("TAG", "invoke: OnInput()");
	return false;
}



void CTagSys::BuildMenu()
{
	static const unsigned s_ColRed = 0xff;
	static const unsigned s_ColGreen = 0xff00;
	static const unsigned s_ColBlue = 0xff0000;
	static const unsigned s_ColWhite = 0xffffff;
	static const unsigned s_ColBlack = 0x0;

	CTagMenuNode *n, *m;
	n = m_pMenuRoot = new CTagMenuNode("(root)");

	n->AddChild(m = new CTagMenuNode("TAG"));
	    m->AddChild(new CTagMenuNode("FRIEND", MenuCBLabel, this, "FRIEND"));
	    m->AddChild(new CTagMenuNode("ENEMY", MenuCBLabel, this, "ENEMY"));
	    m->AddChild(new CTagMenuNode("NEUTRAL", MenuCBLabel, this, "NEUTRAL"));
	    m->AddChild(new CTagMenuNode("FAGGOT", MenuCBLabel, this, "FAGGOT"));
	    m->AddChild(new CTagMenuNode("VIP", MenuCBLabel, this, "VIP"));
	    m->AddChild(new CTagMenuNode("TEAM", MenuCBLabel, this, "TEAM"));
	    m->AddChild(new CTagMenuNode("NOOB", MenuCBLabel, this, "NOOB"));
	n->AddChild(m = new CTagMenuNode("COLOR"));
	    m->AddChild(new CTagMenuNode("RED", MenuCBColor, this, &s_ColRed));
	    m->AddChild(new CTagMenuNode("GREEN", MenuCBColor, this, &s_ColGreen));
	    m->AddChild(new CTagMenuNode("BLUE", MenuCBColor, this, &s_ColBlue));
	    m->AddChild(new CTagMenuNode("WHITE", MenuCBColor, this, &s_ColWhite));
	    m->AddChild(new CTagMenuNode("BLACK", MenuCBColor, this, &s_ColBlack));
	


}

void CTagSys::MenuCBLabel(CTagSys *pThis, const void *pUserData)
{
	pThis->MenuLabel((const char*)pUserData);
}

void CTagSys::MenuCBColor(CTagSys *pThis, const void *pUserData)
{
	pThis->MenuColor(*(unsigned*)pUserData);
}
