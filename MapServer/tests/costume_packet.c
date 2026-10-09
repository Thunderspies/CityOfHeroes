// Production costume codecs run with small current dictionary/allocation fixtures.
#include "entity/costume.h"
#include <utilitieslib/network/net_packet.h>
#include <utilitieslib/network/net_structdefs.h>
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/components/StringCache.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/assert/assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

TokenizerParseInfo ParseCostume[] = {{0}};
TokenizerParseInfo ParseCustomNPCCostume[] = {{0}};
cStashTable costume_HashTable;
cStashTable costume_HashTableExtraInfo;
cStashTable costume_PaletteTable;
int GetBodyPartCount(void) { return 2; }
int getBodyTypeCount(void) { return 9; }
bool isNullOrNone(const char *s) { return !s || !stricmp(s, "None"); }

#include "costume_packet.inc"

#define CHECK(condition) do { if (!(condition)) { \
 fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, #condition); return 0; \
} } while (0)

static int checkCostume(int names, int parts)
{
 Costume input = {0}, output = {0};
 CostumePart source[2] = {0}, destination[2] = {0};
 CostumePart *sourceParts[] = { &source[0], &source[1] };
 CostumePart *destParts[] = { &destination[0], &destination[1] };
 unsigned char bytes[2][8192] = {0};
 Packet packets[2] = {0};
 input.parts = sourceParts;
 output.parts = destParts;
 input.appearance.bodytype = kBodyType_Female;
 input.appearance.colorSkin.integer = 0x12345678;
 input.appearance.currentSuperColorSet = 5;
 input.appearance.iNumParts = parts;
 for (int i = 0; i < MAX_BODY_SCALES; ++i)
  input.appearance.fScales[i] = (i % 9 - 4) * 0.125f;
 for (int i = 0; i < NUM_SG_COLOR_SLOTS; ++i) {
  input.appearance.superColorsPrimaryU[i].SGBitSetLow = 0x12345678 + i;
  input.appearance.superColorsSecondaryU[i].SGBitSetHigh = 0xabcdef00 + i;
 }
 for (int i = 0; i < 2; ++i) {
  source[i].pchGeom = "CurrentGeometry";
  source[i].pchTex1 = "CurrentTexture";
  source[i].pchTex2 = "CurrentTexture";
  source[i].pchFxName = i ? "CurrentEffect" : "None";
  source[i].regionName = "CurrentRegion";
  source[i].bodySetName = "CurrentBodySet";
  for (int c = 0; c < 4; ++c) source[i].color[c].integer = 0x10203040 + c;
 }
 for (int marker = 0; marker < 2; ++marker) {
  initBitStream(&packets[marker].stream, bytes[marker], sizeof(bytes[marker]),
   Write, 0, NULL);
  input.appearance.convertedScale = marker;
  costume_send(&packets[marker], &input, names);
  pktSendBits(&packets[marker], 32, 0xabcddcba);
 }
 CHECK(bsGetBitLength(&packets[0].stream) == bsGetBitLength(&packets[1].stream));
 CHECK(!memcmp(bytes[0], bytes[1], bsGetLength(&packets[0].stream)));
 bsChangeMode(&packets[0].stream, Read);
 CHECK(costume_receive(&packets[0], &output));
 CHECK(output.appearance.convertedScale == 1);
 CHECK(output.appearance.bodytype == input.appearance.bodytype);
 CHECK(output.appearance.colorSkin.integer == input.appearance.colorSkin.integer);
 CHECK(output.appearance.currentSuperColorSet == 5);
 CHECK(output.appearance.iNumParts == parts);
 for (int i = 0; i < MAX_BODY_SCALES; ++i)
  CHECK(fabsf(output.appearance.fScales[i] - input.appearance.fScales[i])
   <= (names || i == 0 ? 0.0f : 2.0f / 63.0f));
 for (int i = 0; i < NUM_SG_COLOR_SLOTS; ++i) {
  CHECK(output.appearance.superColorsPrimaryU[i].SGBitSetLow == 0x12345678 + i);
  CHECK(output.appearance.superColorsSecondaryU[i].SGBitSetHigh == 0xabcdef00 + i);
 }
 for (int i = 0; i < parts; ++i) {
  CHECK(!strcmp(destination[i].pchGeom, source[i].pchGeom));
  CHECK(!strcmp(destination[i].pchTex1, source[i].pchTex1));
  CHECK(!strcmp(destination[i].pchFxName, source[i].pchFxName));
  for (int c = 0; c < 4; ++c)
   CHECK(destination[i].color[c].integer == (c < 2 || i ? source[i].color[c].integer : 0));
  if (names) {
   CHECK(!strcmp(destination[i].regionName, source[i].regionName));
   CHECK(!strcmp(destination[i].bodySetName, source[i].bodySetName));
  }
 }
 CHECK(pktGetBits(&packets[0], 32) == 0xabcddcba);
 CHECK(pktEnd(&packets[0]) && !bsIsBad(&packets[0].stream));
 return 1;
}

int main(void)
{
 costume_HashTable = stashTableCreateWithStringKeys(16, StashDeepCopyKeys);
 costume_HashTableExtraInfo = stashTableCreateWithStringKeys(16, StashDeepCopyKeys);
 stashAddInt((StashTable)costume_HashTable, "CurrentGeometry", 1, false);
 stashAddInt((StashTable)costume_HashTable, "CurrentTexture", 1, false);
 stashAddInt((StashTable)costume_HashTableExtraInfo, "CurrentRegion", 1, false);
 for (int names = 0; names < 2; ++names)
  for (int parts = 1; parts <= 2; ++parts)
   if (!checkCostume(names, parts)) return 1;
 puts("Current costume packet round trips passed");
 return 0;
}
