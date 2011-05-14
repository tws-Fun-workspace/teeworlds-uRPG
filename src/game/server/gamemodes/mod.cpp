/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include "mod.h"

#include <engine/shared/protocol.h>
#include <engine/shared/config.h>
#include <game/server/gamecontext.h>

CGameControllerMOD::CGameControllerMOD(class CGameContext *pGameServer)
: IGameController(pGameServer)
{
	// Exchange this to a string that identifies your game mode.
	// DM, TDM and CTF are reserved for teeworlds original modes.
	m_pGameType = "MOD";

	//m_GameFlags = GAMEFLAG_TEAMS; // GAMEFLAG_TEAMS makes it a two-team gamemode
}

void CGameControllerMOD::Tick()
{
	// this is the main part of the gamemode, this function is run every tick
	DoPlayerScoreWincheck(); // checks for winners, no teams version
	//DoTeamScoreWincheck(); // checks for winners, two teams version

	IGameController::Tick();

	static int RegainHammerTick[MAX_CLIENTS] = {0};

	for(int i = 0; i < MAX_CLIENTS; i++)
	{
		CCharacter *pChr = GameServer()->GetPlayerChar(i);
		if (!pChr)
			continue;

		if (pChr->GetHammerScore() > g_Config.m_SvHammerThreshold)
		{
			pChr->TakeWeapon(WEAPON_HAMMER);
			pChr->SetHammerScore(0);
			RegainHammerTick[i] = Server()->Tick() + g_Config.m_SvHammerPunishtime * Server()->TickSpeed();
			char aBuf[128];
			str_format(aBuf, sizeof aBuf, "WARNING: Your hammer has been taken away for %d seconds, because you are overusing it.", g_Config.m_SvHammerPunishtime);
			GameServer()->SendChatTarget(i, aBuf);
		}
		if (RegainHammerTick[i] > 0 && RegainHammerTick[i] <= Server()->Tick())
		{
			RegainHammerTick[i] = 0;
			pChr->GiveWeapon(WEAPON_HAMMER, -1);
			pChr->SetWeapon(WEAPON_HAMMER); // XXX
			GameServer()->SendChatTarget(i, "Here you got your hammer back, this time do not overuse it!");
		}
	}
}
