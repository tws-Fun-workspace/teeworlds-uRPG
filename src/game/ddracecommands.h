/* (c) Shereef Marzouk. See "licence DDRace.txt" and the readme.txt in the root of the distribution for more information. */
#ifndef GAME_SERVER_DDRACECOMMANDS_H
#define GAME_SERVER_DDRACECOMMANDS_H
#undef GAME_SERVER_DDRACECOMMANDS_H // this file can be included several times

#ifndef CONSOLE_COMMAND
#define CONSOLE_COMMAND(name, params, flags, callback, userdata, help)
#endif

CONSOLE_COMMAND("kill_pl", "i", CFGFLAG_SERVER, ConKillPlayer, this, "Kills player i and announces the kill")
CONSOLE_COMMAND("tele", "i", CFGFLAG_SERVER|CMDFLAG_TEST, ConTeleport, this, "Teleports you to player i")
CONSOLE_COMMAND("addweapon", "i", CFGFLAG_SERVER|CMDFLAG_TEST, ConAddWeapon, this, "Gives weapon with id i to you (all = -1, hammer = 0, gun = 1, shotgun = 2, grenade = 3, rifle = 4, ninja = 5)")
CONSOLE_COMMAND("removeweapon", "i", CFGFLAG_SERVER|CMDFLAG_TEST, ConRemoveWeapon, this, "removes weapon with id i from you (all = -1, hammer = 0, gun = 1, shotgun = 2, grenade = 3, rifle = 4)")
CONSOLE_COMMAND("shotgun", "", CFGFLAG_SERVER|CMDFLAG_TEST, ConShotgun, this, "Gives a shotgun to you")
CONSOLE_COMMAND("grenade", "", CFGFLAG_SERVER|CMDFLAG_TEST, ConGrenade, this, "Gives a grenade launcher to you")
CONSOLE_COMMAND("rifle", "", CFGFLAG_SERVER|CMDFLAG_TEST, ConRifle, this, "Gives a rifle to you")
CONSOLE_COMMAND("weapons", "", CFGFLAG_SERVER|CMDFLAG_TEST, ConWeapons, this, "Gives all weapons to you")
CONSOLE_COMMAND("unshotgun", "", CFGFLAG_SERVER|CMDFLAG_TEST, ConUnShotgun, this, "Takes the shotgun from you")
CONSOLE_COMMAND("ungrenade", "", CFGFLAG_SERVER|CMDFLAG_TEST, ConUnGrenade, this, "Takes the grenade launcher you")
CONSOLE_COMMAND("unrifle", "", CFGFLAG_SERVER|CMDFLAG_TEST, ConUnRifle, this, "Takes the rifle from you")
CONSOLE_COMMAND("unweapons", "", CFGFLAG_SERVER|CMDFLAG_TEST, ConUnWeapons, this, "Takes all weapons from you")
CONSOLE_COMMAND("ninja", "", CFGFLAG_SERVER|CMDFLAG_TEST, ConNinja, this, "Makes you a ninja")
CONSOLE_COMMAND("super", "", CFGFLAG_SERVER|CMDFLAG_TEST, ConSuper, this, "Makes you super")
CONSOLE_COMMAND("unsuper", "", CFGFLAG_SERVER|CMDFLAG_TEST, ConUnSuper, this, "Removes super from you")
CONSOLE_COMMAND("left", "", CFGFLAG_SERVER|CMDFLAG_TEST, ConGoLeft, this, "Makes you move 1 tile left")
CONSOLE_COMMAND("right", "", CFGFLAG_SERVER|CMDFLAG_TEST, ConGoRight, this, "Makes you move 1 tile right")
CONSOLE_COMMAND("up", "", CFGFLAG_SERVER|CMDFLAG_TEST, ConGoUp, this, "Makes you move 1 tile up")
CONSOLE_COMMAND("down", "", CFGFLAG_SERVER|CMDFLAG_TEST, ConGoDown, this, "Makes you move 1 tile down")
CONSOLE_COMMAND("move", "ii", CFGFLAG_SERVER|CMDFLAG_TEST, ConMove, this, "Moves to the tile with x/y-number ii")
CONSOLE_COMMAND("move_raw", "ii", CFGFLAG_SERVER|CMDFLAG_TEST, ConMoveRaw, this, "Moves to the point with x/y-coordinates ii")
CONSOLE_COMMAND("force_pause", "ii", CFGFLAG_SERVER, ConForcePause, this, "Force i to pause for i seconds")
CONSOLE_COMMAND("force_unpause", "i", CFGFLAG_SERVER, ConForcePause, this, "Set force-pause timer of v to 0.")

CONSOLE_COMMAND("mute", "", CFGFLAG_SERVER, ConMute, this, "");
CONSOLE_COMMAND("muteid", "ii", CFGFLAG_SERVER, ConMuteID, this, "");
CONSOLE_COMMAND("muteip", "si", CFGFLAG_SERVER, ConMuteIP, this, "");
CONSOLE_COMMAND("unmute", "i", CFGFLAG_SERVER, ConUnmute, this, "");
CONSOLE_COMMAND("mutes", "", CFGFLAG_SERVER, ConMutes, this, "");

//jDDRace
CONSOLE_COMMAND("setjumps", "ii", CFGFLAG_SERVER|CMDFLAG_TEST, ConSetJumps, this, "Gives player v i jumps")
//XXLDDRace TODO: remove crap like score/rename
CONSOLE_COMMAND("skin", "is", CFGFLAG_SERVER|CMDFLAG_TEST, ConSkin, this, "Changes the skin from i in s")
CONSOLE_COMMAND("rename", "ir", CFGFLAG_SERVER|CMDFLAG_TEST, ConRename, this, "Renames i name to s")
//CONSOLE_COMMAND("orgname", "v", CFGFLAG_SERVER, ConOrgname, this, "Renames i back to his orginal name", IConsole::CONSOLELEVEL_CONFIG) //not working
CONSOLE_COMMAND("xxl", "i", CFGFLAG_SERVER|CMDFLAG_TEST, ConFastReload, this, "Fast reload :-)")
CONSOLE_COMMAND("rainbow", "i?i", CFGFLAG_SERVER|CMDFLAG_TEST, ConRainbow, this, "Colorchange the tee i like a rainbow")
CONSOLE_COMMAND("whisper", "ir", CFGFLAG_SERVER, ConWhisper, this, "Whispers r to player v")
CONSOLE_COMMAND("w", "ir", CFGFLAG_SERVER, ConWhisper, this, "Whispers r to player v")
CONSOLE_COMMAND("score", "ii", CFGFLAG_SERVER|CMDFLAG_TEST, ConScore, this, "Changes the score of i to i")
CONSOLE_COMMAND("bloody", "i", CFGFLAG_SERVER|CMDFLAG_TEST, ConBlood, this, "BLOOD Ouch?!")
//CONSOLE_COMMAND("test", "s", CFGFLAG_SERVER, ConTest, this, "TEST (Dont use this, it can crash your server!!!!!)")
CONSOLE_COMMAND("member", "i", CFGFLAG_SERVER, ConMember, this, "Sets v to member")
CONSOLE_COMMAND("unmember", "i", CFGFLAG_SERVER, ConUnMember, this, "Unsets v to member")
CONSOLE_COMMAND("checkmember", "i", CFGFLAG_SERVER, ConCheckMember, this, "Checks if v is a member")
CONSOLE_COMMAND("icehammer", "i", CFGFLAG_SERVER|CMDFLAG_TEST, ConIceHammer, this, "The hammer freezes a tee")
CONSOLE_COMMAND("unicehammer", "i", CFGFLAG_SERVER|CMDFLAG_TEST, ConUnIceHammer, this, "The hammer gets normal, it unfreezes a tee")
CONSOLE_COMMAND("admin", "i", CFGFLAG_SERVER, ConSetlvl3, this, "Authenticates player v to the level of 3 (CAUTION: Irreversible, once he is a admin you can't remove his status)")
CONSOLE_COMMAND("moder", "i", CFGFLAG_SERVER, ConSetlvl2, this, "Authenticates player v to the level of 2")
CONSOLE_COMMAND("fly", "", CFGFLAG_SERVER|CMDFLAG_TEST, ConToggleFly, this, "Toggles super-fly (holding space) on/off")
CONSOLE_COMMAND("hammer", "ii", CFGFLAG_SERVER|CMDFLAG_TEST, ConHammer, this, "Sets the hammer power of player v to i")
CONSOLE_COMMAND("invis", "i", CFGFLAG_SERVER|CMDFLAG_TEST, ConInvis, this, "Makes player v invisible")
CONSOLE_COMMAND("vis", "i", CFGFLAG_SERVER|CMDFLAG_TEST, ConVis, this, "Makes player v visible again")
CONSOLE_COMMAND("unfreeze", "i", CFGFLAG_SERVER|CMDFLAG_TEST, ConUnFreeze, this, "Unfreezes player v")
CONSOLE_COMMAND("freeze", "i?i", CFGFLAG_SERVER|CMDFLAG_TEST, ConFreeze, this, "Freezes player v for i seconds (infinite by default)")

CONSOLE_COMMAND("mute", "", CFGFLAG_SERVER, ConMute, this, "");
CONSOLE_COMMAND("muteid", "ii", CFGFLAG_SERVER, ConMuteID, this, "");
CONSOLE_COMMAND("muteip", "si", CFGFLAG_SERVER, ConMuteIP, this, "");
CONSOLE_COMMAND("unmute", "i", CFGFLAG_SERVER, ConUnmute, this, "");
CONSOLE_COMMAND("mutes", "", CFGFLAG_SERVER, ConMutes, this, "");
#undef CONSOLE_COMMAND

#endif
