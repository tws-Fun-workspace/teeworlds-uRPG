/* (c) Shereef Marzouk. See "licence DDRace.txt" and the readme.txt in the root of the distribution for more information. */
#ifndef GAME_SERVER_DDRACECOMMANDS_H
#define GAME_SERVER_DDRACECOMMANDS_H
#undef GAME_SERVER_DDRACECOMMANDS_H // this file can be included several times

#ifndef CONSOLE_COMMAND
#define CONSOLE_COMMAND(name, params, flags, callback, userdata, help, level)
#endif

CONSOLE_COMMAND("kill", "?v", CFGFLAG_SERVER, ConKillPlayer, this, "Kills player v and announces the kill", IConsole::CONSOLELEVEL_ADMIN)
CONSOLE_COMMAND("logout", "?v", CFGFLAG_SERVER, ConLogOut, this, "Logs player v out from the console", IConsole::CONSOLELEVEL_USER)
CONSOLE_COMMAND("moder", "v", CFGFLAG_SERVER, ConSetlvl1, this, "Authenticates player v to the level of 1", IConsole::CONSOLELEVEL_ADMIN)
CONSOLE_COMMAND("admin", "v", CFGFLAG_SERVER, ConSetlvl3, this, "Authenticates player v to the level of 3", IConsole::CONSOLELEVEL_CONFIG)
CONSOLE_COMMAND("invis", "v", CFGFLAG_SERVER|CMDFLAG_CHEAT, ConInvis, this, "Makes player v invisible", IConsole::CONSOLELEVEL_ADMIN)
CONSOLE_COMMAND("vis", "v", CFGFLAG_SERVER|CMDFLAG_CHEAT, ConVis, this, "Makes player v visible again", IConsole::CONSOLELEVEL_ADMIN)
CONSOLE_COMMAND("tele", "vi", CFGFLAG_SERVER|CMDFLAG_CHEAT, ConTeleport, this, "Teleports player v to player i", IConsole::CONSOLELEVEL_HELPER)
CONSOLE_COMMAND("freeze", "v?i", CFGFLAG_SERVER|CMDFLAG_CHEAT, ConFreeze, this, "Freezes player v for i seconds (infinite by default)", IConsole::CONSOLELEVEL_ADMIN)
CONSOLE_COMMAND("unfreeze", "v", CFGFLAG_SERVER|CMDFLAG_CHEAT, ConUnFreeze, this, "Unfreezes player v", IConsole::CONSOLELEVEL_ADMIN)
CONSOLE_COMMAND("addweapon", "v?i", CFGFLAG_SERVER|CMDFLAG_CHEAT, ConAddWeapon, this, "Gives weapon with id i to player v (all = -1, hammer = 0, gun = 1, shotgun = 2, grenade = 3, rifle = 4, ninja = 5)", 1)
CONSOLE_COMMAND("removeweapon", "v?i", CFGFLAG_SERVER|CMDFLAG_CHEAT, ConRemoveWeapon, this, "removes weapon with id i from player v (all = -1, hammer = 0, gun = 1, shotgun = 2, grenade = 3, rifle = 4)", 1)
CONSOLE_COMMAND("shotgun", "v", CFGFLAG_SERVER|CMDFLAG_CHEAT, ConShotgun, this, "Gives a shotgun to player v", IConsole::CONSOLELEVEL_ADMIN)
CONSOLE_COMMAND("grenade", "v", CFGFLAG_SERVER|CMDFLAG_CHEAT, ConGrenade, this, "Gives a grenade launcher to player v", IConsole::CONSOLELEVEL_ADMIN)
CONSOLE_COMMAND("rifle", "v", CFGFLAG_SERVER|CMDFLAG_CHEAT, ConRifle, this, "Gives a rifle to player v", IConsole::CONSOLELEVEL_ADMIN)
CONSOLE_COMMAND("weapons", "v", CFGFLAG_SERVER|CMDFLAG_CHEAT, ConWeapons, this, "Gives all weapons to player v", IConsole::CONSOLELEVEL_ADMIN)
CONSOLE_COMMAND("unshotgun", "v", CFGFLAG_SERVER, ConUnShotgun, this, "Takes the shotgun from player v", IConsole::CONSOLELEVEL_ADMIN)
CONSOLE_COMMAND("ungrenade", "v", CFGFLAG_SERVER, ConUnGrenade, this, "Takes the grenade launcher from player v", IConsole::CONSOLELEVEL_ADMIN)
CONSOLE_COMMAND("unrifle", "v", CFGFLAG_SERVER, ConUnRifle, this, "Takes the rifle from player v", IConsole::CONSOLELEVEL_ADMIN)
CONSOLE_COMMAND("unweapons", "v", CFGFLAG_SERVER, ConUnWeapons, this, "Takes all weapons from player v", IConsole::CONSOLELEVEL_ADMIN)
CONSOLE_COMMAND("ninja", "v", CFGFLAG_SERVER|CMDFLAG_CHEAT, ConNinja, this, "Makes player v a ninja", IConsole::CONSOLELEVEL_ADMIN)
CONSOLE_COMMAND("super", "v", CFGFLAG_SERVER|CMDFLAG_CHEAT, ConSuper, this, "Makes player v super", IConsole::CONSOLELEVEL_ADMIN)
CONSOLE_COMMAND("unsuper", "v", CFGFLAG_SERVER, ConUnSuper, this, "Removes super from player v", IConsole::CONSOLELEVEL_ADMIN)
CONSOLE_COMMAND("left", "?v", CFGFLAG_SERVER|CMDFLAG_CHEAT, ConGoLeft, this, "Makes you or player v move 1 tile left", IConsole::CONSOLELEVEL_HELPER)
CONSOLE_COMMAND("right", "?v", CFGFLAG_SERVER|CMDFLAG_CHEAT, ConGoRight, this, "Makes you or player v move 1 tile right", IConsole::CONSOLELEVEL_HELPER)
CONSOLE_COMMAND("up", "?v", CFGFLAG_SERVER|CMDFLAG_CHEAT, ConGoUp, this, "Makes you or player v move 1 tile up", IConsole::CONSOLELEVEL_HELPER)
CONSOLE_COMMAND("down", "?v", CFGFLAG_SERVER|CMDFLAG_CHEAT, ConGoDown, this, "Makes you or player v move 1 tile down", IConsole::CONSOLELEVEL_HELPER)
CONSOLE_COMMAND("move", "vii", CFGFLAG_SERVER|CMDFLAG_CHEAT, ConMove, this, "Moves player i to the tile with x/y-number ii", IConsole::CONSOLELEVEL_HELPER)
CONSOLE_COMMAND("move_raw", "vii", CFGFLAG_SERVER|CMDFLAG_CHEAT, ConMoveRaw, this, "Moves player i to the point with x/y-coordinates ii",IConsole::CONSOLELEVEL_HELPER)
CONSOLE_COMMAND("credits", "", CFGFLAG_SERVER, ConCredits, this, "Shows the credits of the DDRace mod", IConsole::CONSOLELEVEL_USER)
CONSOLE_COMMAND("emote", "?si", CFGFLAG_SERVER, ConEyeEmote, this, "Sets your tee's eye emote", IConsole::CONSOLELEVEL_USER)
CONSOLE_COMMAND("broadmsg", "", CFGFLAG_SERVER, ConToggleBroadcast, this, "Toggles showing the server's broadcast message during race on/off", IConsole::CONSOLELEVEL_USER)
CONSOLE_COMMAND("eyeemote", "", CFGFLAG_SERVER, ConEyeEmote, this, "Toggles use of standard eye-emotes on/off", IConsole::CONSOLELEVEL_USER)
CONSOLE_COMMAND("settings", "?s", CFGFLAG_SERVER, ConSettings, this, "Shows gameplay information for this server", IConsole::CONSOLELEVEL_USER)
CONSOLE_COMMAND("help", "?r", CFGFLAG_SERVER, ConHelp, this, "Shows help to command r, general help if left blank", IConsole::CONSOLELEVEL_USER)
CONSOLE_COMMAND("info", "", CFGFLAG_SERVER, ConInfo, this, "Shows info about this server", IConsole::CONSOLELEVEL_USER)
CONSOLE_COMMAND("me", "r", CFGFLAG_SERVER, ConMe, this, "Like the famous irc command '/me says hi' will display '<yourname> says hi'", IConsole::CONSOLELEVEL_USER)
CONSOLE_COMMAND("pause", "", CFGFLAG_SERVER, ConTogglePause, this, "Toggles pause on/off (if activated on server)", IConsole::CONSOLELEVEL_USER)
CONSOLE_COMMAND("rank", "?r", CFGFLAG_SERVER, ConRank, this, "Shows the rank of player with name r (your rank by default)", IConsole::CONSOLELEVEL_USER)
CONSOLE_COMMAND("rules", "", CFGFLAG_SERVER, ConRules, this, "Shows the server rules", IConsole::CONSOLELEVEL_USER)
CONSOLE_COMMAND("team", "?i", CFGFLAG_SERVER, ConJoinTeam, this, "Lets you join team i (shows your team if left blank)", IConsole::CONSOLELEVEL_USER)
CONSOLE_COMMAND("top5", "?i", CFGFLAG_SERVER, ConTop5, this, "Shows five ranks of the ladder beginning with rank i (1 by default)", IConsole::CONSOLELEVEL_USER)
CONSOLE_COMMAND("showothers", "?i", CFGFLAG_SERVER, ConShowOthers, this, "Whether to showplayers from other teams or not (off by default), optional i = 0 for off else for on", IConsole::CONSOLELEVEL_USER)

CONSOLE_COMMAND("mute", "", CFGFLAG_SERVER, ConMute, this, "", IConsole::CONSOLELEVEL_ADMIN);
CONSOLE_COMMAND("muteid", "vi", CFGFLAG_SERVER, ConMuteID, this, "", IConsole::CONSOLELEVEL_ADMIN);
CONSOLE_COMMAND("muteip", "si", CFGFLAG_SERVER, ConMuteIP, this, "", IConsole::CONSOLELEVEL_ADMIN);
CONSOLE_COMMAND("unmute", "i", CFGFLAG_SERVER, ConUnmute, this, "", IConsole::CONSOLELEVEL_ADMIN);
CONSOLE_COMMAND("mutes", "", CFGFLAG_SERVER, ConMutes, this, "", IConsole::CONSOLELEVEL_ADMIN);

//jDDRace
CONSOLE_COMMAND("jumps", "", CFGFLAG_SERVER, ConJumps, this, "Shows your jumps", IConsole::CONSOLELEVEL_USER)
CONSOLE_COMMAND("setjumps", "vi", CFGFLAG_SERVER|CMDFLAG_CHEAT, ConSetJumps, this, "Gives player v i jumps", IConsole::CONSOLELEVEL_ADMIN)
//XXLDDRace TODO: remove crap like score/renacme
CONSOLE_COMMAND("skin", "vs", CFGFLAG_SERVER, ConSkin, this, "Changes the skin from i in s", IConsole::CONSOLELEVEL_CONFIG)
CONSOLE_COMMAND("rename", "vr", CFGFLAG_SERVER, ConRename, this, "Renames i name to s", IConsole::CONSOLELEVEL_CONFIG)
CONSOLE_COMMAND("orgname", "v", CFGFLAG_SERVER, ConOrgname, this, "Renames i back to his orginal name", IConsole::CONSOLELEVEL_CONFIG) //nottworking
CONSOLE_COMMAND("fake", "vr", CFGFLAG_SERVER, ConFakeMessage, this, "Sends a fake message s from i", IConsole::CONSOLELEVEL_CONFIG)
CONSOLE_COMMAND("xxl", "v", CFGFLAG_SERVER|CMDFLAG_CHEAT, ConFastReload, this, "Fast reload :-)", IConsole::CONSOLELEVEL_CONFIG)
CONSOLE_COMMAND("rainbow", "v?i", CFGFLAG_SERVER, ConRainbow, this, "Colorchange the tee i like a rainbow", IConsole::CONSOLELEVEL_ADMIN)
CONSOLE_COMMAND("whisper", "vr", CFGFLAG_SERVER, ConWhisper, this, "Whispers r to player v", IConsole::CONSOLELEVEL_HELPER)
CONSOLE_COMMAND("w", "vr", CFGFLAG_SERVER, ConWhisper, this, "Whispers r to player v", IConsole::CONSOLELEVEL_HELPER)
CONSOLE_COMMAND("helper", "?v", CFGFLAG_SERVER, ConHelper, this, "Calls a helper./Authenticates player v to the level of 1", IConsole::CONSOLELEVEL_USER)
CONSOLE_COMMAND("score", "vi", CFGFLAG_SERVER, ConScore, this, "Changes the score of i to i", IConsole::CONSOLELEVEL_CONFIG)
CONSOLE_COMMAND("bloody", "v", CFGFLAG_SERVER, ConBlood, this, "BLOOD Ouch?!", IConsole::CONSOLELEVEL_CONFIG)
CONSOLE_COMMAND("test", "?vr", CFGFLAG_SERVER, ConTest, this, "TEST (Dont use this, it can crash your server!!!!!)", IConsole::CONSOLELEVEL_CONFIG)
CONSOLE_COMMAND("rescue", "", CFGFLAG_SERVER, ConRescue, this, "Rescue from freeze", IConsole::CONSOLELEVEL_USER)
CONSOLE_COMMAND("r", "", CFGFLAG_SERVER, ConRescue, this, "Rescue from freeze", IConsole::CONSOLELEVEL_USER)
CONSOLE_COMMAND("register", "r", CFGFLAG_SERVER, ConRegister, this, "Rescue from freeze", IConsole::CONSOLELEVEL_USER)
CONSOLE_COMMAND("login", "r", CFGFLAG_SERVER, ConLogin, this, "Rescue from freeze", IConsole::CONSOLELEVEL_USER)
CONSOLE_COMMAND("member", "v", CFGFLAG_SERVER, ConMember, this, "Sets v to member", IConsole::CONSOLELEVEL_ADMIN)
CONSOLE_COMMAND("unmember", "v", CFGFLAG_SERVER, ConUnMember, this, "Unsets v to member", IConsole::CONSOLELEVEL_ADMIN)
CONSOLE_COMMAND("checkmember", "v", CFGFLAG_SERVER, ConCheckMember, this, "Checks if v is a member", IConsole::CONSOLELEVEL_MODERATOR)
CONSOLE_COMMAND("icehammer", "v", CFGFLAG_SERVER, ConIceHammer, this, "The hammer freezes a tee", IConsole::CONSOLELEVEL_ADMIN)
CONSOLE_COMMAND("unicehammer", "v", CFGFLAG_SERVER, ConUnIceHammer, this, "The hammer gets normal, it unfreezes a tee", IConsole::CONSOLELEVEL_ADMIN)
CONSOLE_COMMAND("sadmin", "v", CFGFLAG_SERVER, ConSetlvl4, this, "Authenticates player v to the level of 4 (CAUTION: Irreversible, once he is a superadmin you can't remove his status)", IConsole::CONSOLELEVEL_CONFIG)
CONSOLE_COMMAND("moder", "v", CFGFLAG_SERVER, ConSetlvl2, this, "Authenticates player v to the level of 2", IConsole::CONSOLELEVEL_ADMIN)
CONSOLE_COMMAND("fly", "", CFGFLAG_SERVER, ConToggleFly, this, "Toggles super-fly (holding space) on/off", IConsole::CONSOLELEVEL_HELPER)


#if defined(CONF_SQL)
CONSOLE_COMMAND("times", "?s?i", CFGFLAG_SERVER, ConTimes, this, "/times ?s?i shows last 5 times of the server or of a player beginning with name s starting with time i (i = 1 by default)", IConsole::CONSOLELEVEL_USER)
#endif
#undef CONSOLE_COMMAND

#endif
