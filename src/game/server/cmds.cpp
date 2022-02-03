/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <stdio.h>
#include <string.h>

#include <engine/server.h>
#include <game/version.h>
#include "cmds.h"
#include "account.h"
#include "account.h"

CCmd::CCmd(CPlayer *pPlayer, CGameContext *pGameServer)
{
	m_pPlayer = pPlayer;
	m_pGameServer = pGameServer;
}

void CCmd::ChatCmd(CNetMsg_Cl_Say *Msg)
{
		if(!strncmp(Msg->m_pMessage, "/login", 6))
		{
			LastChat();
			char Username[512];
			char Password[512];
			if(sscanf(Msg->m_pMessage, "/login %s %s", Username, Password) != 2)
			{
				GameServer()->SendChatTarget(m_pPlayer->GetCID(), "格式错误, 请输入: '/login <用户名> <密码>'");
				return;
			}
			m_pPlayer->m_pAccount->Login(Username, Password);
			return;
		}
		else if(!str_comp_nocase(Msg->m_pMessage, "/logout"))
		{
			LastChat();

			if(!m_pPlayer->m_AccData.m_UserID)
			{
				GameServer()->SendChatTarget(m_pPlayer->GetCID(), "你还没登录呢");
				return;
			}
			m_pPlayer->m_pAccount->Apply();
			m_pPlayer->m_pAccount->Reset();	

			GameServer()->SendChatTarget(m_pPlayer->GetCID(), "退出登录成功");

			CCharacter *pOwner = GameServer()->GetPlayerChar(m_pPlayer->GetCID());

			if(pOwner)
			{
				if(pOwner->IsAlive())
					pOwner->Die(m_pPlayer->GetCID(), WEAPON_GAME);
			}

			return;
        }
		else if(!strncmp(Msg->m_pMessage, "/register", 9))
		{
			LastChat();
			char Username[512];
			char Password[512];
			if(sscanf(Msg->m_pMessage, "/register %s %s", Username, Password) != 2)
			{
				GameServer()->SendChatTarget(m_pPlayer->GetCID(), "格式错误，请使用： '/register <用户名> <密码>'");
				return;
			}
			m_pPlayer->m_pAccount->Register(Username, Password);
			return;
		}
		else if(!str_comp_nocase(Msg->m_pMessage, "/info"))
  		{
			LastChat();
			GameServer()->SendChatTarget(m_pPlayer->GetCID(), "\nuTown_v2.0 by Pikotee & KlickFoot");
			GameServer()->SendChatTarget(m_pPlayer->GetCID(), "\nDon't trust Blunk(Torben Weiss) and QuickTee/r00t they're stealing mods...");
			return;
   		}
		else if(!str_comp_nocase(Msg->m_pMessage, "/cmdlist"))
		{
			LastChat();
			char aBuf[512];
			str_format(aBuf, sizeof(aBuf), "=========");
			GameServer()->SendMotd(m_pPlayer->GetCID(), aBuf);

			return;
        }
		if(!strncmp(Msg->m_pMessage, "/", 1))
		{
			LastChat();
			GameServer()->SendChatTarget(m_pPlayer->GetCID(), "Wrong CMD, see /cmdlist");
		}

}

void CCmd::LastChat()
{
	 m_pPlayer->m_LastChat = GameServer()->Server()->Tick();
}
