#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "entity/character_base.h"
#include "entity/character_net.h"
#include <utilitieslib/network/net_packet.h>
#include <utilitieslib/network/net_structdefs.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/utils.h>
#include <stdio.h>
#include <string.h>

// Dictionary/storage and auction boundaries; packet composition is production code.
static int capacities[kInventoryType_Count], ids[kInventoryType_Count][2];
static int amounts[kInventoryType_Count][2], clears[kInventoryType_Count];
static int countRefreshes, auctionReceived;
int character_GetInvSize(Character *p, InventoryType type) { return type == kInventoryType_Concept ? 0 : type % 2 + 1; }
int character_GetInvTotalSize(Character *p, InventoryType type) { return 50 + 10 * type; }
int character_GetAuctionInvTotalSize(Character *p) { return 233; }
int character_GetBoostInvTotalSize(Character *p) { return 17; }
void character_GetInvInfo(Character *p, int *id, int *amount, const char **name, InventoryType type, int idx)
{ *id = 100*(type+1)+idx; *amount = 5+type+idx; }
void character_SetInvTotalSize(Character *p, InventoryType type, int size) { if(p) capacities[type] = size; }
void character_ClearInventory(Character *p, InventoryType type, const char *context) { if(p) ++clears[type]; }
void character_SetInventory(Character *p, InventoryType type, int id, int idx, int amount, const char *context)
{ if(p) { ids[type][idx] = id; amounts[type][idx] = amount; } }
void character_SetStoredSalvageInvCurrentCount(Character *p) { ++countRefreshes; }
void character_SetSalvageInvCurrentCount(Character *p) { ++countRefreshes; }
void AuctionInventory_Send(AuctionInventory *inv, Packet *pak) { pktSendBits(pak, 32, 0x12345678); }
void AuctionInventory_Recv(AuctionInventory **inv, Packet *pak) { auctionReceived = pktGetBits(pak, 32) == 0x12345678; }
#define INV_COUNT_PACKBITS 8
#include "inventory_packet_types.inc"
#include "inventory_prepare.inc"
#include "inventory_packet.inc"
#include "inventory_delta_send.inc"
#include "inventory_delta_receive.inc"
#define CHECK(c) do { if (!(c)) { fprintf(stderr, "%d: %s\n", __LINE__, #c); return 0; } } while (0)
static const int expectedTypes[] = {0, 2, 3, 4};
static int checkInventories(void)
{
 Character input = {0}, output = {0}; EntPlayer pl = {0}; Entity e = {0};
 unsigned char bytes[8192] = {0}; Packet pak = {0};
 e.pl = &pl; input.entParent = &e;
 initBitStream(&pak.stream, bytes, sizeof(bytes), Write, 0, NULL);
 character_SendInventories(&input, &pak); pktSendBits(&pak, 32, 0xabcddcba);
 bsChangeMode(&pak.stream, Read);
 CHECK(pktGetBitsPack(&pak, 3) == 4); bsRewind(&pak.stream);
 character_ReceiveInventories(&output, &pak);
 for(int i=0; i<4; ++i) {
  int type=expectedTypes[i]; CHECK(capacities[type] == 50+10*type && clears[type] == 1);
  for(int j=0; j<character_GetInvSize(&input, type); ++j) {
   CHECK(ids[type][j] == 100*(type+1)+j && amounts[type][j] == 5+type+j);
  }
 }
 CHECK(!clears[1] && !capacities[1]);
 CHECK(output.auctionInvTotalSlots == 233);
 CHECK(pktGetBits(&pak,32)==0xabcddcba && pktEnd(&pak));
 bsRewind(&pak.stream); character_ReceiveInventories(NULL, &pak);
 CHECK(pktGetBits(&pak,32)==0xabcddcba && pktEnd(&pak));
 initBitStream(&pak.stream, bytes, sizeof(bytes), Write, 0, NULL);
 character_SendInventorySizes(&input, &pak); pktSendBits(&pak,32,0xabcdef01);
 bsChangeMode(&pak.stream, Read);
 CHECK(pktGetBitsPack(&pak,1)==1 && pktGetBitsPack(&pak,3)==4); bsRewind(&pak.stream);
 character_ReceiveInventorySizes(&output, &pak);
 CHECK(output.auctionInvTotalSlots==233 && output.iNumBoostSlots==17);
 CHECK(pktGetBits(&pak,32)==0xabcdef01 && pktEnd(&pak));
 bsRewind(&pak.stream); character_ReceiveInventorySizes(NULL, &pak);
 CHECK(pktGetBits(&pak,32)==0xabcdef01 && pktEnd(&pak));
 return 1;
}
static int checkDelta(int auction)
{
 Character input={0}, output={0}; Entity sender={0}, receiver={0};
 unsigned char bytes[8192]={0}; Packet pak={0};
 sender.pchar=&input; receiver.pchar=&output;
 for(int i=0; i<4; ++i) { eaiPush(&input.invStatusChange.type, expectedTypes[i]); eaiPush(&input.invStatusChange.idx, 0); }
 input.auctionInvUpdated=auction;
 initBitStream(&pak.stream,bytes,sizeof(bytes),Write,0,NULL);
 entity_SendInvUpdate(&sender,&pak); pktSendBits(&pak,32,0xabcddcba);
 CHECK(eaiSize(&input.invStatusChange.type)==0 && eaiSize(&input.invStatusChange.idx)==0 && !input.auctionInvUpdated);
 bsChangeMode(&pak.stream,Read); entity_ReceiveInvUpdate(&receiver,&pak);
 CHECK(output.auctionInvUpdated==auction && auctionReceived==auction);
 CHECK(countRefreshes==2);
 CHECK(pktGetBits(&pak,32)==0xabcddcba && pktEnd(&pak));
 eaiDestroy(&input.invStatusChange.type); eaiDestroy(&input.invStatusChange.idx);
 countRefreshes=0; auctionReceived=0;
 return 1;
}
static int checkQueue(void)
{
 Character input={0}; GenericInvItem **inv=NULL;
 CHECK(!inventorytype_IsPacketType(kInventoryType_Concept));
 CHECK(!inventorytype_IsPacketType(-1) && !inventorytype_IsPacketType(kInventoryType_Count));
 CHECK(s_PrepInvToSetItem(&input, &inv, kInventoryType_Concept, 0));
 CHECK(!eaiSize(&input.invStatusChange.type) && !eaiSize(&input.invStatusChange.idx));
 for(int i=0; i<4; ++i) {
  CHECK(inventoryPacketTypes[i] == expectedTypes[i]);
  CHECK(s_PrepInvToSetItem(&input, &inv, expectedTypes[i], 0));
  CHECK(input.invStatusChange.type[i] == expectedTypes[i] && input.invStatusChange.idx[i] == 0);
 }
 eaDestroy(&inv); eaiDestroy(&input.invStatusChange.type); eaiDestroy(&input.invStatusChange.idx);
 return 1;
}
static int checkDiscard(void)
{
 Character output={0}; unsigned char bytes[256]={0}; Packet pak={0};
 initBitStream(&pak.stream,bytes,sizeof(bytes),Write,0,NULL);
 // Unsupported current-format record: no obsolete concept variables are read.
 pktSendBitsPack(&pak,GENERICINV_TYPE_PACKBITS,1);
 pktSendBitsPack(&pak,GENERICINV_IDX_PACKBITS,0);
 pktSendBitsPack(&pak,GENERICINV_AMOUNT_PACKBITS,3);
 pktSendBitsPack(&pak,GENERICINV_ID_PACKBITS,123);
 pktSendBits(&pak,32,0xabcddcba); bsChangeMode(&pak.stream,Read);
 character_inventory_Receive(&output,&pak);
 CHECK(!ids[1][0] && !amounts[1][0]);
 CHECK(pktGetBits(&pak,32)==0xabcddcba && pktEnd(&pak));
 return 1;
}
int main(void) { return checkInventories() && checkDelta(0) && checkDelta(1) && checkQueue() && checkDiscard() ? 0 : 1; }
