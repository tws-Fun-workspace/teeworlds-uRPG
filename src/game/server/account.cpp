/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */

#include <string.h>
#include <fstream>
#include <engine/config.h>
#include "account.h"
//#include "game/server/gamecontext.h"

#if defined(CONF_FAMILY_WINDOWS)
	#include <tchar.h>
	#include <direct.h>
#endif
#if defined(CONF_FAMILY_UNIX)
	#include <sys/types.h>
	#include <fcntl.h>
	#include <unistd.h>
#endif

int CAccount::PlayerLevelUp()
{
	if(m_pPlayer->m_AccData.m_UserID)
	{
		bool upgraded = false;
		long int NeedExp = m_pPlayer->m_AccData.m_Level*GetNeedForUp();
		while(m_pPlayer->m_AccData.m_ExpPoints >= NeedExp)
		{
			upgraded = true;
			m_pPlayer->m_AccData.m_ExpPoints -= m_pPlayer->m_AccData.m_Level*GetNeedForUp();
			NeedExp = m_pPlayer->m_AccData.m_Level*GetNeedForUp();
			//m_pPlayer->m_AccData.m_Money += 1000;
			if(m_pPlayer->m_AccData.m_Level == 2)
				GameServer()->SendChatTarget(m_pPlayer->GetCID(), "FIRST STEP!.");		
		}
		if(upgraded)
		{
			GameServer()->SendChatTarget(m_pPlayer->GetCID(), "[Level UP] 恭喜你, 你升级了!");
			GameServer()->CreateSoundGlobal(SOUND_CTF_CAPTURE, m_pPlayer->GetCID());
			m_pPlayer->m_AccData.m_Level++;
			m_pPlayer->m_AccData.m_Money += GetPlayerLevel()*100;
		}
	}
}

int CAccount::GetNeedForUp()
{
}

int CAccount::GetPlayerLevel()
{
	return m_pPlayer->m_AccData.m_Level;
}

int CAccount::GetPlayerExp()
{
	return m_pPlayer->m_AccData.m_ExpPoints;
}


CAccount::CAccount(CPlayer *pPlayer, CGameContext *pGameServer)
{
   m_pPlayer = pPlayer;
   m_pGameServer = pGameServer;
}

/*
#ifndef GAME_VERSION_H
#define GAME_VERSION_H
#ifndef NON_HASED_VERSION
#include "generated/nethash.cpp"
#define GAME_VERSION "0.6.1"
#define GAME_NETVERSION "0.6 626fce9a778df4d4" //the std game version
#endif
#endif
*/

void CAccount::Login(char *Username, char *Password)
{
	char aBuf[125];
	if(m_pPlayer->m_AccData.m_UserID)
	{
		dbg_msg("account", "Account login failed ('%s' - Already logged in)", Username);
		GameServer()->SendChatTarget(m_pPlayer->GetCID(), "您当前已经登录了一个账号");
		return;
	}
	else if(strlen(Username) > 15 || !strlen(Username))
	{
		str_format(aBuf, sizeof(aBuf), "您设置的用户名太%s了", strlen(Username)?"长":"短");
		GameServer()->SendChatTarget(m_pPlayer->GetCID(), aBuf);
		return;
    }
	else if(strlen(Password) > 15 || !strlen(Password))
	{
		str_format(aBuf, sizeof(aBuf), "您设置的密码太%s了!", strlen(Password)?"长":"短");
		GameServer()->SendChatTarget(m_pPlayer->GetCID(), aBuf);
		return;
    }
	else if(!Exists(Username))
	{
		dbg_msg("account", "Account login failed ('%s' - Missing)", Username);
		GameServer()->SendChatTarget(m_pPlayer->GetCID(), "不存在此用户名的账号，请检查是否输入正确");
		GameServer()->SendChatTarget(m_pPlayer->GetCID(), "如果还没有注册账号，请输入以下指令注册 (/register <用户名> <密码>)，不需要输入<>");
		return;
	}

	str_format(aBuf, sizeof(aBuf), "accounts/+%s.acc", Username);

	char AccUsername[32];
	char AccPassword[32];
	char AccRcon[32];
	int AccID;


 
	FILE *Accfile;
	Accfile = fopen(aBuf, "r");
	fscanf(Accfile, "%s\n%s\n%s\n%d", AccUsername, AccPassword, AccRcon, &AccID);
	fclose(Accfile);

	

	for(int i = 0; i < MAX_SERVER; i++)
	{
		for(int j = 0; j < MAX_CLIENTS; j++)
		{
			if(GameServer()->m_apPlayers[j] && GameServer()->m_apPlayers[j]->m_AccData.m_UserID == AccID)
			{
				dbg_msg("account", "Account login failed ('%s' - already in use (local))", Username);
				GameServer()->SendChatTarget(m_pPlayer->GetCID(), "此账号正在使用中");
				return;
			}

			if(!GameServer()->m_aaExtIDs[i][j])
				continue;

			if(AccID == GameServer()->m_aaExtIDs[i][j])
			{
				dbg_msg("account", "Account login failed ('%s' - already in use (extern))", Username);
				GameServer()->SendChatTarget(m_pPlayer->GetCID(), "此账号正在使用中");
				return;
			}
		}
	}

	if(strcmp(Username, AccUsername))
	{
		dbg_msg("account", "Account login failed ('%s' - Wrong username)", Username);
		GameServer()->SendChatTarget(m_pPlayer->GetCID(), "错误的用户名或密码");
		return;
	}

	if(strcmp(Password, AccPassword))
	{
		dbg_msg("account", "Account login failed ('%s' - Wrong password)", Username);
		GameServer()->SendChatTarget(m_pPlayer->GetCID(), "错误的用户名或密码");
		return;
	}


	Accfile = fopen(aBuf, "r"); 		

	fscanf(Accfile, "%s\n%s\n%s\n%d\n\n%d\n%d\n%d\n\n%d\n%d\n\n%d\n%d\n\nAdd",
		m_pPlayer->m_AccData.m_Username, // Done
		m_pPlayer->m_AccData.m_Password, // Done
		m_pPlayer->m_AccData.m_RconPassword,
		&m_pPlayer->m_AccData.m_UserID, // Done

 		&m_pPlayer->m_AccData.m_Money, // Done
		&m_pPlayer->m_AccData.m_Health, // Done
		&m_pPlayer->m_AccData.m_Armor, // Done

		&m_pPlayer->m_AccData.m_Donor, 
		&m_pPlayer->m_AccData.m_VIP, // Done

		&m_pPlayer->m_AccData.m_Level,
		&m_pPlayer->m_AccData.m_ExpPoints); 

	fclose(Accfile);

	CCharacter *pOwner = GameServer()->GetPlayerChar(m_pPlayer->GetCID());
	 
	if(m_pPlayer->GetTeam() == TEAM_SPECTATORS)
		m_pPlayer->SetTeam(0);
  	
	dbg_msg("account", "Account login sucessful ('%s')", Username);
	GameServer()->SendChatTarget(m_pPlayer->GetCID(), "登录成功！");
}

void CAccount::Register(char *Username, char *Password)
{
	char aBuf[125];
	if(m_pPlayer->m_AccData.m_UserID)
	{
		dbg_msg("account", "Account registration failed ('%s' - Logged in)", Username);
		GameServer()->SendChatTarget(m_pPlayer->GetCID(), "您当前已经登录了一个账号，不可重复登录");
		return;
	}
	if(strlen(Username) > 15 || !strlen(Username))
	{
		str_format(aBuf, sizeof(aBuf), "您设置的用户名太%s了", strlen(Username)?"长":"短");
		GameServer()->SendChatTarget(m_pPlayer->GetCID(), aBuf);
		return;
    }
	else if(strlen(Password) > 15 || !strlen(Password))
	{
		str_format(aBuf, sizeof(aBuf), "您设置的密码太%s了!", strlen(Password)?"长":"短");
		GameServer()->SendChatTarget(m_pPlayer->GetCID(), aBuf);
		return;
    }
	else if(Exists(Username))
	{
		dbg_msg("account", "Account registration failed ('%s' - Already exists)", Username);
		GameServer()->SendChatTarget(m_pPlayer->GetCID(), "账号已存在");
		return;
	}

	#if defined(CONF_FAMILY_UNIX)
	char Filter[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz.-_";
	// "äöü<>|!§$%&/()=?`´*'#+~«»¢“”æßðđŋħjĸł˝;,·^°@ł€¶ŧ←↓→øþ\\";
	char *p = strpbrk(Username, Filter);
	if(!p)
	{
		GameServer()->SendChatTarget(m_pPlayer->GetCID(), "请不要使用中文等特殊字符作为用户名！");
		GameServer()->SendChatTarget(m_pPlayer->GetCID(), "仅支持A到Z, a到z, 0到9, .到_");
		return;
	}

	#endif

	#if defined(CONF_FAMILY_WINDOWS)
	static TCHAR * ValidChars = _T("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz.-_");
	if (_tcsspnp(Username, ValidChars))
	{
		GameServer()->SendChatTarget(m_pPlayer->GetCID(), "Don't use invalid chars for username!");
		GameServer()->SendChatTarget(m_pPlayer->GetCID(), "A - Z, a - z, 0 - 9, . - _");
		return;
	}

	if(mkdir("accounts"))
		dbg_msg("account", "Account folder created!");
	#endif

	str_format(aBuf, sizeof(aBuf), "accounts/+%s.acc", Username);

	FILE *Accfile;
	Accfile = fopen(aBuf, "a+");

	str_format(aBuf, sizeof(aBuf), "%s\n%s\n%s\n%d\n\n%d\n%d\n%d\n\n%d\n%d\n\n%d\n%d\n\nAdd", 
		Username, 
		Password, 
		"0",
		NextID(),

		m_pPlayer->m_AccData.m_Money,
		m_pPlayer->m_AccData.m_Health,
		m_pPlayer->m_AccData.m_Armor, 

		m_pPlayer->m_AccData.m_Donor,
		m_pPlayer->m_AccData.m_VIP,

		m_pPlayer->m_AccData.m_Level,
		m_pPlayer->m_AccData.m_ExpPoints);

	fputs(aBuf, Accfile);
	fclose(Accfile);

	dbg_msg("account", "Registration succesful ('%s')", Username);
	str_format(aBuf, sizeof(aBuf), "账号注册成功！下次进入时输入以下指令登录 - ('/login %s %s'): ", Username, Password);
	GameServer()->SendChatTarget(m_pPlayer->GetCID(), aBuf);
	Login(Username, Password);
}
bool CAccount::Exists(const char *Username)
{
	char aBuf[128];
	str_format(aBuf, sizeof(aBuf), "accounts/+%s.acc", Username);
    if(FILE *Accfile = fopen(aBuf, "r"))
    {
        fclose(Accfile);
        return true;
    }
    return false;
}

void CAccount::Apply()
{
	char aBuf[512];
	str_format(aBuf, sizeof(aBuf), "accounts/+%s.acc", m_pPlayer->m_AccData.m_Username);
	std::remove(aBuf);
	FILE *Accfile;
	Accfile = fopen(aBuf,"a+");
	
	str_format(aBuf, sizeof(aBuf), "%s\n%s\n%s\n%d\n\n%d\n%d\n%d\n\n%d\n%d\n\n%d\n%d\n\nAdd", 
		m_pPlayer->m_AccData.m_Username,
		m_pPlayer->m_AccData.m_Password, 
		m_pPlayer->m_AccData.m_RconPassword, 
		m_pPlayer->m_AccData.m_UserID,

		m_pPlayer->m_AccData.m_Money,
		m_pPlayer->m_AccData.m_Health,
		m_pPlayer->m_AccData.m_Armor,

		m_pPlayer->m_AccData.m_Donor,
		m_pPlayer->m_AccData.m_VIP,

		m_pPlayer->m_AccData.m_Level,
		m_pPlayer->m_AccData.m_ExpPoints);

	fputs(aBuf, Accfile);
	fclose(Accfile);
}

void CAccount::Reset()
{
	str_copy(m_pPlayer->m_AccData.m_Username, "", 32);
	str_copy(m_pPlayer->m_AccData.m_Password, "", 32);
	str_copy(m_pPlayer->m_AccData.m_RconPassword, "", 32);
	m_pPlayer->m_AccData.m_UserID = 0;
	
	m_pPlayer->m_AccData.m_Money = 0;
	m_pPlayer->m_AccData.m_Health = 0;
	m_pPlayer->m_AccData.m_Armor = 0;

	m_pPlayer->m_AccData.m_Level = 1;
	m_pPlayer->m_AccData.m_ExpPoints = 0;

}

void CAccount::Delete()
{
	char aBuf[128];
	if(m_pPlayer->m_AccData.m_UserID)
	{
		Reset();
		str_format(aBuf, sizeof(aBuf), "accounts/+%s.acc", m_pPlayer->m_AccData.m_Username);
		std::remove(aBuf);
		dbg_msg("account", "Account deleted ('%s')", m_pPlayer->m_AccData.m_Username);
		GameServer()->SendChatTarget(m_pPlayer->GetCID(), "账号成功删除！");
	}
	else
		GameServer()->SendChatTarget(m_pPlayer->GetCID(), "抱歉，您得先登录才能删除账号");
}

void CAccount::NewPassword(char *NewPassword)
{
	char aBuf[128];
	if(!m_pPlayer->m_AccData.m_UserID)
	{
		GameServer()->SendChatTarget(m_pPlayer->GetCID(), "抱歉，您得先登录才能修改账号密码");
		return;
	}
	if(strlen(NewPassword) > 15 || !strlen(NewPassword))
	{
		str_format(aBuf, sizeof(aBuf), "新密码太%s了！", strlen(NewPassword)?"长":"短");
		GameServer()->SendChatTarget(m_pPlayer->GetCID(), aBuf);
		return;
    }

	str_copy(m_pPlayer->m_AccData.m_Password, NewPassword, 32);
	Apply();

	
	dbg_msg("account", "Password changed - ('%s')", m_pPlayer->m_AccData.m_Username);
	GameServer()->SendChatTarget(m_pPlayer->GetCID(), "密码成功更改！");
}

void CAccount::NewUsername(char *NewUsername)
{
	char aBuf[128];
	if(!m_pPlayer->m_AccData.m_UserID)
	{
		GameServer()->SendChatTarget(m_pPlayer->GetCID(), "抱歉，您得先登录才能修改用户名");
		return;
	}
	if(strlen(NewUsername) > 15 || !strlen(NewUsername))
	{
		str_format(aBuf, sizeof(aBuf), "用户名太%s了！", strlen(NewUsername)?"长":"短");
		GameServer()->SendChatTarget(m_pPlayer->GetCID(), aBuf);
		return;
    }

	str_format(aBuf, sizeof(aBuf), "accounts/+%s.acc", m_pPlayer->m_AccData.m_Username);
	std::rename(aBuf, NewUsername);

	str_copy(m_pPlayer->m_AccData.m_Username, NewUsername, 32);
	Apply();

	
	dbg_msg("account", "Username changed - ('%s')", m_pPlayer->m_AccData.m_Username);
	GameServer()->SendChatTarget(m_pPlayer->GetCID(), "用户名已成功更改！");
}

int CAccount::NextID()
{
	FILE *Accfile;
	int UserID = 1;
	char aBuf[32];
	char AccUserID[32];

	str_copy(AccUserID, "accounts/++UserIDs++.acc", sizeof(AccUserID));

	if(Exists("+UserIDs++"))
	{
		Accfile = fopen(AccUserID, "r");
		fscanf(Accfile, "%d", &UserID);
		fclose(Accfile);

		std::remove(AccUserID);

		Accfile = fopen(AccUserID, "a+");
		str_format(aBuf, sizeof(aBuf), "%d", UserID+1);
		fputs(aBuf, Accfile);
		fclose(Accfile);

		return UserID+1;
	}
	else
	{
		Accfile = fopen(AccUserID, "a+");
		str_format(aBuf, sizeof(aBuf), "%d", UserID);
		fputs(aBuf, Accfile);
		fclose(Accfile);
	}

	return 1;
}
