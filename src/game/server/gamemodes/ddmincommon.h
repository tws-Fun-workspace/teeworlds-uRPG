#ifndef DDMINCOMMON_H
#define DDMINCOMMON_H

#include <cstdio>

#define REFREEZE_INTERVAL_TICKS (Server()->TickSpeed()>>1)

#if defined(CONF_FAMILY_WINDOWS)
 #define D(F, ...) dbg_msg("Flagdrag", "%s:%i:%s(): " F, __FILE__, __LINE__, \
                                                            __FUNCTION__, __VA_ARGS__)
#elif defined(CONF_FAMILY_UNIX)
 #define D(F, ARGS...) dbg_msg("Flagdrag", "%s:%i:%s(): " F, __FILE__, __LINE__, \
#endif

#define DVF "(%f, %f)"
#define DVI "(%d, %d)"

#define DV(V) (V).x, (V).y

#define FLAGCOL_ELAST 0.5f

#define TICK Server()->Tick()
#define TICKSPEED Server()->TickSpeed()
#define CFG(X) g_Config.m_Sv ## X

#define MAX_BROADCAST 128

#endif
