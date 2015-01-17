/* (c) Timo Buhrmester. See licence.txt in the root of the distribution   */
/* for more information. If you are missing that file, acquire a complete */
/* release at teeworlds.com.                                              */

#include <engine/shared/config.h>
#include <game/server/gamecontext.h>
#include <game/server/entities/character.h>
#include "mod.h"

#define TS Server()->TickSpeed()
#define TICK Server()->Tick()

#define GS GameServer()
#define CHAR(C) (((C) < 0 || (C) >= MAX_CLIENTS) ? 0 : GS->GetPlayerChar(C))
#define CFG(A) g_Config.m_Sv ## A
#define D(F, ARGS...) dbg_msg("mod", "%s:%i:%s(): " F, __FILE__, __LINE__, \
                                                            __func__,##ARGS)

CGameControllerMOD::CGameControllerMOD(class CGameContext *pGameServer)
: IGameController(pGameServer)
{
	m_pGameType = "openfng";
	m_GameFlags = GAMEFLAG_TEAMS;
}

CGameControllerMOD::~CGameControllerMOD()
{
}

void CGameControllerMOD::Tick()
{
	DoTeamScoreWincheck();
	IGameController::Tick();

	for(int i = 0; i < MAX_CLIENTS; i++)
	{
		CCharacter *pChr = CHAR(i);
		if (!pChr)
			continue;

		int FrzTicks = pChr->GetFreezeTicks();

		if (FrzTicks > 0)
		{
			m_aMoltenBy[i] = -1;
			int Killer = pChr->WasFrozenBy();
			if (Killer < 0 || Killer == m_aFrozenBy[i])
				continue;
			m_aFrozenBy[i] = Killer;
			
			HandleFreeze(Killer, i);
		}
		else
		{
			m_aFrozenBy[i] = -1;
			int Melter = pChr->WasMoltenBy();
			if (Melter < 0 || Melter == m_aMoltenBy[i])
				continue;
			m_aMoltenBy[i] = Melter;

			HandleMelt(Melter, i);
		}
	}
}

void CGameControllerMOD::HandleFreeze(int Killer, int Victim)
{
	CCharacter *pVictim = CHAR(Victim);
	int FailTeam = pVictim->GetPlayer()->GetTeam() & 1;
	m_aTeamscore[1 - FailTeam] += CFG(FreezeTeamscore);

	CCharacter *pKiller = CHAR(Killer);
	if (!pKiller)
		return;

	pKiller->GetPlayer()->m_Score += CFG(FreezeScore);
	SendFreezeKill(Killer, Victim);
}

void CGameControllerMOD::HandleMelt(int Melter, int Meltee)
{
	CCharacter *pMeltee = CHAR(Meltee);
	int MeltTeam = pMeltee->GetPlayer()->GetTeam()&1;
	m_aTeamscore[MeltTeam] += CFG(MeltTeamscore);

	CCharacter *pMelter = CHAR(Melter);
	if (!pMelter)
		return;

	pMelter->GetPlayer()->m_Score += CFG(MeltScore);
}

void CGameControllerMOD::HandleSacr(int Killer, int Victim)
{
	CCharacter *pVictim = CHAR(Victim);
	int FailTeam = pVictim->GetPlayer()->GetTeam()&1;
	m_aTeamscore[1-FailTeam] += CFG(SacrTeamscore);

	CCharacter* pKiller = CHAR(Killer);
	if (!pKiller)
		return;

	pKiller->GetPlayer()->m_Score += CFG(SacrScore);
}

void CGameControllerMOD::SendFreezeKill(int Killer, int Victim)
{
	char aBuf[256];
	str_format(aBuf, sizeof(aBuf), "freez0r='%d:%s' victim='%d:%s'",
	                              Killer, Server()->ClientName(Killer),
	                              Victim, Server()->ClientName(Victim));

	GS->Console()->Print(IConsole::OUTPUT_LEVEL_DEBUG, "game", aBuf);

	// send the kill message
	CNetMsg_Sv_KillMsg Msg;
	Msg.m_Killer = Killer;
	Msg.m_Victim = Victim;
	Msg.m_Weapon = WEAPON_RIFLE;
	Msg.m_ModeSpecial = 0;
	Server()->SendPackMsg(&Msg, MSGFLAG_VITAL, -1);
}

void CGameControllerMOD::Snap(int SnappingClient)
{
	IGameController::Snap(SnappingClient);

	CNetObj_GameData *pGameDataObj = (CNetObj_GameData*)Server()->
	      SnapNewItem(NETOBJTYPE_GAMEDATA, 0, sizeof(CNetObj_GameData));

	if(!pGameDataObj)
		return;

	pGameDataObj->m_TeamscoreRed = m_aTeamscore[TEAM_RED];
	pGameDataObj->m_TeamscoreBlue = m_aTeamscore[TEAM_BLUE];

	pGameDataObj->m_FlagCarrierRed = 0;
	pGameDataObj->m_FlagCarrierBlue = 0;
}

void CGameControllerMOD::OnCharacterSpawn(class CCharacter *pVictim)
{
	IGameController::OnCharacterSpawn(pVictim);
	pVictim->TakeWeapon(WEAPON_GUN);
	pVictim->GiveWeapon(WEAPON_RIFLE, -1);
	pVictim->SetWeapon(WEAPON_RIFLE);
}

bool CGameControllerMOD::OnEntity(int Index, vec2 Pos)
{
	switch(Index)
	{
	case ENTITY_SPAWN:
	case ENTITY_SPAWN_RED:
	case ENTITY_SPAWN_BLUE:
		return IGameController::OnEntity(Index, Pos);

	default:
		if (!CFG(SuppressEntities))
			return IGameController::OnEntity(Index, Pos);
	}

	return false;
}

int CGameControllerMOD::OnCharacterDeath(class CCharacter *pVictim,
                                   class CPlayer *pUnusedKiller, int Weapon)
{
	//IGameController::OnCharacterDeath(pVictim, pKiller, Weapon);
	if (Weapon != WEAPON_WORLD || pVictim->GetFreezeTicks() <= 0)
		return 0;
	
	HandleSacr(pVictim->WasFrozenBy(), pVictim->GetPlayer()->GetCID());
	return 0;
}

void CGameControllerMOD::PostReset()
{
	for(int i = 0; i < MAX_CLIENTS; i++)
		m_aFrozenBy[i] = m_aMoltenBy[i] = -1;
}
/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include "mod.h"

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

	IGameController::Tick();
}
