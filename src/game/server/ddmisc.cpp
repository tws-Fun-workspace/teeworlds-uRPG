#include <base/system.h>

#include <game/server/player.h>

#include "ddmisc.h"

#include "../../engine/shared/config.h"

CDDChatHnd::CDDChatHnd()
{
	IChatCtl::Register(this);
}

bool CDDChatHnd::HandleChatMsg(class CPlayer *pPlayer, const char *pMsg)
{
	if (str_comp(pMsg, "/p") != 0)
		return false;

	pPlayer->m_PersonalBroadcastTick = 0;
	char buf[1];
	*buf = 0;
	GameContext()->SendBroadcast(buf, pPlayer->GetCID());

	CCharacter* ch = pPlayer->GetCharacter();
	if (!ch || ch->m_CKPunishTick < GameContext()->Server()->Tick() || !GameContext()->GetPlayerByUID(ch->m_CKPunish))
		return true;

	CCharacter* killer = GameContext()->GetPlayerByUID(ch->m_CKPunish)->GetCharacter();
	if (!killer)
		return true;

	ch->m_CKPunishTick = 0;

	if (g_Config.m_SvChatblockPunish > 0)
		killer->Freeze(g_Config.m_SvChatblockPunish * GameContext()->Server()->TickSpeed());
	else if (g_Config.m_SvChatblockPunish == -1)
	{
		vec2 tmp = killer->GetPos();
		killer->SetPos(ch->GetPos());
		ch->SetPos(tmp);
	}

	char aBuf[200];
	str_format(aBuf, sizeof(aBuf), "You were punished by %s for chatkilling him!", GameContext()->Server()->ClientName(pPlayer->GetCID()));
	GameContext()->SendChatTarget(killer->GetPlayer()->GetCID(), aBuf);

	return true;
}
