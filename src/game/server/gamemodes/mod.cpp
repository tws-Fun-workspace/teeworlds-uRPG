/* (c) Timo Buhrmester. See licence.txt in the root of the distribution   */
/* for more information. If you are missing that file, acquire a complete */
/* release at teeworlds.com.                                              */

#include <base/system.h>

#include <engine/shared/config.h>

#include <game/server/gamecontext.h>
#include <game/server/entities/character.h>

#include "mod.h"

#define TS Server()->TickSpeed()
#define TICK Server()->Tick()

#define GS GameServer()
#define CHAR(C) (((C) < 0 || (C) >= MAX_CLIENTS) ? 0 : GS->GetPlayerChar(C))
#define PLAYER(C) (((C) < 0 || (C) >= MAX_CLIENTS) ? 0 : GS->m_apPlayers[C])
#define TPLAYER(C) (((C) < 0 || (C) >= MAX_CLIENTS) ? 0 : (GS->IsClientReady(C) && GS->IsClientPlayer(C)) ? GS->m_apPlayers[C] : 0)
#define CFG(A) g_Config.m_Sv ## A
#define D(F, ARGS...) dbg_msg("mod", "%s:%i:%s(): " F, __FILE__, __LINE__, \
                                                            __func__,##ARGS)

CGameControllerMOD::CGameControllerMOD(class CGameContext *pGameServer)
: IGameController(pGameServer)
{
	m_pGameType = "openfng";
	m_GameFlags = GAMEFLAG_TEAMS;
	m_aCltMask[0] = m_aCltMask[1] = 0;
	PostReset();
}

CGameControllerMOD::~CGameControllerMOD()
{
}

void CGameControllerMOD::Tick()
{
	DoTeamScoreWincheck();
	IGameController::Tick();

	DoHookers();

	bool Empty = true;

	for(int i = 0; i < MAX_CLIENTS; i++)
	{
		if (Empty && TPLAYER(i))
			Empty = false;

		CCharacter *pChr = CHAR(i);
		if (!pChr)
			continue;

		Empty = false;

		int FrzTicks = pChr->GetFreezeTicks();

		if (FrzTicks > 0)
		{
			if ((FrzTicks+1) % TS == 0)
				GS->CreateDamageInd(pChr->m_Pos, 0, (FrzTicks+1) / TS, m_aCltMask[pChr->GetPlayer()->GetTeam()&1]);

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

	if (Empty)
		m_aTeamscore[0] = m_aTeamscore[1] = 0;
	else
		DoScoreDisplays();

	DoBroadcasts();
	DoAYB();
}

void CGameControllerMOD::DoAYB()
{
	if (m_GameOverTick != -1 || !CFG(AllYourBase))
		return;

	if (max(m_aTeamscore[0], m_aTeamscore[1]) + CFG(AllYourBase) >= CFG(Scorelimit))
		Broadcast("ALL YOUR BASE ARE BELONG TO US.", TS);

}

void CGameControllerMOD::DoBroadcasts(bool ForceSend)
{
	if (m_BroadcastStop >= 0 && m_BroadcastStop < TICK)
	{
		m_aBroadcast[0] = ' '; m_aBroadcast[1] = '\0';
		m_BroadcastStop = -1;
	}

	if ((ForceSend || m_NextBroadcast < TICK) && *m_aBroadcast && CFG(Broadcasts))
	{
		for(int i = 0; i < MAX_CLIENTS; i++)
			if (TPLAYER(i))
				GS->SendBroadcast(m_aBroadcast, i);
		m_NextBroadcast = TICK + TS;
	}
}

void CGameControllerMOD::DoScoreDisplays()
{
	if (!m_aScoreDisplayCount[0] && !m_aScoreDisplayCount[1])
		return;

	for(int i = 0; i < 2; i++)
	{
		if (m_aScoreDisplayValue[i] != m_aTeamscore[i])
		{
			char aBuf[16];
			str_format(aBuf, sizeof aBuf, "%d", m_aTeamscore[i]);
			D("i: %d, dv: %d, ts: %d, dc: %d",i, m_aScoreDisplayValue[i], m_aTeamscore[i], m_aScoreDisplayCount[i]);
			m_aScoreDisplayValue[i] = m_aTeamscore[i];
			for(int j = 0; j < m_aScoreDisplayCount[i]; j++)
			{
				if (m_aScoreDisplayTextIDs[i][j] >= 0)
					GS->DestroyLolText(m_aScoreDisplayTextIDs[i][j]);
				m_aScoreDisplayTextIDs[i][j] = GS->CreateLolText(0, false, m_aScoreDisplays[i][j], vec2(0.f, 0.f), 3600 * TS, aBuf);
			}
		}
	}
}

void CGameControllerMOD::DoHookers()
{
	for(int i = 0; i < MAX_CLIENTS; i++)
	{
		CCharacter *pChr = CHAR(i);
		if (!pChr)
			continue;
		
		int Hooking = pChr->GetHookedPlayer();

		if (Hooking >= 0 && pChr->GetHookTick() < CFG(HookRegisterDelay))
			Hooking = -1;

		int HammeredBy = pChr->LastHammeredBy();
		pChr->ClearLastHammeredBy();

		if (Hooking >= 0)
		{
			CCharacter *pVic = CHAR(Hooking);
			if (pVic)
			{
				bool SameTeam = pChr->GetPlayer()->GetTeam() == pVic->GetPlayer()->GetTeam();
				m_aLastInteraction[Hooking] = SameTeam ? -1 : i;
			}
		}

		if (HammeredBy >= 0)
		{	
			D("%d was hammered by %d", i, HammeredBy);
			CCharacter *pHam = CHAR(HammeredBy);
			bool SameTeam = pChr->GetPlayer()->GetTeam() == pHam->GetPlayer()->GetTeam();
			if (SameTeam)
				D("same team, bitch");
			m_aLastInteraction[i] = SameTeam ? -1 : HammeredBy;
		}
	}
}

void CGameControllerMOD::Broadcast(const char *pText, int Ticks)
{
	str_copy(m_aBroadcast, pText, sizeof m_aBroadcast);
	m_BroadcastStop = Ticks < 0 ? -1 : (TICK + Ticks);
}

void CGameControllerMOD::HandleFreeze(int Killer, int Victim)
{
	CCharacter *pVictim = CHAR(Victim);
	if (CFG(BleedOnFreeze))
	{
		pVictim->Bleed(1);
		GS->CreateSound(pVictim->m_Pos, SOUND_CTF_RETURN);
	}

	int FailTeam = pVictim->GetPlayer()->GetTeam() & 1;
	m_aTeamscore[1 - FailTeam] += CFG(FreezeTeamscore);

	CPlayer *pPlKiller = TPLAYER(Killer);

	if (!pPlKiller)
		return;

	//freezing counts as a hostile interaction
	m_aLastInteraction[pVictim->GetPlayer()->GetCID()] = pPlKiller->GetCID();

	pPlKiller->m_Score += CFG(FreezeScore);
	SendFreezeKill(Killer, Victim, WEAPON_RIFLE);

	if (pPlKiller->GetCharacter() && CFG(FreezeLoltext) && CFG(FreezeScore))
	{
		char aBuf[64];
		str_format(aBuf, sizeof aBuf, "%+d", CFG(FreezeScore));
		GS->CreateLolText(pPlKiller->GetCharacter(), false, vec2(0.f, -50.f), vec2(0.f, 0.f), 50, aBuf);
	}
}

void CGameControllerMOD::HandleMelt(int Melter, int Meltee)
{
	CCharacter *pMeltee = CHAR(Meltee);
	int MeltTeam = pMeltee->GetPlayer()->GetTeam()&1;
	m_aTeamscore[MeltTeam] += CFG(MeltTeamscore);

	CPlayer *pPlMelter = TPLAYER(Melter);

	if (!pPlMelter)
		return;

	pPlMelter->m_Score += CFG(MeltScore);
	SendFreezeKill(Melter, Meltee, WEAPON_HAMMER);

	if (pPlMelter->GetCharacter() && CFG(MeltLoltext) && CFG(MeltScore))
	{
		char aBuf[64];
		str_format(aBuf, sizeof aBuf, "%+d", CFG(MeltScore));
		GS->CreateLolText(pPlMelter->GetCharacter(), false, vec2(0.f, -50.f), vec2(0.f, 0.f), 50, aBuf);
	}
}

void CGameControllerMOD::HandleSacr(int Killer, int Victim)
{
	CCharacter *pVictim = CHAR(Victim);

	int FailTeam = pVictim->GetPlayer()->GetTeam()&1;
	m_aTeamscore[1-FailTeam] += CFG(SacrTeamscore);

	if (CFG(SacrSound) == 1)
		GameServer()->CreateSoundGlobal(SOUND_CTF_CAPTURE);
	else if (CFG(SacrSound) == 2)
		GameServer()->CreateSound(pVictim->m_Pos, SOUND_CTF_CAPTURE);

	if (CFG(SacrTeamscore))
	{
		char aBuf[64];
		str_format(aBuf, sizeof aBuf, "%s scored (%+d)", GetTeamName(1-FailTeam), CFG(SacrTeamscore));
		Broadcast(aBuf, CFG(BroadcastTime) * TS);
	}

	CPlayer *pPlKiller = TPLAYER(Killer);
	if (!pPlKiller)
		return;

	pPlKiller->m_Score += CFG(SacrScore);
	SendFreezeKill(Killer, Victim, WEAPON_NINJA);

	if (pPlKiller->GetCharacter() && CFG(SacrLoltext) && CFG(SacrScore))
	{
		char aBuf[64];
		str_format(aBuf, sizeof aBuf, "%+d", CFG(SacrScore));
		GS->CreateLolText(pPlKiller->GetCharacter(), false, vec2(0.f, -50.f), vec2(0.f, 0.f), 50, aBuf);
	}
}

void CGameControllerMOD::SendFreezeKill(int Killer, int Victim, int Weapon)
{
	char aBuf[256];
	str_format(aBuf, sizeof(aBuf), "frzkill k:%d:'%s' v:%d:'%s' w:%d",
	                      Killer, Server()->ClientName(Killer),
	                      Victim, Server()->ClientName(Victim), Weapon);

	GS->Console()->Print(IConsole::OUTPUT_LEVEL_DEBUG, "game", aBuf);

	// send the kill message
	CNetMsg_Sv_KillMsg Msg;
	Msg.m_Killer = Killer;
	Msg.m_Victim = Victim;
	Msg.m_Weapon = Weapon;
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
	m_aCltMask[pVictim->GetPlayer()->GetTeam()&1] |= (1<<pVictim->GetPlayer()->GetCID());
	
	IGameController::OnCharacterSpawn(pVictim);

	pVictim->TakeWeapon(WEAPON_GUN);
	pVictim->GiveWeapon(WEAPON_RIFLE, -1);
	pVictim->SetWeapon(WEAPON_RIFLE);

	int Cid = pVictim->GetPlayer()->GetCID();
	for(int i = 0; i < MAX_CLIENTS; ++i)
		if (m_aLastInteraction[i] == Cid)
			m_aLastInteraction[i] = -1;

	m_aLastInteraction[pVictim->GetPlayer()->GetCID()] = -1;
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
	m_aCltMask[pVictim->GetPlayer()->GetTeam()&1] &= ~(1<<pVictim->GetPlayer()->GetCID());

	//IGameController::OnCharacterDeath(pVictim, pKiller, Weapon);

	int Cid = pVictim->GetPlayer()->GetCID();
	if (Weapon == WEAPON_WORLD && pVictim->GetFreezeTicks() > 0 && m_aLastInteraction[Cid] != -1)
		HandleSacr(m_aLastInteraction[Cid], Cid);

	return 0;
}

void CGameControllerMOD::PostReset()
{
	IGameController::PostReset();

	for(int i = 0; i < MAX_CLIENTS; i++)
		m_aFrozenBy[i] = m_aMoltenBy[i] = m_aLastInteraction[i] = -1;

	m_NextBroadcast = 0;
	m_BroadcastStop = 0;
	m_aBroadcast[0] = '\0';

	m_aScoreDisplayCount[0] = m_aScoreDisplayCount[1] = 0;
	m_aScoreDisplayValue[0] = m_aScoreDisplayValue[1] = -1;
	
	for(int i = 0; i < MAX_SCOREDISPLAYS; i++)
		m_aScoreDisplayTextIDs[0][i] = m_aScoreDisplayTextIDs[1][i] = -1;

	InitScoreMarkers();
}

void CGameControllerMOD::InitScoreMarkers()
{
	m_aScoreDisplayCount[0] = m_aScoreDisplayCount[1] = 0;
	CMapItemLayerTilemap *pTMap = GS->Collision()->Layers()->GameLayer();
	CTile *pTiles = (CTile *)GS->Collision()->Layers()->Map()->GetData(pTMap->m_Data);
	for(int y = 0; y < pTMap->m_Height; y++)
		for(int x = 0; x < pTMap->m_Width; x++)
		{
			int Index = pTiles[y * pTMap->m_Width + x].m_Index;
			if (Index == TILE_REDSCORE || Index == TILE_BLUESCORE)
			{
				int Team = Index - TILE_REDSCORE;
				if (m_aScoreDisplayCount[Team] < MAX_SCOREDISPLAYS)
					m_aScoreDisplays[Team][m_aScoreDisplayCount[Team]++] = vec2(x*32.f, y*32.f);
				else
					dbg_msg("mod", "warning, too many score displays on map, ignoring this one.");
			}
				
		}
}

bool CGameControllerMOD::CanJoinTeam(int Team, int NotThisID)
{
	int Can = IGameController::CanJoinTeam(Team, NotThisID);
	if (!Can)
		return false;

	CCharacter *pChr = CHAR(NotThisID);

	return !pChr || pChr->GetFreezeTicks() <= 0;
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
