/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <game/server/entities/character.h>
#include "mod.h"

CGameControllerMOD::CGameControllerMOD(class CGameContext *pGameServer)
: IGameController(pGameServer)
{
	m_pGameType = "openfng";
	m_GameFlags = GAMEFLAG_TEAMS;
}

void CGameControllerMOD::Tick()
{
	DoTeamScoreWincheck();
	IGameController::Tick();
}
