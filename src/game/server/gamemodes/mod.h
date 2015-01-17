/* (c) Timo Buhrmester. See licence.txt in the root of the distribution   */
/* for more information. If you are missing that file, acquire a complete */
/* release at teeworlds.com.                                              */

#ifndef GAME_SERVER_GAMEMODES_MOD_H
#define GAME_SERVER_GAMEMODES_MOD_H

#include <game/server/gamecontroller.h>

#define MAX_BROADCAST 256

class CGameControllerMOD : public IGameController
{
private:
	int m_aFrozenBy[MAX_CLIENTS];
	int m_aMoltenBy[MAX_CLIENTS];
	char m_aBroadcast[MAX_BROADCAST];
	int m_NextBroadcast;
	int m_BroadcastStop;

	int m_aCltMask[2]; //for sending damageindicators only to teammates

	void SendFreezeKill(int Killer, int Victim, int Weapon);
	void HandleFreeze(int Killer, int Victim);
	void HandleMelt(int Melter, int Meltee);
	void HandleSacr(int Killer, int Victim);

	void Broadcast(const char *pMsg, int Ticks);
public:
	CGameControllerMOD(class CGameContext *pGameServer);
	virtual ~CGameControllerMOD();

	//virtual bool CanBeMovedOnBalance(int ClientID);
	virtual void Tick();
	virtual void Snap(int SnappingClient);
	virtual bool OnEntity(int Index, vec2 Pos);
	virtual void OnCharacterSpawn(class CCharacter *pChr);
	virtual int OnCharacterDeath(class CCharacter *pVictim,
	                          class CPlayer *pUnusedKiller, int Weapon);
	//virtual void OnPlayerInfoChange(class CPlayer *pP);
	//virtual bool CanSpawn(int Team, vec2 *pPos);
	//virtual const char *GetTeamName(int Team);
	//virtual int GetAutoTeam(int NotThisID);
	virtual bool CanJoinTeam(int Team, int NotThisID);
	virtual void PostReset();
};
#endif

/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_SERVER_GAMEMODES_MOD_H
#define GAME_SERVER_GAMEMODES_MOD_H
#include <game/server/gamecontroller.h>

// you can subclass GAMECONTROLLER_CTF, GAMECONTROLLER_TDM etc if you want
// todo a modification with their base as well.
class CGameControllerMOD : public IGameController
{
public:
	CGameControllerMOD(class CGameContext *pGameServer);
	virtual void Tick();
	// add more virtual functions here if you wish
};
#endif
