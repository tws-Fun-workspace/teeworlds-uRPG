/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include "mod.h"
#include <game/server/gamecontroller.h>

CGameControllerMOD::CGameControllerMOD(class CGameContext *pGameServer)
: IGameController(pGameServer)
{
	// Exchange this to a string that identifies your game mode.
	// DM, TDM and CTF are reserved for teeworlds original modes.
	m_pGameType = "uRPG";

	//m_GameFlags = GAMEFLAG_FLAGS; // GAMEFLAG_TEAMS makes it a two-team gamemode
}

void CGameControllerMOD::Tick()
{
	// this is the main part of the gamemode, this function is run every tick
	//DoPlayerScoreWincheck(); // checks for winners, no teams version
	//DoTeamScoreWincheck(); // checks for winners, two teams version

	IGameController::Tick();
}
