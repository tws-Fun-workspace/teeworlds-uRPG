/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include "mod.h"

#include <engine/shared/protocol.h>
#include <engine/shared/config.h>
#include <game/server/gamecontext.h>
#include <game/server/entities/character.h>

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

	static int LastBounceTick[MAX_CLIENTS] = {0};
	for(int i = 0; i < MAX_CLIENTS; i++)
	{
		CCharacter *pChr = GameServer()->GetPlayerChar(i);
		if (!pChr || LastBounceTick[i] + g_Config.m_SvBounceDelay > Server()->Tick())
			continue;
		if (GameServer()->Collision()->GetCollisionAt(pChr->m_Pos.x, pChr->m_Pos.y) == TILE_BOUNCE)
		{
			GameServer()->CreateExplosion(pChr->m_Pos, i, WEAPON_GRENADE, true);
			GameServer()->CreateSound(pChr->m_Pos, SOUND_GRENADE_EXPLODE);
			pChr->TakeDamage(vec2(g_Config.m_SvBounceXforce/10.0f, g_Config.m_SvBounceYforce/10.0f), 0, i, WEAPON_GRENADE);
			LastBounceTick[i] = Server()->Tick();
		}
	}
}
