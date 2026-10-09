#include "entity/chatSettings.h"
#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "entity/character_base.h"
#include <utilitieslib/network/net_packet.h>
#include <utilitieslib/network/net_structdefs.h>
#include <utilitieslib/components/bitfield.h>
#include <utilitieslib/utils/utils.h>
#include <stdio.h>
#include <string.h>
#include "chat_packet.inc"
#define CHECK(c) do { if (!(c)) { fprintf(stderr, "%d: %s\n", __LINE__, #c); return 0; } } while (0)
static int checkRoundTrip(int localized)
{
 Entity input={0}, output={0}; EntPlayer source={0}, dest={0}; Character pchar={0}; CharacterClass cl={0};
 ChatSettings before;
 unsigned char bytes[16384]={0}; Packet pak={0};
 input.pl=&source; output.pl=&dest; input.pchar=&pchar; pchar.pclass=&cl; cl.pchName="Class_Blaster";
 source.chatSendChannel=INFO_TEAM_COM;
 source.chat_settings.options=(localized ? CSFlags_DoNotLocalize : 0) | CSFlags_BottomDividerSelected | (3<<2);
 source.chat_settings.userSendChannel=2; source.chat_settings.primaryChatMinimized=333;
 for(int i=0;i<MAX_CHAT_WINDOWS;++i) {
  source.chat_settings.windows[i].selectedtop=i+1;
  source.chat_settings.windows[i].selectedbot=i+2;
  source.chat_settings.windows[i].tabListBF=1<<i;
  source.chat_settings.windows[i].divider=0.25f+0.125f*i;
 }
 for(int i=0;i<MAX_CHAT_TABS;++i) {
  ChatTabSettings *tab=&source.chat_settings.tabs[i];
  sprintf_s(tab->name,sizeof(tab->name),"Tab%d",i);
  BitFieldSet(tab->systemChannels,SYSTEM_CHANNEL_BITFIELD_SIZE,INFO_HELP,1);
  tab->userBF=i; tab->optionsBF=1<<ChannelOption_Top;
  tab->defaultChannel=INFO_HELP; tab->defaultType=ChannelType_System;
 }
 for(int i=0;i<MAX_CHAT_CHANNELS;++i) {
  ChatChannelSettings *ch=&source.chat_settings.channels[i];
  sprintf_s(ch->name,sizeof(ch->name),"Channel%d",i);
  ch->optionsBF=i; ch->color1=0x10203040+i; ch->color2=0x50607080+i;
 }
 before=source.chat_settings;
 initBitStream(&pak.stream,bytes,sizeof(bytes),Write,0,NULL);
 sendChatSettings(&pak,&input); pktSendBits(&pak,32,0xabcddcba);
 CHECK(!memcmp(&before,&source.chat_settings,sizeof(before)));
 bsChangeMode(&pak.stream,Read);
 CHECK(pktGetBitsPack(&pak,1)==source.chatSendChannel); bsRewind(&pak.stream);
 receiveChatSettings(&pak,&output);
 CHECK(dest.chatSendChannel==source.chatSendChannel);
 CHECK(dest.chat_settings.options==before.options && dest.chat_settings.userSendChannel==2);
 CHECK(dest.chat_settings.primaryChatMinimized==333);
 CHECK(!memcmp(dest.chat_settings.windows,before.windows,sizeof(before.windows)));
 CHECK(!memcmp(dest.chat_settings.tabs,before.tabs,sizeof(before.tabs)));
 CHECK(!memcmp(dest.chat_settings.channels,before.channels,sizeof(before.channels)));
#ifdef SERVER
 CHECK(BitFieldGet(dest.chat_settings.systemChannels,SYSTEM_CHANNEL_BITFIELD_SIZE,INFO_HELP));
#endif
 CHECK(pktGetBits(&pak,32)==0xabcddcba && pktEnd(&pak));
 return 1;
}
static int checkDefaults(int villain, int praetorian, int mastermind)
{
 Entity e={0}; EntPlayer pl={0}; Character pchar={0}; CharacterClass cl={0};
 e.pl=&pl; e.pchar=&pchar; pchar.pclass=&cl;
 pchar.playerTypeByLocation=villain ? kPlayerType_Villain : kPlayerType_Hero;
 pl.praetorianProgress=praetorian ? kPraetorianProgress_Praetoria : kPraetorianProgress_PrimalBorn;
 cl.pchName=mastermind ? "Class_Mastermind" : "Class_Blaster";
 chatSettings_InitDefaults(&e);
 CHECK(pl.chat_settings.options==0 && pl.chatSendChannel==INFO_TAB);
 CHECK(!strcmp(pl.chat_settings.tabs[0].name,"DefaultGlobalChat"));
 CHECK(!strcmp(pl.chat_settings.tabs[1].name,"DefaultChatChat"));
 CHECK(!strcmp(pl.chat_settings.tabs[2].name,"DefaultHelpChat"));
 CHECK(!strcmp(pl.chat_settings.tabs[3].name,"DefaultCombatChat"));
 CHECK(pl.chat_settings.windows[0].tabListBF==(mastermind ? 31 : 15));
 CHECK(pl.chat_settings.windows[0].divider==0.5f);
 CHECK(pl.chat_settings.tabs[1].optionsBF==(1<<ChannelOption_Bottom));
 CHECK(BitFieldGet(pl.chat_settings.tabs[0].systemChannels,SYSTEM_CHANNEL_BITFIELD_SIZE,INFO_LOOKING_FOR_GROUP));
 CHECK(BitFieldGet(pl.chat_settings.tabs[0].systemChannels,SYSTEM_CHANNEL_BITFIELD_SIZE,INFO_ARCHITECT));
 CHECK(BitFieldGet(pl.chat_settings.tabs[0].systemChannels,SYSTEM_CHANNEL_BITFIELD_SIZE,INFO_LEAGUE_COM));
 CHECK(BitFieldGet(pl.chat_settings.tabs[2].systemChannels,SYSTEM_CHANNEL_BITFIELD_SIZE,INFO_HELP));
 CHECK(BitFieldGet(pl.chat_settings.tabs[2].systemChannels,SYSTEM_CHANNEL_BITFIELD_SIZE,INFO_REWARD));
 CHECK(BitFieldGet(pl.chat_settings.tabs[2].systemChannels,SYSTEM_CHANNEL_BITFIELD_SIZE,INFO_ARCHITECT));
 CHECK(BitFieldGet(pl.chat_settings.tabs[2].systemChannels,SYSTEM_CHANNEL_BITFIELD_SIZE,INFO_LEAGUE_COM));
 CHECK(BitFieldGet(pl.chat_settings.tabs[3].systemChannels,SYSTEM_CHANNEL_BITFIELD_SIZE,INFO_REWARD));
 CHECK(BitFieldGet(pl.chat_settings.tabs[3].systemChannels,SYSTEM_CHANNEL_BITFIELD_SIZE,INFO_COMBAT_SPAM));
 CHECK(BitFieldGet(pl.chat_settings.tabs[0].systemChannels,SYSTEM_CHANNEL_BITFIELD_SIZE,
  praetorian ? INFO_EVENT_PRAETORIAN : villain ? INFO_EVENT_VILLAIN : INFO_EVENT_HERO));
 CHECK(mastermind ? !strcmp(pl.chat_settings.tabs[4].name,"DefaultPetCombat") : !pl.chat_settings.tabs[4].name[0]);
#ifdef SERVER
 CHECK(BitFieldGet(pl.chat_settings.systemChannels,SYSTEM_CHANNEL_BITFIELD_SIZE,INFO_HELP));
 CHECK(BitFieldGet(pl.chat_settings.systemChannels,SYSTEM_CHANNEL_BITFIELD_SIZE,INFO_REWARD));
#endif
 return 1;
}
static int checkOptionsMask(void)
{
 Entity in={0},out={0}; EntPlayer source={0},dest={0};
 unsigned char bytes[16384]={0}; Packet pak={0};
 in.pl=&source; out.pl=&dest; source.chat_settings.options=-1;
 initBitStream(&pak.stream,bytes,sizeof(bytes),Write,0,NULL);
 sendChatSettings(&pak,&in); pktSendBits(&pak,32,0xabcddcba);
 CHECK(source.chat_settings.options==-1);
 bsChangeMode(&pak.stream,Read); receiveChatSettings(&pak,&out);
 CHECK(dest.chat_settings.options==0x5f);
 CHECK(pktGetBits(&pak,32)==0xabcddcba && pktEnd(&pak));
 return 1;
}
#ifdef CLIENT
// UI/filter construction and localization-provider boundaries.
static void *menuMessages;
static int loaded, saved, localizedNames;
static int testLocalize(void *store, char *buf, size_t len, const char *key)
{ ++localizedNames; return sprintf_s(buf,len,"L:%s",key); }
static void loadEntityChatSettings(Entity *e, int full) { ++loaded; }
void sendChatSettingsToServer(void) { ++saved; }
#define msPrintf testLocalize
#include "chat_ui_receive.inc"
#undef msPrintf
static int checkLocalization(int localized)
{
 Entity in={0},out={0}; EntPlayer source={0},dest={0};
 unsigned char bytes[16384]={0}; Packet pak={0};
 in.pl=&source; out.pl=&dest;
 source.chat_settings.options=localized ? CSFlags_DoNotLocalize : 0;
 strcpy_s(source.chat_settings.tabs[0].name,sizeof(source.chat_settings.tabs[0].name),"Help");
 strcpy_s(source.chat_settings.tabs[19].name,sizeof(source.chat_settings.tabs[19].name),"Tiny");
 initBitStream(&pak.stream,bytes,sizeof(bytes),Write,0,NULL);
 sendChatSettings(&pak,&in); pktSendBits(&pak,32,0xabcddcba);
 bsChangeMode(&pak.stream,Read); receiveChatSettingsFromServer(&pak,&out);
 CHECK(loaded==1 && saved==!localized && localizedNames==(localized ? 0 : 2));
 CHECK(!strcmp(dest.chat_settings.tabs[0].name,localized ? "Help" : "L:Help"));
 CHECK(!strcmp(dest.chat_settings.tabs[19].name,localized ? "Tiny" : "L:Tiny"));
 CHECK(pktGetBits(&pak,32)==0xabcddcba && pktEnd(&pak));
 loaded=saved=localizedNames=0;
 return 1;
}
#endif
int main(void)
{
 if(!checkRoundTrip(0) || !checkRoundTrip(1) || !checkOptionsMask()) return 1;
 for(int villain=0;villain<2;++villain)
  for(int praetorian=0;praetorian<2;++praetorian)
   for(int mastermind=0;mastermind<2;++mastermind)
    if(!checkDefaults(villain,praetorian,mastermind)) return 1;
#ifdef CLIENT
 if(!checkLocalization(0) || !checkLocalization(1)) return 1;
#endif
 return 0;
}
