#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "entity/costume.h"
#include <utilitieslib/network/net_packet.h>
#include <utilitieslib/network/net_structdefs.h>
#include <stdio.h>

#define START_BIT_COUNT(pak, name)
#define STOP_BIT_COUNT(pak)

static U32 teamBuff, dock, inspiration;
const cCostume *costume_as_const(Costume *c) { return (const cCostume *)c; }
static void costume_Apply(Entity *e) {}
static void receiveTeamBuffMode(Packet *pak) { teamBuff = pktGetBits(pak, 1); }
static void receiveDockMode(Packet *pak) { dock = pktGetBits(pak, 32); }
static void receiveInspirationMode(Packet *pak) { inspiration = pktGetBits(pak, 32); }

// Compile the actual adjacent full-update sections against real entity layouts.
static void sendSettings(Packet *pak, Entity *e)
{
 #include "entity_settings_send.inc"
}
static void receiveSettings(Packet *pak, Entity *e)
{
 #include "entity_settings_receive.inc"
}
#define CHECK(condition) do { if (!(condition)) { \
 fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, #condition); return 0; \
} } while (0)

static int checkSettings(int flags)
{
 Entity input = {0}, output = {0};
 EntPlayer source = {0}, destination = {0};
 unsigned char bytes[1024] = {0};
 Packet packet = {0};
 input.pl = &source;
 output.pl = &destination;
 source.lfg = 0x123;
 source.hidden = 0x234;
 source.supergroup_mode = flags;
 source.hide_supergroup_emblem = !flags;
 source.teambuff_display = flags;
 source.dock_mode = 0x12345678;
 source.inspiration_mode = 0x87654321;
 source.newFeaturesVersion = 0xabcd;
 initBitStream(&packet.stream, bytes, sizeof(bytes), Write, 0, NULL);
 sendSettings(&packet, &input);
 CHECK(bsGetBitLength(&packet.stream) == 10 + 10 + 3 + 32 * 3);
 pktSendBits(&packet, 32, 0xabcddcba);
 bsChangeMode(&packet.stream, Read);
 receiveSettings(&packet, &output);
 CHECK(destination.lfg == source.lfg && destination.hidden == source.hidden);
 CHECK(destination.supergroup_mode == flags);
 CHECK(destination.hide_supergroup_emblem == !flags);
 CHECK(teamBuff == flags);
 CHECK(dock == source.dock_mode && inspiration == source.inspiration_mode);
 CHECK(destination.newFeaturesVersion == source.newFeaturesVersion);
 CHECK(pktGetBits(&packet, 32) == 0xabcddcba);
 CHECK(pktEnd(&packet) && !bsIsBad(&packet.stream));
 return 1;
}

int main(void)
{
 return checkSettings(0) && checkSettings(1) ? 0 : 1;
}
