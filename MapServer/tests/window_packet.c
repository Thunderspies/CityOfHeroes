#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "gameComm/wdwbase.h"
#include <utilitieslib/network/net_packet.h>
#include <utilitieslib/network/net_structdefs.h>
#include <stdio.h>
#define CHECK(c) do { if (!(c)) { fprintf(stderr, "%d: %s\n", __LINE__, #c); return 1; } } while (0)
static void sendWindows(Packet *pak, Entity *e)
{
 int k;
 #include "window_packet_send.inc"
}
static int retired(int id)
{
 return id == WDW_DEPRECATED_1 || id == WDW_UNUSED_1 || id == WDW_DEPRECATED_2
  || id == WDW_CONCEPTINV || id == WDW_INVENT || id == WDW_WEB_STORE || id == WDW_MAIN_STORE_ACCESS;
}
int main(void)
{
 Entity e = {0}; EntPlayer pl = {0};
 WdwBase windows[MAX_WINDOW_COUNT] = {0};
 unsigned char bytes[65536] = {0}; Packet pak = {0};
 e.pl = &pl; pl.winLocs = windows;
 for (int i = 0; i < MAX_WINDOW_COUNT; ++i) {
  windows[i].xp = 10+i; windows[i].yp = 200+i;
  windows[i].mode = WINDOW_DISPLAYING; windows[i].locked = i%2;
  windows[i].wd = 300+i; windows[i].ht = 400+i;
  windows[i].draggable_frame = 1; windows[i].maximized = i%2;
  windows[i].color = 0x11223344+i; windows[i].back_color = 0x55667788+i;
  windows[i].sc = 0.75f; windows[i].start_shrunk = DEFAULT_OPEN;
 }
 initBitStream(&pak.stream, bytes, sizeof(bytes), Write, 0, NULL);
 sendWindows(&pak, &e); pktSendBits(&pak, 32, 0xabcddcba);
 bsChangeMode(&pak.stream, Read);
 for (int i = 0; i < MAX_WINDOW_COUNT; ++i) {
  WdwBase out = {0}; int id;
  if (retired(i)) continue;
  receiveWindowIdx(&pak, &id); receiveWindow(&pak, &out);
  CHECK(id == i); CHECK(out.xp == windows[i].xp && out.yp == windows[i].yp);
  CHECK(out.mode == windows[i].mode && out.locked == windows[i].locked);
  CHECK(out.wd == windows[i].wd && out.ht == windows[i].ht);
  CHECK(out.maximized == windows[i].maximized && out.sc == windows[i].sc);
  CHECK(out.color == windows[i].color && out.back_color == windows[i].back_color);
 }
 CHECK(pktGetBits(&pak, 32) == 0xabcddcba);
 CHECK(pktEnd(&pak) && !bsIsBad(&pak.stream));
 // Custom-window IDs remain valid in the shared record codec.
 initBitStream(&pak.stream, bytes, sizeof(bytes), Write, 0, NULL);
 windows[0].start_shrunk = ALWAYS_CLOSED;
 sendWindow(&pak, &windows[0], MAX_WINDOW_COUNT+2);
 pktSendBits(&pak, 32, 0xabcdef01); bsChangeMode(&pak.stream, Read);
 { WdwBase out = {0}; int id;
  receiveWindowIdx(&pak, &id); receiveWindow(&pak, &out);
  CHECK(id == MAX_WINDOW_COUNT+2 && out.mode == WINDOW_DOCKED);
 }
 CHECK(pktGetBits(&pak, 32) == 0xabcdef01 && pktEnd(&pak));
 return 0;
}
