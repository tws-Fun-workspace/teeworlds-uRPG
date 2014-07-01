/* (c) Shereef Marzouk. See "licence DDRace.txt" and the readme.txt in the root of the distribution for more information. */
#include "gamecontext.h"
#include <engine/shared/config.h>
#include <engine/server/server.h>
#include <game/server/teams.h>
#include <game/server/gamemodes/DDRace.h>
#include <game/version.h>
#include <game/generated/nethash.cpp>
#if defined(CONF_SQL)
#include <game/server/score/sql_score.h>
#endif

bool CheckClientID(int ClientID);
bool CheckRights(int ClientID, int Victim, CGameContext *GameContext);


bool CheckRights(int ClientID, int Victim, CGameContext *GameContext)
{
    if(!CheckClientID(ClientID)) return false;
    if(!CheckClientID(Victim)) return false;

    if (ClientID == Victim)
        return true;

    if (!GameContext->m_apPlayers[ClientID] || !GameContext->m_apPlayers[Victim])
        return false;

    if(GameContext->m_apPlayers[ClientID]->m_Authed <= GameContext->m_apPlayers[Victim]->m_Authed)
        return false;

    return true;
}
void CGameContext::ConGHammer(IConsole::IResult *pResult, void *pUserData)
{
    if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
    CGameContext *pSelf = (CGameContext *)pUserData;
    int Victim = pResult->GetVictim();

    CPlayer *pPlayer = pSelf->m_apPlayers[Victim];
    if(!pPlayer)
        return;

    CCharacter* pChr = pSelf->m_apPlayers[Victim]->GetCharacter();
    if (!pChr)
        return;

    pChr->m_gHammer = true;

}
void CGameContext::ConUnGHammer(IConsole::IResult *pResult, void *pUserData)
{
    if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
    CGameContext *pSelf = (CGameContext *)pUserData;
    int Victim = pResult->GetVictim();

    CPlayer *pPlayer = pSelf->m_apPlayers[Victim];
    if(!pPlayer)
        return;

    CCharacter* pChr = pSelf->m_apPlayers[Victim]->GetCharacter();
    if (!pChr)
        return;
    pChr->m_gHammer = false;
}
void CGameContext::ConDeepHammer(IConsole::IResult *pResult, void *pUserData)
{
	if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
	CGameContext *pSelf = (CGameContext *)pUserData;
	int Victim = pResult->GetVictim();

	CPlayer *pPlayer = pSelf->m_apPlayers[Victim];
	if(!pPlayer)
		return;

	CCharacter* pChr = pSelf->GetPlayerChar(pResult->m_ClientID);
	if (pChr && !pChr->m_Super)
	{
		pChr->m_Super = false;
	}
	pChr->m_DeepHammer = true;
    pChr->m_unDeepHammer = false;
}

void CGameContext::ConUnDeepHammer(IConsole::IResult *pResult, void *pUserData)
{
	if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
	CGameContext *pSelf = (CGameContext *)pUserData;
	int Victim = pResult->GetVictim();

	CPlayer *pPlayer = pSelf->m_apPlayers[Victim];
	if(!pPlayer)
		return;

	CCharacter* pChr = pSelf->m_apPlayers[Victim]->GetCharacter();
	if (!pChr)
		return;
    pChr->m_DeepHammer = false;
	pChr->m_unDeepHammer = true;
}
void CGameContext::ConIceHammer(IConsole::IResult *pResult, void *pUserData)
{
	if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
	CGameContext *pSelf = (CGameContext *)pUserData;
	int Victim = pResult->GetVictim();

	CPlayer *pPlayer = pSelf->m_apPlayers[Victim];
	if(!pPlayer)
		return;

	CCharacter* pChr = pSelf->m_apPlayers[Victim]->GetCharacter();
	if (!pChr)
		return;

	pChr->m_IceHammer = true;
}

void CGameContext::ConUnIceHammer(IConsole::IResult *pResult, void *pUserData)
{
	if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
	CGameContext *pSelf = (CGameContext *)pUserData;
	int Victim = pResult->GetVictim();

	CPlayer *pPlayer = pSelf->m_apPlayers[Victim];
	if(!pPlayer)
		return;

	CCharacter* pChr = pSelf->m_apPlayers[Victim]->GetCharacter();
	if (!pChr)
		return;

	pChr->m_IceHammer = false;
}
void CGameContext::ConRename(IConsole::IResult *pResult, void *pUserData)
{
	if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
	const char *newName = pResult->GetString(0);
	CGameContext *pSelf = (CGameContext *)pUserData;
	int Victim = pResult->GetVictim();

	CPlayer *pPlayer = pSelf->m_apPlayers[Victim];
	if(!pPlayer)
		return;

	CCharacter* pChr = pSelf->m_apPlayers[Victim]->GetCharacter();
	if(!pChr)
		return;

	//change name
	char oldName[MAX_NAME_LENGTH];
	str_copy(oldName, pSelf->Server()->ClientName(Victim), MAX_NAME_LENGTH);

	pSelf->Server()->SetClientName(Victim, newName);
	char aBuf[256];
	str_format(aBuf, sizeof(aBuf), "%s has changed %s's name to '%s'", pSelf->Server()->ClientName(pResult->m_ClientID), oldName, pSelf->Server()->ClientName(Victim));
	pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "info", aBuf);

	str_format(aBuf, sizeof(aBuf), "%s changed your name to %s.", pSelf->Server()->ClientName(pResult->m_ClientID), pSelf->Server()->ClientName(Victim));
	pSelf->SendChatTarget(Victim, aBuf);
}
void CGameContext::ConWhisper(IConsole::IResult *pResult, void *pUserData)
{
	// if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
	CGameContext *pSelf = (CGameContext *)pUserData;
	int Victim = pResult->GetVictim();
	CCharacter* pChr = pSelf->m_apPlayers[pResult->m_ClientID]->GetCharacter();
	if(!pChr) return;
    const char *message = pResult->GetString(0);


	char aBuf[256];
	str_format(aBuf, sizeof(aBuf), "Whisper from %s:   %s", pSelf->Server()->ClientName(pResult->m_ClientID), message);
	pSelf->SendChatTarget(Victim, aBuf);

	str_format(aBuf, sizeof(aBuf), "To: %s, Message: %s", pSelf->Server()->ClientName(Victim), message);
	pSelf->SendChatTarget(pResult->m_ClientID, aBuf);
}
void CGameContext::ConHammer(IConsole::IResult *pResult, void *pUserData)
{
    if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
	CGameContext *pSelf = (CGameContext *)pUserData;
	int Victim = pResult->GetVictim();
	if(!CheckRights(pResult->m_ClientID, Victim, (CGameContext *)pUserData)) return;

	char aBuf[128];
	int Type = pResult->GetInteger(0);

	CPlayer *pPlayer = pSelf->m_apPlayers[Victim];
	if(!pPlayer)
		return;

	CCharacter* pChr = pSelf->GetPlayerChar(Victim);
	if(!pChr)
		return;

	CServer* pServ = (CServer*)pSelf->Server();
	if(Type > 10 || Type < 0)
	{
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "info", "Select hammer between 0 and 10");
	}
	else
	{
		pChr->m_HammerType = Type;
		str_format(aBuf, sizeof(aBuf), "Hammer of '%s' ClientID=%d setted to %d", pServ->ClientName(Victim), Victim, Type);
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "info", aBuf);
	}
}
void CGameContext::ConInvis(IConsole::IResult *pResult, void *pUserData)
{
	if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
	CGameContext *pSelf = (CGameContext *)pUserData;
	char aBuf[128];
	int Victim = pResult->GetVictim();

	if(!pSelf->m_apPlayers[pResult->m_ClientID])
		return;

	if(pSelf->m_apPlayers[Victim])
	{
		pSelf->m_apPlayers[Victim]->m_Invisible = true;
		CServer* pServ = (CServer*)pSelf->Server();
		str_format(aBuf, sizeof(aBuf), "'%s' ClientID=%d is now invisible.", pServ->ClientName(Victim), Victim);
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "info", aBuf);
	}
}

void CGameContext::ConVis(IConsole::IResult *pResult, void *pUserData)
{
	if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
	CGameContext *pSelf = (CGameContext *)pUserData;
	int Victim = pResult->GetVictim();

	if(!pSelf->m_apPlayers[pResult->m_ClientID])
		return;
	char aBuf[128];
	if(pSelf->m_apPlayers[Victim])
	{
		pSelf->m_apPlayers[Victim]->m_Invisible = false;
		CServer* pServ = (CServer*)pSelf->Server();
		str_format(aBuf, sizeof(aBuf), "'%s' ClientID=%d is visible.", pServ->ClientName(Victim), Victim);
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "info", aBuf);
	}
}
void CGameContext::ConBlood(IConsole::IResult *pResult, void *pUserData)
{
	if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
	int Victim = pResult->GetVictim();

	CGameContext *pSelf = (CGameContext *)pUserData;
	CPlayer *pPlayer = pSelf->m_apPlayers[Victim];
	if(!pPlayer)
		return;

	CCharacter* pChr = pSelf->m_apPlayers[Victim]->GetCharacter();
	if(!pChr)
		return;

	char aBuf[128];
	if (!pChr->m_Bloody)
	{
		pChr->m_Bloody = true;

		str_format(aBuf, sizeof(aBuf), "You got bloody by %s.", pSelf->Server()->ClientName(pResult->m_ClientID));
		pSelf->SendChatTarget(Victim, aBuf);
	}
	else
	{
		pChr->m_Bloody = false;
		str_format(aBuf, sizeof(aBuf), "%s removed your blood.", pSelf->Server()->ClientName(pResult->m_ClientID));
		pSelf->SendChatTarget(Victim, aBuf);
	}
}

void CGameContext::ConGoLeft(IConsole::IResult *pResult, void *pUserData)
{
	if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
    int Victim = pResult->GetVictim();
    CGameContext *pSelf = (CGameContext *)pUserData;
	pSelf->MoveCharacter(Victim, -1, 0);
}

void CGameContext::ConGoRight(IConsole::IResult *pResult, void *pUserData)
{
	if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
    int Victim = pResult->GetVictim();
    CGameContext *pSelf = (CGameContext *)pUserData;
    pSelf->MoveCharacter(Victim, 1, 0);
}

void CGameContext::ConGoDown(IConsole::IResult *pResult, void *pUserData)
{
	if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
    int Victim = pResult->GetVictim();
    CGameContext *pSelf = (CGameContext *)pUserData;
    pSelf->MoveCharacter(Victim, 0, 1);
}

void CGameContext::ConGoUp(IConsole::IResult *pResult, void *pUserData)
{
	if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
    int Victim = pResult->GetVictim();
    CGameContext *pSelf = (CGameContext *)pUserData;
    pSelf->MoveCharacter(Victim, 0, -1);
}

void CGameContext::ConMove(IConsole::IResult *pResult, void *pUserData)
{
    if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
	CGameContext *pSelf = (CGameContext *) pUserData;
	if (!CheckClientID(pResult->m_ClientID))
		return;
    int Victim = pResult->GetVictim();
	pSelf->MoveCharacter(Victim, pResult->GetInteger(0),
			pResult->GetInteger(1));
}

void CGameContext::ConMoveRaw(IConsole::IResult *pResult, void *pUserData)
{
    if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
	CGameContext *pSelf = (CGameContext *) pUserData;
	if (!CheckClientID(pResult->m_ClientID))
		return;
	pSelf->MoveCharacter(pResult->m_ClientID, pResult->GetInteger(0),
			pResult->GetInteger(1), true);
}

void CGameContext::MoveCharacter(int ClientID, int X, int Y, bool Raw)
{
	CCharacter* pChr = GetPlayerChar(ClientID);

	if (!pChr)
		return;

	pChr->Core()->m_Pos.x += ((Raw) ? 1 : 32) * X;
	pChr->Core()->m_Pos.y += ((Raw) ? 1 : 32) * Y;
}

void CGameContext::ConKillPlayer(IConsole::IResult *pResult, void *pUserData)
{
    // if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
	CGameContext *pSelf = (CGameContext *) pUserData;
	if (!CheckClientID(pResult->m_ClientID))
		return;
	int Victim = pResult->GetVictim();

	if (pSelf->m_apPlayers[Victim])
	{
		pSelf->m_apPlayers[Victim]->KillCharacter(WEAPON_GAME);
		char aBuf[512];
		str_format(aBuf, sizeof(aBuf), "%s was killed by %s",
				pSelf->Server()->ClientName(Victim),
				pSelf->Server()->ClientName(pResult->m_ClientID));
		pSelf->SendChat(-1, CGameContext::CHAT_ALL, aBuf);
	}
    else {
        pSelf->m_apPlayers[Victim]->KillCharacter(WEAPON_GAME);
    }
}


void CGameContext::ConSuper(IConsole::IResult *pResult, void *pUserData)
{
	if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
    CGameContext *pSelf = (CGameContext *)pUserData;
    int Victim = pResult->GetVictim();

    CPlayer *pPlayer = pSelf->m_apPlayers[Victim];
    if(!pPlayer)
        return;

    CCharacter* pChr = pSelf->m_apPlayers[Victim]->GetCharacter();
    if(!pChr)
        return;

    char aBuf[128];
    if (!pChr->m_Super)
    {
        pChr->m_Super = true;
        pChr->UnFreeze();
    }
}

void CGameContext::ConUnSuper(IConsole::IResult *pResult, void *pUserData)
{
	if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
    CGameContext *pSelf = (CGameContext *)pUserData;
    int Victim = pResult->GetVictim();

    CPlayer *pPlayer = pSelf->m_apPlayers[Victim];
    if(!pPlayer)
        return;

    CCharacter* pChr = pSelf->m_apPlayers[Victim]->GetCharacter();
    if(!pChr)
        return;

    char aBuf[128];
    if (pChr->m_Super)
    {
        pChr->m_Super = false;
    }
}

void CGameContext::ConShotgun(IConsole::IResult *pResult, void *pUserData)
{
    if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
	CGameContext *pSelf = (CGameContext *) pUserData;
	pSelf->ModifyWeapons(pResult, pUserData, WEAPON_SHOTGUN, false);
}

void CGameContext::ConGrenade(IConsole::IResult *pResult, void *pUserData)
{
    if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
	CGameContext *pSelf = (CGameContext *) pUserData;
	pSelf->ModifyWeapons(pResult, pUserData, WEAPON_GRENADE, false);
}

void CGameContext::ConLaser(IConsole::IResult *pResult, void *pUserData)
{
    if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
    CGameContext *pSelf = (CGameContext *) pUserData;
    pSelf->ModifyWeapons(pResult, pUserData, WEAPON_RIFLE, false);
}
void CGameContext::ConNinja(IConsole::IResult *pResult, void *pUserData)
{
    if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
    CGameContext *pSelf = (CGameContext *) pUserData;
    pSelf->ModifyWeapons(pResult, pUserData, WEAPON_NINJA, false);
}
void CGameContext::ConWeapons(IConsole::IResult *pResult, void *pUserData)
{
    if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
    CGameContext *pSelf = (CGameContext *)pUserData;
    int Victim = pResult->GetVictim();

    CPlayer *pPlayer = pSelf->m_apPlayers[Victim];
    if(!pPlayer)
        return;

    CCharacter* pChr = pSelf->m_apPlayers[Victim]->GetCharacter();
    if(!pChr)
        return;

	pSelf->ModifyWeapons(pResult, pUserData, -1, false);
}

void CGameContext::ConUnShotgun(IConsole::IResult *pResult, void *pUserData)
{
    if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
	CGameContext *pSelf = (CGameContext *) pUserData;
	pSelf->ModifyWeapons(pResult, pUserData, WEAPON_SHOTGUN, true);
}

void CGameContext::ConUnGrenade(IConsole::IResult *pResult, void *pUserData)
{
    if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
	CGameContext *pSelf = (CGameContext *) pUserData;
	pSelf->ModifyWeapons(pResult, pUserData, WEAPON_GRENADE, true);
}

void CGameContext::ConUnLaser(IConsole::IResult *pResult, void *pUserData)
{
    if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
	CGameContext *pSelf = (CGameContext *) pUserData;
	pSelf->ModifyWeapons(pResult, pUserData, WEAPON_RIFLE, true);
}

void CGameContext::ConUnWeapons(IConsole::IResult *pResult, void *pUserData)
{
    if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
	CGameContext *pSelf = (CGameContext *) pUserData;
	pSelf->ModifyWeapons(pResult, pUserData, -1, true);
}

void CGameContext::ConAddWeapon(IConsole::IResult *pResult, void *pUserData)
{
    if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
	CGameContext *pSelf = (CGameContext *) pUserData;
	pSelf->ModifyWeapons(pResult, pUserData, pResult->GetInteger(0), false);
}

void CGameContext::ConRemoveWeapon(IConsole::IResult *pResult, void *pUserData)
{
    if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
	CGameContext *pSelf = (CGameContext *) pUserData;
	pSelf->ModifyWeapons(pResult, pUserData, pResult->GetInteger(0), true);
}
void CGameContext::ConToggleFly(IConsole::IResult *pResult, void *pUserData)
{
	if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
	CGameContext *pSelf = (CGameContext *)pUserData;
	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientID];
	if(!pPlayer)
		return;
	CCharacter* pChr = pPlayer->GetCharacter();
	if(!pChr)
		return;
	if(pChr->m_Super)
	{
		pChr->m_Fly = !pChr->m_Fly;
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "info", (pChr->m_Fly) ? "Fly enabled" : "Fly disabled");
	}
}
void CGameContext::ConRainbow(IConsole::IResult *pResult, void *pUserData)
{
	if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
	CGameContext *pSelf = (CGameContext *)pUserData;
	int Victim = pResult->GetVictim();
	int Rainbowtype = clamp(pResult->GetInteger(0), 0, 2);

	CPlayer *pPlayer = pSelf->m_apPlayers[Victim];
	if(!pPlayer)
		return;

	CCharacter* pChr = pSelf->m_apPlayers[Victim]->GetCharacter();
	if(!pChr)
		return;

	char aBuf[256];
	if ((pSelf->m_apPlayers[Victim]->m_Rainbow == RAINBOW_NONE || pSelf->m_apPlayers[Victim]->m_Rainbow == RAINBOW_BLACKWHITE) && Rainbowtype <= 1)
	{
		pSelf->m_apPlayers[Victim]->m_Rainbow = RAINBOW_COLOR;
		str_format(aBuf, sizeof(aBuf), "You got rainbow by %s.", pSelf->Server()->ClientName(pResult->m_ClientID));
		pSelf->SendChatTarget(Victim, aBuf);
	}
	else if ((pSelf->m_apPlayers[Victim]->m_Rainbow == RAINBOW_NONE || pSelf->m_apPlayers[Victim]->m_Rainbow == RAINBOW_COLOR) && Rainbowtype == 2)
	{
		pSelf->m_apPlayers[Victim]->m_Rainbow = RAINBOW_BLACKWHITE;
		str_format(aBuf, sizeof(aBuf), "You got black and white rainbow by %s.", pSelf->Server()->ClientName(pResult->m_ClientID));
		pSelf->SendChatTarget(Victim, aBuf);
	}
	else
	{
		pSelf->m_apPlayers[Victim]->m_Rainbow = RAINBOW_NONE;
		str_format(aBuf, sizeof(aBuf), "%s removed your rainbow.", pSelf->Server()->ClientName(pResult->m_ClientID));
		pSelf->SendChatTarget(Victim, aBuf);
	}
}
void CGameContext::ModifyWeapons(IConsole::IResult *pResult, void *pUserData, int Weapon, bool Remove) {
	CGameContext *pSelf = (CGameContext *) pUserData;
	if (!CheckClientID(pResult->m_ClientID))
		return;
    int ClientID = pResult->m_ClientID;
    int Victim = pResult->GetVictim();

    CPlayer *pPlayer = pSelf->m_apPlayers[Victim];
    if(!pPlayer)
        return;

    CCharacter* pChr = pSelf->m_apPlayers[Victim]->GetCharacter();
    if(!pChr)
        return;

	if (clamp(Weapon, -1, NUM_WEAPONS - 1) != Weapon)
	{
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "info",
				"invalid weapon id");
		return;
	}


	if (Weapon == -1)
	{
		if (Remove
				&& (pChr->GetActiveWeapon() == WEAPON_SHOTGUN
						|| pChr->GetActiveWeapon() == WEAPON_GRENADE
						|| pChr->GetActiveWeapon() == WEAPON_RIFLE))
			pChr->SetActiveWeapon(WEAPON_GUN);

		if (Remove)
		{
			pChr->SetWeaponGot(WEAPON_SHOTGUN, false);
			pChr->SetWeaponGot(WEAPON_GRENADE, false);
			pChr->SetWeaponGot(WEAPON_RIFLE, false);
		}
		else
			pChr->GiveAllWeapons();
	}
	else if (Weapon != WEAPON_NINJA)
	{
		if (Remove && pChr->GetActiveWeapon() == Weapon)
			pChr->SetActiveWeapon(WEAPON_GUN);

		if (Remove)
			pChr->SetWeaponGot(Weapon, false);
		else
			pChr->GiveWeapon(Weapon, -1);
	}
	else
	{
		if (Remove)
		{
			pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "info",
					"you can't remove ninja");
			return;
		}

		pChr->GiveNinja();
	}

}

void CGameContext::ConTeleport(IConsole::IResult *pResult, void *pUserData)
{
    if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
	CGameContext *pSelf = (CGameContext *) pUserData;
	int TeleTo = pResult->GetInteger(0);
    int Tele = pResult->GetVictim();

    if (!CheckRights(pResult->m_ClientID, Tele, (CGameContext *)pUserData))
        return;

    if (pSelf->m_apPlayers[TeleTo])
    {
        CCharacter* pChr = pSelf->GetPlayerChar(Tele);
        if (pChr && pSelf->GetPlayerChar(TeleTo))
        {
            pChr->Core()->m_Pos = pSelf->m_apPlayers[TeleTo]->m_ViewPos;
        }
    }
}

void CGameContext::ConKill(IConsole::IResult *pResult, void *pUserData)
{
    if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
	CGameContext *pSelf = (CGameContext *) pUserData;

	if (!CheckClientID(pResult->m_ClientID))
		return;

    int Victim = pResult->GetVictim();
	CPlayer *pPlayer = pSelf->m_apPlayers[Victim];

	if (!pPlayer
			|| (pPlayer->m_LastKill
					&& pPlayer->m_LastKill
					+ pSelf->Server()->TickSpeed()
					* g_Config.m_SvKillDelay
					> pSelf->Server()->Tick()))
		return;

	pPlayer->m_LastKill = pSelf->Server()->Tick();
	pPlayer->KillCharacter(WEAPON_SELF);
}

void CGameContext::ConForcePause(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	CServer* pServ = (CServer*)pSelf->Server();
	int Victim = pResult->GetVictim();
	int Seconds = 0;
	if (pResult->NumArguments() > 0)
		Seconds = clamp(pResult->GetInteger(0), 0, 999999);

	CPlayer *pPlayer = pSelf->m_apPlayers[Victim];
	if (!pPlayer)
		return;

	pPlayer->m_ForcePauseTime = Seconds*pServ->TickSpeed();
	pPlayer->m_Paused = CPlayer::PAUSED_FORCE;
}

void CGameContext::Mute(IConsole::IResult *pResult, NETADDR *Addr, int Secs,
		const char *pDisplayName)
{
	char aBuf[128];
	int Found = 0;
	// find a matching mute for this ip, update expiration time if found
	for (int i = 0; i < m_NumMutes; i++)
	{
		if (net_addr_comp(&m_aMutes[i].m_Addr, Addr) == 0)
		{
			m_aMutes[i].m_Expire = Server()->Tick()
							+ Secs * Server()->TickSpeed();
			Found = 1;
		}
	}

	if (!Found) // nothing found so far, find a free slot..
	{
		if (m_NumMutes < MAX_MUTES)
		{
			m_aMutes[m_NumMutes].m_Addr = *Addr;
			m_aMutes[m_NumMutes].m_Expire = Server()->Tick()
							+ Secs * Server()->TickSpeed();
			m_NumMutes++;
			Found = 1;
		}
	}
	if (Found)
	{
		str_format(aBuf, sizeof aBuf, "'%s' has been muted for %d seconds.",
				pDisplayName, Secs);
		SendChat(-1, CHAT_ALL, aBuf);
	}
	else // no free slot found
		Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "mutes", "mute array is full");
}

void CGameContext::ConMute(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *) pUserData;
	pSelf->Console()->Print(
			IConsole::OUTPUT_LEVEL_STANDARD,
			"mutes",
			"Use either 'muteid <client_id> <seconds>' or 'muteip <ip> <seconds>'");
}

// mute through client id
void CGameContext::ConMuteID(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *) pUserData;
	int Victim = pResult->GetVictim();

	NETADDR Addr;
	pSelf->Server()->GetClientAddr(Victim, &Addr);

	pSelf->Mute(pResult, &Addr, clamp(pResult->GetInteger(0), 1, 86400),
			pSelf->Server()->ClientName(Victim));
}

// mute through ip, arguments reversed to workaround parsing
void CGameContext::ConMuteIP(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *) pUserData;
	NETADDR Addr;
	if (net_addr_from_str(&Addr, pResult->GetString(0)))
	{
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "mutes",
				"Invalid network address to mute");
	}
	pSelf->Mute(pResult, &Addr, clamp(pResult->GetInteger(1), 1, 86400),
			pResult->GetString(0));
}

// unmute by mute list index
void CGameContext::ConUnmute(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *) pUserData;
	char aIpBuf[64];
	char aBuf[64];
	int Victim = pResult->GetVictim();

	if (Victim < 0 || Victim >= pSelf->m_NumMutes)
		return;

	pSelf->m_NumMutes--;
	pSelf->m_aMutes[Victim] = pSelf->m_aMutes[pSelf->m_NumMutes];

	net_addr_str(&pSelf->m_aMutes[Victim].m_Addr, aIpBuf, sizeof(aIpBuf), false);
	str_format(aBuf, sizeof(aBuf), "Unmuted %s", aIpBuf);
	pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "mutes", aBuf);
}

// list mutes
void CGameContext::ConMutes(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *) pUserData;
	char aIpBuf[64];
	char aBuf[128];
	pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "mutes",
			"Active mutes:");
	for (int i = 0; i < pSelf->m_NumMutes; i++)
	{
		net_addr_str(&pSelf->m_aMutes[i].m_Addr, aIpBuf, sizeof(aIpBuf), false);
		str_format(
				aBuf,
				sizeof aBuf,
				"%d: \"%s\", %d seconds left",
				i,
				aIpBuf,
				(pSelf->m_aMutes[i].m_Expire - pSelf->Server()->Tick())
				/ pSelf->Server()->TickSpeed());
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "mutes", aBuf);
	}
}

void CGameContext::ConList(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	int ClientID = pResult->m_ClientID;
	if(!CheckClientID(ClientID)) return;

	char zerochar = 0;
	if(pResult->NumArguments() > 0)
		pSelf->List(ClientID, pResult->GetString(0));
	else
		pSelf->List(ClientID, &zerochar);
}
void CGameContext::ConUnFastReload(IConsole::IResult *pResult, void *pUserData)
{
	if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
    CGameContext *pSelf = (CGameContext *)pUserData;
    int Victim = pResult->GetVictim();

    CPlayer *pPlayer = pSelf->m_apPlayers[Victim];
    if(!pPlayer)
        return;

    CCharacter* pChr = pSelf->m_apPlayers[Victim]->GetCharacter();
    if(!pChr)
        return;

    char aBuf[128];
    if (pChr->m_FastReload)
    {
        pChr->m_ReloadMultiplier = 1000;
        pChr->m_FastReload = false;
        str_format(aBuf, sizeof(aBuf), "%s removed your XXL.", pSelf->Server()->ClientName(pResult->m_ClientID));
        pSelf->SendChatTarget(Victim, aBuf);
    }
}
void CGameContext::ConFastReload(IConsole::IResult *pResult, void *pUserData)
{
	if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
	CGameContext *pSelf = (CGameContext *)pUserData;
	int Victim = pResult->GetVictim();

	CPlayer *pPlayer = pSelf->m_apPlayers[Victim];
	if(!pPlayer)
		return;

	CCharacter* pChr = pSelf->m_apPlayers[Victim]->GetCharacter();
	if(!pChr)
		return;

	char aBuf[128];
	if (!pChr->m_FastReload)
	{
		pChr->m_ReloadMultiplier = g_Config.m_SvXXLSpeed;
		pChr->m_FastReload = true;

		str_format(aBuf, sizeof(aBuf), "You got XXL by %s.", pSelf->Server()->ClientName(pResult->m_ClientID));
		pSelf->SendChatTarget(Victim, aBuf);
	}
}
void CGameContext::ConScore(IConsole::IResult *pResult, void *pUserData)
{
    if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
    int Victim = pResult->GetVictim();
    int Score = clamp(pResult->GetInteger(0), -9999, 9999);

    CGameContext *pSelf = (CGameContext *)pUserData;
    CPlayer *pPlayer = pSelf->m_apPlayers[Victim];
    if(!pPlayer)
        return;

    if (!g_Config.m_SvRconScore)
    {
        pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "info", "You can't set a score, if sv_rcon_score is not enabled.");
        return;
    }

    CCharacter* pChr = pSelf->m_apPlayers[Victim]->GetCharacter();;
    if(!pChr)
        return;

    pSelf->m_apPlayers[Victim]->m_Score = Score;

    char aBuf[256];
    str_format(aBuf, sizeof(aBuf), "%s set score of %s to %i" ,pSelf->Server()->ClientName(pResult->m_ClientID),pSelf->Server()->ClientName(Victim), Score);
    pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "info", aBuf);

    str_format(aBuf, sizeof(aBuf), "%s set your to %i.", pSelf->Server()->ClientName(pResult->m_ClientID), Score);
    pSelf->SendChatTarget(Victim, aBuf);
}
void CGameContext::ConHeXP(IConsole::IResult *pResult, void *pUserData)
{
    if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
    int Victim = pResult->GetVictim();

    CGameContext *pSelf = (CGameContext *)pUserData;
    CPlayer *pPlayer = pSelf->m_apPlayers[Victim];
    if(!pPlayer)
        return;

    CCharacter* pChr = pSelf->m_apPlayers[Victim]->GetCharacter();;
    if(!pChr)
        return;
    if(pChr->m_hexp <= 0) {
        pChr->m_hexp = 1;
        char aBuf[256];
        str_format(aBuf, sizeof(aBuf), "You got HeXP by %s.", pSelf->Server()->ClientName(pResult->m_ClientID));
        pSelf->SendChatTarget(Victim, aBuf);
    }
    else {
        pChr->m_hexp = 0;
        char aBuf[256];
        str_format(aBuf, sizeof(aBuf), "%s removed your HeXP", pSelf->Server()->ClientName(pResult->m_ClientID));
        pSelf->SendChatTarget(Victim, aBuf);
    }
}
void CGameContext::ConGuneXP(IConsole::IResult *pResult, void *pUserData)
{
    if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
    int Victim = pResult->GetVictim();

    CGameContext *pSelf = (CGameContext *)pUserData;
    CPlayer *pPlayer = pSelf->m_apPlayers[Victim];
    if(!pPlayer)
        return;

    CCharacter* pChr = pSelf->m_apPlayers[Victim]->GetCharacter();;
    if(!pChr)
        return;
    if(pChr->m_gunexp <= 0) {
        pChr->m_gunexp = 1;
        char aBuf[256];
        str_format(aBuf, sizeof(aBuf), "You got Gunexp by %s.", pSelf->Server()->ClientName(pResult->m_ClientID));
        pSelf->SendChatTarget(Victim, aBuf);
    }
    else {
        pChr->m_gunexp = 0;
        char aBuf[256];
        str_format(aBuf, sizeof(aBuf), "%s removed your Gunexp", pSelf->Server()->ClientName(pResult->m_ClientID));
        pSelf->SendChatTarget(Victim, aBuf);
    }
}
void CGameContext::ConSetJumps(IConsole::IResult *pResult, void *pUserData)
{
    if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
    CGameContext *pSelf = (CGameContext *)pUserData;
    int Victim = pResult->GetVictim();

    CPlayer *pPlayer = pSelf->m_apPlayers[Victim];
    if(!pPlayer)
        return;

    CCharacter *pChr = pSelf->m_apPlayers[Victim]->GetCharacter();
    if (!pChr)
        return;

    pChr->Core()->m_MaxJumps = clamp(pResult->GetInteger(0), 0, 9999);
}
void CGameContext::ConTHammer(IConsole::IResult *pResult, void *pUserData)
{
    if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
    CGameContext *pSelf = (CGameContext *)pUserData;
    int Victim = pResult->GetVictim();

    CPlayer *pPlayer = pSelf->m_apPlayers[Victim];
    if(!pPlayer)
        return;

    CCharacter* pChr = pSelf->m_apPlayers[Victim]->GetCharacter();
    if (!pChr)
        return;
    if(pChr->m_THammer) {
        pChr->m_THammer = false;
        char aBuf[256];
        str_format(aBuf, sizeof(aBuf), "%s removed your Tele-hammer", pSelf->Server()->ClientName(pResult->m_ClientID));
        pSelf->SendChatTarget(Victim, aBuf);
    }
    else {
        pChr->m_THammer = true;
        char aBuf[256];
        str_format(aBuf, sizeof(aBuf), "You got Tele-hammer by %s", pSelf->Server()->ClientName(pResult->m_ClientID));
        pSelf->SendChatTarget(Victim, aBuf);
    }

}
void CGameContext::ConColorHammer(IConsole::IResult *pResult, void *pUserData)
{
    if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
    CGameContext *pSelf = (CGameContext *)pUserData;
    int Victim = pResult->GetVictim();

    CPlayer *pPlayer = pSelf->m_apPlayers[Victim];
    if(!pPlayer)
        return;

    CCharacter* pChr = pSelf->m_apPlayers[Victim]->GetCharacter();
    if(!pChr)
        return;

    if(!pChr->m_cHammer)
        pChr->m_cHammer = true;
    else
        pChr->m_cHammer = false;
    char aBuf[256];
    str_format(aBuf, sizeof(aBuf), "Colors activated");
    pSelf->SendChatTarget(Victim, aBuf);
}
void CGameContext::ConUnColorHammer(IConsole::IResult *pResult, void *pUserData)
{
    if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
    CGameContext *pSelf = (CGameContext *)pUserData;
    int Victim = pResult->GetVictim();

    CPlayer *pPlayer = pSelf->m_apPlayers[Victim];
    if(!pPlayer)
        return;

    CCharacter* pChr = pSelf->m_apPlayers[Victim]->GetCharacter();
    if(!pChr)
        return;

    if(!pChr->m_cHammer)
        pChr->m_cHammer = true;
    else
        pChr->m_cHammer = false;
    char aBuf[256];
    str_format(aBuf, sizeof(aBuf), "Colors deactivated");
    pSelf->SendChatTarget(Victim, aBuf);
}

void CGameContext::ConPlasmaLaser(IConsole::IResult *pResult, void *pUserData)
{
    if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
    CGameContext *pSelf = (CGameContext *)pUserData;
    int Victim = pResult->GetVictim();

    CPlayer *pPlayer = pSelf->m_apPlayers[Victim];
    if(!pPlayer)
        return;
    CCharacter* pChr = pSelf->m_apPlayers[Victim]->GetCharacter();
    if(!pChr)
        return;

    int isFreeze = pResult->GetInteger(0) != false? true : false;
    int isExplos = pResult->GetInteger(1) != false? true : false;

    if(!pChr->m_pLaser) {
        pChr->m_pLaser = true;
        char aBuf[256];
        str_format(aBuf, sizeof(aBuf), "Plasma-Laser activated");
        pSelf->SendChatTarget(Victim, aBuf);
    }
    pChr->m_isFreeze = isFreeze;
    pChr->m_isExplos = isExplos;

        // char aBuf[128];
        // str_format(aBuf, sizeof(aBuf), "Freeze = %d, Explo = %d.", isFreeze, isExplos);
        // pSelf->SendChatTarget(Victim, aBuf);
}
void CGameContext::ConUnPlasmaLaser(IConsole::IResult *pResult, void *pUserData)
{
    if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
    CGameContext *pSelf = (CGameContext *)pUserData;
    int Victim = pResult->GetVictim();

    CPlayer *pPlayer = pSelf->m_apPlayers[Victim];
    if(!pPlayer)
        return;
    CCharacter* pChr = pSelf->m_apPlayers[Victim]->GetCharacter();
    if(!pChr)
        return;

    if(pChr->m_pLaser) {
        pChr->m_pLaser = false;
        pChr->m_isFreeze = false;
        pChr->m_isExplos = false;
        char aBuf[64];
        str_format(aBuf, sizeof(aBuf), "Plasma-Laser deactivated");
        pSelf->SendChatTarget(Victim, aBuf);
    }
}
void CGameContext::ConGrenadeBounce(IConsole::IResult *pResult, void *pUserData)
{
    if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
    CGameContext *pSelf = (CGameContext *)pUserData;
    int Victim = pResult->GetVictim();

    CPlayer *pPlayer = pSelf->m_apPlayers[Victim];
    if(!pPlayer)
        return;
    CCharacter* pChr = pSelf->m_apPlayers[Victim]->GetCharacter();
    if(!pChr)
        return;
    char aBuf[64];


    if(!pChr->m_gBounce) {
        pChr->m_gBounce = true;
        str_format(aBuf, sizeof(aBuf), "You have bounce grenade");
    }
    else {
        pChr->m_gBounce = false;
        str_format(aBuf, sizeof(aBuf), "Removed your grenade bounce");
    }
    pSelf->SendChatTarget(Victim, aBuf);
}
void CGameContext::ConGunSpread(IConsole::IResult *pResult, void *pUserData) {
    if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
    CGameContext *pSelf = (CGameContext *)pUserData;
    int Victim = pResult->GetVictim();
    int CountOfBullets = pResult->GetInteger(0);

    CPlayer *pPlayer = pSelf->m_apPlayers[Victim];
    if(!pPlayer)
        return;
    CCharacter* pChr = pSelf->m_apPlayers[Victim]->GetCharacter();
    if(!pChr)
        return;

    if(pChr) {
        pChr->m_SpreadGun = CountOfBullets;
    }
}
void CGameContext::ConShotgunSpread(IConsole::IResult *pResult, void *pUserData) {
    if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
    CGameContext *pSelf = (CGameContext *)pUserData;
    int Victim = pResult->GetVictim();
    int CountOfBullets = pResult->GetInteger(0);

    CPlayer *pPlayer = pSelf->m_apPlayers[Victim];
    if(!pPlayer)
        return;
    CCharacter* pChr = pSelf->m_apPlayers[Victim]->GetCharacter();
    if(!pChr)
        return;

    if(pChr) {
        pChr->m_SpreadShotgun = CountOfBullets;
    }
}
void CGameContext::ConGrenadeSpread(IConsole::IResult *pResult, void *pUserData) {
    if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
    CGameContext *pSelf = (CGameContext *)pUserData;
    int Victim = pResult->GetVictim();
    int CountOfBullets = pResult->GetInteger(0);

    CPlayer *pPlayer = pSelf->m_apPlayers[Victim];
    if(!pPlayer)
        return;
    CCharacter* pChr = pSelf->m_apPlayers[Victim]->GetCharacter();
    if(!pChr)
        return;

    if(pChr) {
        pChr->m_SpreadGrenade = CountOfBullets;
    }
}
void CGameContext::ConLaserSpread(IConsole::IResult *pResult, void *pUserData) {
    if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
    CGameContext *pSelf = (CGameContext *)pUserData;
    int Victim = pResult->GetVictim();
    int CountOfBullets = pResult->GetInteger(0);

    CPlayer *pPlayer = pSelf->m_apPlayers[Victim];
    if(!pPlayer)
        return;
    CCharacter* pChr = pSelf->m_apPlayers[Victim]->GetCharacter();
    if(!pChr)
        return;

    if(pChr) {
        pChr->m_SpreadLaser = CountOfBullets;
    }
}
/*
                                    m_SpreadGun
                                    m_SpreadShotgun
                                    m_SpreadGrenade
                                    m_SpreadLaser
*/
void CGameContext::ConShotgunWall(IConsole::IResult *pResult, void *pUserData)
{
    if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
    CGameContext *pSelf = (CGameContext *)pUserData;
    int Victim = pResult->GetVictim();

    CPlayer *pPlayer = pSelf->m_apPlayers[Victim];
    if(!pPlayer)
        return;

    CCharacter* pChr = pSelf->m_apPlayers[Victim]->GetCharacter();
    if (!pChr)
        return;
    if(pChr->m_shotgunWall) {
        pChr->m_shotgunWall = false;
        char aBuf[64];
        str_format(aBuf, sizeof(aBuf), "Shotgun Walls deactivated");
        pSelf->SendChatTarget(Victim, aBuf);
    }
    else {
        pChr->m_shotgunWall = true;
        char aBuf[64];
        str_format(aBuf, sizeof(aBuf), "Shotgun Walls activated");
        pSelf->SendChatTarget(Victim, aBuf);
    }
}

void CGameContext::ConGiveKid(IConsole::IResult *pResult, void *pUserData) {
    if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
    CGameContext *pSelf = (CGameContext *)pUserData;
    int Victim = pResult->GetVictim();

    CPlayer *pPlayer = pSelf->m_apPlayers[Victim];
    if(!pPlayer)
        return;

    CCharacter* pChr = pSelf->m_apPlayers[Victim]->GetCharacter();
    if (!pChr)
        return;

    int Level = 1;
    CServer* pServer = (CServer*)pSelf->Server();
    if(pSelf->m_apPlayers[Victim] && Victim != pResult->m_ClientID)
        pServer->SetRconLevel(Victim, 1);
    char aBuf[64];
    str_format(aBuf, sizeof(aBuf), "You are now Kid");
    pSelf->SendChatTarget(Victim, aBuf);
}
void CGameContext::ConGiveHelper(IConsole::IResult *pResult, void *pUserData) {
    if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
    CGameContext *pSelf = (CGameContext *)pUserData;
    int Victim = pResult->GetVictim();

    CPlayer *pPlayer = pSelf->m_apPlayers[Victim];
    if(!pPlayer)
        return;

    CCharacter* pChr = pSelf->m_apPlayers[Victim]->GetCharacter();
    if (!pChr)
        return;


    int Level = 2;
    CServer* pServer = (CServer*)pSelf->Server();
    if(pSelf->m_apPlayers[Victim] && Victim != pResult->m_ClientID)
        pServer->SetRconLevel(Victim, Level);
    char aBuf[64];
    str_format(aBuf, sizeof(aBuf), "You are now Helper");
    pSelf->SendChatTarget(Victim, aBuf);

}
void CGameContext::ConGiveModer(IConsole::IResult *pResult, void *pUserData) {
    if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
    CGameContext *pSelf = (CGameContext *)pUserData;
    int Victim = pResult->GetVictim();

    CPlayer *pPlayer = pSelf->m_apPlayers[Victim];
    if(!pPlayer)
        return;

    CCharacter* pChr = pSelf->m_apPlayers[Victim]->GetCharacter();
    if (!pChr)
        return;


    int Level = 3;
    CServer* pServer = (CServer*)pSelf->Server();
    if(pSelf->m_apPlayers[Victim] && Victim != pResult->m_ClientID)
        pServer->SetRconLevel(Victim, Level);
    char aBuf[64];
    str_format(aBuf, sizeof(aBuf), "You are now Moderator");
    pSelf->SendChatTarget(Victim, aBuf);

}
void CGameContext::ConRemoveLevel(IConsole::IResult *pResult, void *pUserData) {
    if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
    CGameContext *pSelf = (CGameContext *)pUserData;
    int Victim = pResult->GetVictim();

    CPlayer *pPlayer = pSelf->m_apPlayers[Victim];
    if(!pPlayer)
        return;

    CCharacter* pChr = pSelf->m_apPlayers[Victim]->GetCharacter();
    if (!pChr)
        return;


    int Level = 0;
    CServer* pServer = (CServer*)pSelf->Server();
    if(pSelf->m_apPlayers[Victim] && Victim != pResult->m_ClientID)
        pServer->SetRconLevel(Victim, Level);
    char aBuf[64];
    str_format(aBuf, sizeof(aBuf), "%s Removed your Level", pSelf->Server()->ClientName(pResult->m_ClientID));
    pSelf->SendChatTarget(Victim, aBuf);
}
void CGameContext::ConUnFreezePlayer(IConsole::IResult *pResult, void *pUserData) {
    if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
    CGameContext *pSelf = (CGameContext *)pUserData;
    int Victim = pResult->GetVictim();
    int ClientID = pResult->m_ClientID;

    CPlayer *pPlayer = pSelf->m_apPlayers[Victim];
    if(!pPlayer)
        return;

    CCharacter* pChr = pSelf->m_apPlayers[Victim]->GetCharacter();
    if (!pChr)
        return;


    static bool Warning = false;
    char aBuf[128];

    if(pChr->m_DeepFreeze && !Warning)
    {
        pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "warning", "This client is deeply frozen, repeat the command to defrost him.");
        Warning = true;
        return;
    }
    if(pChr->m_DeepFreeze && Warning)
    {
        pChr->m_DeepFreeze = false;
        Warning = false;
    }
    pChr->m_FreezeTime = 2;
    pChr->GetPlayer()->m_RconFreeze = false;
    CServer* pServ = (CServer*)pSelf->Server();
    str_format(aBuf, sizeof(aBuf), "'%s' ClientID=%d has been defrosted.", pServ->ClientName(Victim), Victim);
    pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "info", aBuf);
}
void CGameContext::ConFreezePlayer(IConsole::IResult *pResult, void *pUserData) {
    if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
    CGameContext *pSelf = (CGameContext *)pUserData;
    int Victim = pResult->GetVictim();
    int ClientID = pResult->m_ClientID;

    CPlayer *pPlayer = pSelf->m_apPlayers[Victim];
    if(!pPlayer)
        return;

    CCharacter* pChr = pSelf->m_apPlayers[Victim]->GetCharacter();
    if (!pChr)
        return;

    char aBuf[128];

    int Seconds = -1;

    if(pResult->NumArguments() == 1)
        Seconds = clamp(pResult->GetInteger(0), -2, 9999);

    if(pSelf->m_apPlayers[Victim])
    {
        pChr->Freeze(Seconds);
        pChr->GetPlayer()->m_RconFreeze = Seconds != -2;
        CServer* pServ = (CServer*)pSelf->Server();
        if(Seconds >= 0)
            str_format(aBuf, sizeof(aBuf), "'%s' ClientID=%d has been Frozen for %d.", pServ->ClientName(Victim), Victim, Seconds);
        else if(Seconds == -2)
        {
            pChr->m_DeepFreeze = true;
            str_format(aBuf, sizeof(aBuf), "'%s' ClientID=%d has been Deep Frozen.", pServ->ClientName(Victim), Victim);
        }
        else
            str_format(aBuf, sizeof(aBuf), "'%s' ClientID=%d is Frozen until you unfreeze him.", pServ->ClientName(Victim), Victim);
        pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "info", aBuf);
    }
}

void CGameContext::ConEHammer(IConsole::IResult *pResult, void *pUserData)
{
    if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
    CGameContext *pSelf = (CGameContext *)pUserData;
    int Victim = pResult->GetVictim();

    CPlayer *pPlayer = pSelf->m_apPlayers[Victim];
    if(!pPlayer)
        return;

    CCharacter* pChr = pSelf->m_apPlayers[Victim]->GetCharacter();
    if (!pChr)
        return;
    if(pChr->m_EHammer) {
        pChr->m_EHammer = false;
        char aBuf[256];
        str_format(aBuf, sizeof(aBuf), "%s removed your Explosion hammer", pSelf->Server()->ClientName(pResult->m_ClientID));
        pSelf->SendChatTarget(Victim, aBuf);
    }
    else {
        pChr->m_EHammer = true;
        char aBuf[256];
        str_format(aBuf, sizeof(aBuf), "You got Explosion hammer by %s", pSelf->Server()->ClientName(pResult->m_ClientID));
        pSelf->SendChatTarget(Victim, aBuf);
    }

}
void CGameContext::ConRemRise(IConsole::IResult *pResult, void *pUserData) {
    if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
    CGameContext *pSelf = (CGameContext *)pUserData;
    int Victim = pResult->GetVictim();
    int ClientID = pResult->m_ClientID;


    CPlayer *pPlayer = pSelf->m_apPlayers[Victim];
    if(!pPlayer)
        return;

    CCharacter* pChr = pSelf->m_apPlayers[Victim]->GetCharacter();
    if (!pChr)
        return;

    pChr->m_EHammer = false;
    pChr->m_gHammer = false;
    pChr->m_THammer = false;
    pChr->m_cHammer = false;
    pChr->m_hexp = 0;
    pChr->m_SpreadGun = false;
    pChr->m_SpreadShotgun = false;
    pChr->m_SpreadGrenade = false;
    pChr->m_gBounce = false;
    pChr->m_pLaser = false;
    pChr->m_SpreadLaser = false;
    pChr->m_FastReload = false;
    pChr->m_ReloadMultiplier = 1000;
    pChr->m_Super = false;
    pChr->m_HammerType = 0;
    pChr->m_Bloody = false;
    pSelf->m_apPlayers[Victim]->m_Rainbow = false;
    pSelf->m_apPlayers[Victim]->m_Invisible = false;
}
void CGameContext::ConPlayAs(IConsole::IResult *pResult, void *pUserData) {
    if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
    CGameContext *pSelf = (CGameContext *)pUserData;
    int Victim = pResult->GetVictim();
    int ClientID = pResult->m_ClientID;

    if(Victim == ClientID) {
        pSelf->SendChatTarget(ClientID, "You can't use this command on yourself");
        return;
    }
    CPlayer *pPlayer = pSelf->m_apPlayers[Victim];
    if(!pPlayer)
        return;

    CCharacter* pMe  = pSelf->m_apPlayers[ClientID]->GetCharacter();
    CCharacter* pChr = pSelf->m_apPlayers[Victim]->GetCharacter();
    if (!pChr || !pMe)
        return;


    if(pMe->m_playAsId) {
        CCharacter* pChrA = pSelf->m_apPlayers[pMe->m_playAsId]->GetCharacter();
        if(pChrA) {
            pChrA->m_isUnderControl = false;
            pMe->m_playAsId = false;
            pSelf->SendChatTarget(ClientID, "PlayAs disabled");
        }
        return;
    }
    else {
        pMe->m_playAsId = Victim;
        pChr->m_isUnderControl = true;
        char aBuf[128];
        str_format(aBuf, sizeof(aBuf), "You are playing as %s", pSelf->Server()->ClientName(Victim));   
        pSelf->SendChatTarget(ClientID, aBuf);
    }
}

void CGameContext::ConCanKill(IConsole::IResult *pResult, void *pUserData)
{
    if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
    CGameContext *pSelf = (CGameContext *)pUserData;
    int Victim = pResult->GetVictim();

    CPlayer *pPlayer = pSelf->m_apPlayers[Victim];
    if(!pPlayer)
        return;

    CCharacter* pChr = pSelf->m_apPlayers[Victim]->GetCharacter();
    if (!pChr)
        return;

    if(pChr->isCanKill) {
        pChr->isCanKill = false;
        char aBuf[256];
        str_format(aBuf, sizeof(aBuf), "You can't kill players");
        pSelf->SendChatTarget(Victim, aBuf);
    }
    else {
        pChr->isCanKill = true;
        char aBuf[256];
        str_format(aBuf, sizeof(aBuf), "You can kill players");
        pSelf->SendChatTarget(Victim, aBuf);
    }

}









/*
// Template :


void CGameContext::ConBlaBla(IConsole::IResult *pResult, void *pUserData) {
    if(!CheckRights(pResult->m_ClientID, pResult->GetVictim(), (CGameContext *)pUserData)) return;
    CGameContext *pSelf = (CGameContext *)pUserData;
    int Victim = pResult->GetVictim();
    int ClientID = pResult->m_ClientID;

    // int Integer = pResult->GetInteger(0); 0 - first integer
    // const char *String = pResult->GetString(0); 0 - first !string!

    CPlayer *pPlayer = pSelf->m_apPlayers[Victim];
    if(!pPlayer)
        return;

    CCharacter* pChr = pSelf->m_apPlayers[Victim]->GetCharacter();
    if (!pChr)
        return;
}

*/

