/* (c) Shereef Marzouk. See "licence DDRace.txt" and the readme.txt in the root of the distribution for more information. */
#ifndef GAME_SERVER_DDRACECOMMANDS_H
#define GAME_SERVER_DDRACECOMMANDS_H
#undef GAME_SERVER_DDRACECOMMANDS_H // this file can be included several times
#ifndef CONSOLE_COMMAND
#define CONSOLE_COMMAND(name, params, flags, callback, userdata, help)
#endif

CONSOLE_COMMAND("Kill","v",                 CFGFLAG_SERVER,              ConKillPlayer, this, "Kills player v and announces the kill")
CONSOLE_COMMAND("Tele", "vi",               CFGFLAG_SERVER|CMDFLAG_TEST, ConTeleport, this, "Teleports v to player v")
CONSOLE_COMMAND("Weapons", "v",             CFGFLAG_SERVER|CMDFLAG_TEST, ConWeapons, this, "Gives all weapons to you")
CONSOLE_COMMAND("AddWeapon", "i",           CFGFLAG_SERVER|CMDFLAG_TEST, ConAddWeapon, this, "Gives weapon with id i to you (all = -1, hammer = 0, gun = 1, shotgun = 2, grenade = 3, laser = 4, ninja = 5)")
CONSOLE_COMMAND("UnWeapons", "v",           CFGFLAG_SERVER|CMDFLAG_TEST, ConUnWeapons, this, "Takes all weapons from you")
CONSOLE_COMMAND("Removeweapon", "i",        CFGFLAG_SERVER,              ConRemoveWeapon, this, "removes weapon with id i from you (all = -1, hammer = 0, gun = 1, shotgun = 2, grenade = 3, laser = 4)")
CONSOLE_COMMAND("Shotgun", "v",             CFGFLAG_SERVER|CMDFLAG_TEST, ConShotgun, this, "Gives a shotgun to you")
CONSOLE_COMMAND("Grenade", "v",             CFGFLAG_SERVER|CMDFLAG_TEST, ConGrenade, this, "Gives a grenade launcher to you")
CONSOLE_COMMAND("Laser", "v",               CFGFLAG_SERVER|CMDFLAG_TEST, ConLaser, this, "Gives a laser to you")
CONSOLE_COMMAND("UnShotgun", "v",           CFGFLAG_SERVER,              ConUnShotgun, this, "Takes the shotgun from you")
CONSOLE_COMMAND("UnGrenade", "v",           CFGFLAG_SERVER,              ConUnGrenade, this, "Takes the grenade launcher you")
CONSOLE_COMMAND("UnLaser", "v",             CFGFLAG_SERVER,              ConUnLaser, this, "Takes the laser from you")
CONSOLE_COMMAND("Ninja", "v",               CFGFLAG_SERVER|CMDFLAG_TEST, ConNinja, this, "Makes you a ninja")
CONSOLE_COMMAND("Super", "v",               CFGFLAG_SERVER|CMDFLAG_TEST, ConSuper, this, "Makes v super")
CONSOLE_COMMAND("unSuper", "v",             CFGFLAG_SERVER,              ConUnSuper, this, "Removes super from smb")
CONSOLE_COMMAND("Left", "v",                CFGFLAG_SERVER|CMDFLAG_TEST, ConGoLeft, this, "Makes you move 1 tile left")
CONSOLE_COMMAND("Right", "v",               CFGFLAG_SERVER|CMDFLAG_TEST, ConGoRight, this, "Makes you move 1 tile right")
CONSOLE_COMMAND("Up", "v",                  CFGFLAG_SERVER|CMDFLAG_TEST, ConGoUp, this, "Makes you move 1 tile up")
CONSOLE_COMMAND("Down", "v",                CFGFLAG_SERVER|CMDFLAG_TEST, ConGoDown, this, "Makes you move 1 tile down")

CONSOLE_COMMAND("Move", "vii",              CFGFLAG_SERVER|CMDFLAG_TEST, ConMove, this, "Moves to the tile with x/y-number ii")
CONSOLE_COMMAND("Move_raw", "vi",           CFGFLAG_SERVER,              ConMoveRaw, this, "Moves to the point with x/y-coordinates ii")
CONSOLE_COMMAND("Force_pause", "vi",        CFGFLAG_SERVER,              ConForcePause, this, "Force i to pause for i seconds")
CONSOLE_COMMAND("Force_unpause", "v",       CFGFLAG_SERVER,              ConForcePause, this, "Set force-pause timer of v to 0.")
CONSOLE_COMMAND("showothers", "?i",         CFGFLAG_CHAT,                ConShowOthers, this, "Whether to showplayers from other teams or not (off by default), optional i = 0 for off else for on")

CONSOLE_COMMAND("list", "?s",               CFGFLAG_CHAT,                ConList, this, "List connected players with optional case-insensitive substring matching filter")




// Another commands
CONSOLE_COMMAND("SetJumps", "vi",           CFGFLAG_SERVER|CMDFLAG_TEST, ConSetJumps, this, "Gives player v i jumps")
CONSOLE_COMMAND("Bloody", "v",              CFGFLAG_SERVER|CMDFLAG_TEST, ConBlood, this, "BLOOD Ouch?!")
CONSOLE_COMMAND("Fly", "v",                 CFGFLAG_SERVER|CMDFLAG_TEST, ConToggleFly, this, "Toggles super-fly (holding space) on/off")
CONSOLE_COMMAND("Xxl", "v",                 CFGFLAG_SERVER|CMDFLAG_TEST, ConFastReload, this, "Fast reload.")
CONSOLE_COMMAND("UnXxl", "v",               CFGFLAG_SERVER,              ConUnFastReload, this, "Remove fast reload.")
CONSOLE_COMMAND("Rainbow", "v?i",           CFGFLAG_SERVER|CMDFLAG_TEST, ConRainbow, this, "Colorchange the tee i like a rainbow")
CONSOLE_COMMAND("Invis", "v",               CFGFLAG_SERVER|CMDFLAG_TEST, ConInvis, this, "Makes player v invisible")
CONSOLE_COMMAND("Vis", "v",                 CFGFLAG_SERVER|CMDFLAG_TEST, ConVis, this, "Makes player v visible again")
CONSOLE_COMMAND("whisper", "v?r",           CFGFLAG_CHAT,                ConWhisper, this, "Whispers r to player v")
CONSOLE_COMMAND("w", "v?r",                 CFGFLAG_CHAT,                ConWhisper, this, "Whispers r to player v")
CONSOLE_COMMAND("Rename", "vr",             CFGFLAG_SERVER,              ConRename, this, "Renames i name to s")
CONSOLE_COMMAND("Score", "vi",              CFGFLAG_SERVER|CMDFLAG_TEST, ConScore, this, "Changes the score of i to i")
CONSOLE_COMMAND("Hammer", "vi",             CFGFLAG_SERVER|CMDFLAG_TEST, ConHammer, this, "Sets the hammer power of player v to i")
CONSOLE_COMMAND("tHammer", "v",             CFGFLAG_SERVER|CMDFLAG_TEST, ConTHammer, this, "Tele-hammer to your cursor")
CONSOLE_COMMAND("eHammer", "v",             CFGFLAG_SERVER|CMDFLAG_TEST, ConEHammer, this, "Create Explosion near cursor")
CONSOLE_COMMAND("Hexp", "v",                CFGFLAG_SERVER|CMDFLAG_TEST, ConHeXP, this, "Hammer explosion")
CONSOLE_COMMAND("Gunexp", "v",              CFGFLAG_SERVER|CMDFLAG_TEST, ConGuneXP, this, "Gun explosion")
CONSOLE_COMMAND("gBounce", "v",             CFGFLAG_SERVER|CMDFLAG_TEST, ConGrenadeBounce, this, "Bounce grenades")
CONSOLE_COMMAND("sWall", "v",               CFGFLAG_SERVER|CMDFLAG_TEST, ConShotgunWall, this, "Shotgun Walls")

CONSOLE_COMMAND("IceHammer", "v",           CFGFLAG_SERVER|CMDFLAG_TEST, ConIceHammer, this, "The hammer freezes a tee")
CONSOLE_COMMAND("unIceHammer", "v",         CFGFLAG_SERVER,              ConUnIceHammer, this, "The hammer gets normal, it unfreezes a tee")

CONSOLE_COMMAND("DeepHammer", "v",          CFGFLAG_SERVER|CMDFLAG_TEST, ConDeepHammer, this, "Deephammer")
CONSOLE_COMMAND("unDeepHammer", "v",        CFGFLAG_SERVER,              ConUnDeepHammer, this, "Undeephammer")

CONSOLE_COMMAND("gHammer", "v",             CFGFLAG_SERVER|CMDFLAG_TEST, ConGHammer, this, "Grenade - hammer")
CONSOLE_COMMAND("unGHammer", "v",           CFGFLAG_SERVER,              ConUnGHammer, this, "Remove Grenade - hammer")

CONSOLE_COMMAND("cHammer", "v",             CFGFLAG_SERVER|CMDFLAG_TEST, ConColorHammer, this, "Color - Hammer")
CONSOLE_COMMAND("unCHammer", "v",           CFGFLAG_SERVER,              ConUnColorHammer, this, "Remove Color - Hammer")

CONSOLE_COMMAND("Plasma", "v?ii",           CFGFLAG_SERVER|CMDFLAG_TEST, ConPlasmaLaser, this, "Laser-Plasma, Freeze 1/0, Explosion 1/0")
CONSOLE_COMMAND("unPlasma", "v",            CFGFLAG_SERVER,              ConUnPlasmaLaser, this, "Remove Laser-Plasma")

CONSOLE_COMMAND("GunSpread", "v?i",         CFGFLAG_SERVER|CMDFLAG_TEST, ConGunSpread, this, "Gun Spread, i = count of spread")
CONSOLE_COMMAND("ShotgunSpread", "v?i",     CFGFLAG_SERVER|CMDFLAG_TEST, ConShotgunSpread, this, "Shotgun Spread, i = count of spread")
CONSOLE_COMMAND("GrenadeSpread", "v?i",     CFGFLAG_SERVER|CMDFLAG_TEST, ConGrenadeSpread, this, "Grenade Spread, i = count of spread")
CONSOLE_COMMAND("LaserSpread", "v?i",       CFGFLAG_SERVER|CMDFLAG_TEST, ConLaserSpread, this, "Laser Spread, i = count of spread")

CONSOLE_COMMAND("GiveKid", "v",             CFGFLAG_SERVER,              ConGiveKid, this, "Give kid to player v")
CONSOLE_COMMAND("GiveHelper", "v",          CFGFLAG_SERVER,              ConGiveHelper, this, "Give helper to player v")
CONSOLE_COMMAND("GiveModer", "v",           CFGFLAG_SERVER,              ConGiveModer, this, "Give moderator to player v")
CONSOLE_COMMAND("RemoveLevel", "v",         CFGFLAG_SERVER,              ConRemoveLevel, this, "Remove level form player v")

CONSOLE_COMMAND("Freeze", "v?i",            CFGFLAG_SERVER|CMDFLAG_TEST, ConFreezePlayer, this, "Freezes player v for i seconds (infinite by default)")
CONSOLE_COMMAND("UnFreeze", "v",            CFGFLAG_SERVER,              ConUnFreezePlayer, this, "Unfreezes player v")

CONSOLE_COMMAND("PlayAs", "v",              CFGFLAG_SERVER,              ConPlayAs, this, "Play as v")

CONSOLE_COMMAND("canKill", "v",             CFGFLAG_SERVER,              ConCanKill, this, "Enable to kill players with gun")
CONSOLE_COMMAND("canDie", "v",              CFGFLAG_SERVER,              ConCanDie, this, "Enable to kill players with gun")

CONSOLE_COMMAND("RemRise", "v",             CFGFLAG_SERVER,              ConRemRise, this, "Remove all extras")
CONSOLE_COMMAND("Mute", "v",                CFGFLAG_SERVER,              ConMute, this, "Mute player");
CONSOLE_COMMAND("MuteId", "vi",             CFGFLAG_SERVER,              ConMuteID, this, "Mute player by id");
CONSOLE_COMMAND("MuteIp", "si",             CFGFLAG_SERVER,              ConMuteIP, this, "Mute player by IP");
CONSOLE_COMMAND("unMute", "v",              CFGFLAG_SERVER,              ConUnmute, this, "UnMute player");
CONSOLE_COMMAND("Mutes", "",                CFGFLAG_SERVER,              ConMutes, this, "Show mutes");
#undef CONSOLE_COMMAND

#endif
