#include "entity/entity.h"
#include "entity/character_base.h"
#include "entity/teamCommon.h"
#include <utilitieslib/network/net_packet.h>
#include <utilitieslib/network/net_structdefs.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/utils.h>
#include <stdio.h>
#include <string.h>

static int uiUpdates;
void levelingpactListUiUpdate(void) { ++uiUpdates; }
LevelingPact *createLevelingpact(void) { return calloc(1, sizeof(LevelingPact)); }
void destroyLevelingpact(LevelingPact *p)
{
 free(p->members.ids); free(p->members.onMapserver);
 free(p->members.names); free(p->members.mapIds); free(p);
}

#include "pact_send.inc"
#include "pact_receive.inc"

#define CHECK(condition) do { if (!(condition)) { \
 fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, #condition); return 0; \
} } while (0)

static int readUpdate(Entity *source, Entity *destination, bool ignored, int update)
{
 unsigned char bytes[4096] = {0}, expectedBytes[4096] = {0};
 Packet packet = {0}, expected = {0};
 initBitStream(&packet.stream, bytes, sizeof(bytes), Write, 0, NULL);
 initBitStream(&expected.stream, expectedBytes, sizeof(expectedBytes), Write, 0, NULL);
 entSendLevelingpactInfo(&packet, source, update);
 // The supported wire contract includes IDs and names without presence markers.
 pktSendBits(&expected, 1, update);
 if (update) {
  pktSendBitsAuto(&expected, source->levelingpact ? source->levelingpact_id : 0);
  if (source->levelingpact) {
   pktSendBitsAuto(&expected, source->levelingpact->members.count);
   for (int i = 0; i < source->levelingpact->members.count; ++i) {
    pktSendBitsAuto(&expected, source->levelingpact->members.ids[i]);
    pktSendString(&expected, source->levelingpact->members.names[i]);
   }
  }
 }
 CHECK(bsGetBitLength(&packet.stream) == bsGetBitLength(&expected.stream));
 CHECK(!memcmp(bytes, expectedBytes, bsGetLength(&expected.stream)));
 pktSendBits(&packet, 32, 0xabcddcba);
 bsChangeMode(&packet.stream, Read);
 CHECK(entReceiveLevelingpactInfo(&packet, destination, ignored) == update);
 CHECK(pktGetBits(&packet, 32) == 0xabcddcba);
 CHECK(pktEnd(&packet) && !bsIsBad(&packet.stream));
 return 1;
}

static int checkPact(void)
{
 Entity source = {0}, destination = {0};
 LevelingPact pact = {0};
 int ids[MAX_LEVELINGPACT_MEMBERS] = {101, 202};
 MemberName names[MAX_LEVELINGPACT_MEMBERS] = {"Current Hero", "Off-map Hero"};
 Character *character = calloc(1, sizeof(*character));
 destination.pchar = character;
 source.levelingpact = &pact;
 source.levelingpact_id = 42;
 pact.members.ids = ids;
 pact.members.names = names;
 for (int count = 1; count <= MAX_LEVELINGPACT_MEMBERS; ++count) {
  pact.members.count = count;
  CHECK(readUpdate(&source, &destination, false, 1));
  CHECK(destination.levelingpact_id == 42);
  CHECK(destination.levelingpact->members.count == count);
  CHECK(destination.levelingpact->count == count);
  for (int i = 0; i < count; ++i) {
   CHECK(destination.levelingpact->members.ids[i] == ids[i]);
   CHECK(!strcmp(destination.levelingpact->members.names[i], names[i]));
   CHECK(!destination.levelingpact->members.onMapserver[i]);
  }
 }
 LevelingPact *savedPact = destination.levelingpact;
 int savedUiUpdates = uiUpdates;
 source.levelingpact_id = 77;
 strcpy(names[0], "Newer-name fixture");
 CHECK(readUpdate(&source, &destination, true, 1));
 CHECK(destination.levelingpact_id == 42 && destination.levelingpact == savedPact);
 CHECK(!strcmp(destination.levelingpact->members.names[0], "Current Hero"));
 CHECK(readUpdate(&source, NULL, false, 1));
 destination.pchar = NULL;
 CHECK(readUpdate(&source, &destination, false, 1));
 CHECK(destination.levelingpact_id == 42 && destination.levelingpact == savedPact);
 destination.pchar = character;
 CHECK(readUpdate(&source, &destination, false, 0));
 CHECK(destination.levelingpact == savedPact);
 source.levelingpact = NULL;
 CHECK(readUpdate(&source, &destination, true, 1));
 CHECK(destination.levelingpact_id == 42 && destination.levelingpact == savedPact);
 CHECK(uiUpdates == savedUiUpdates);
 CHECK(readUpdate(&source, NULL, false, 1));
 CHECK(readUpdate(&source, &destination, false, 1));
 CHECK(!destination.levelingpact && destination.levelingpact_id == 0);
 CHECK(uiUpdates == savedUiUpdates + 1);
 free(character);
 return 1;
}

int main(void)
{
 return checkPact() ? 0 : 1;
}
