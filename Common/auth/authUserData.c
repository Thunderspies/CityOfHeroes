/***************************************************************************
 *     Copyright (c) 2004-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include "auth/authUserData.h"
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/components/EString.h>

#ifdef SERVER
#include "comm_game.h"
#endif

// This holds the authbits set locally in servers.cfg - they will not be
// mergd into client auth data so they won't get pushed back up to the
// auth server.
U32 g_overridden_auth_bits[AUTH_DWORDS] = { 0 };
static int isReactivationActive;

// Word 0
#define WORD_WORD0_RESERVED                0
#define WORD_MAGIC_PACK                    0
#define WORD_SUPER_SCIENCE_PACK            0
#define WORD_MARTIAL_ARTS_PACK            0
#define WORD_CON_2009_BIT1                0
#define WORD_CON_2009_BIT2                0
#define WORD_CON_2009_BIT3                0
#define WORD_CON_2009_BIT4                0
#define WORD_CON_2009_BIT5                0
#define WORD_LOYALTY_REWARD                0
#define WORD_ROGUE_COMPLETE_BOX            0
#define WORD_GR_PO_RESISTANCE            0
#define WORD_GR_PO_MIGHT_EMPIRE            0
#define WORD_GR_PO_CLOCKWORK            0
#define WORD_GR_PO_WILL_SEERS            0
#define WORD_ROGUE_ACCESS                0
#define WORD_CREATE_PRIMALS                0
#define WORD_MUTANT_PACK                0
#define WORD_GHOUL_COSTUME                0
#define WORD_PRAETORIA_PD_COSTUME        0
#define WORD_RIKTI_COSTUME                0
#define WORD_VAHZILOK_MEAT_DOC_COSTUME    0
#define WORD_CORALAX_BOSS_COSTUME        0

#define BITS_WORD0_RESERVED                10
#define BITS_CORALAX_BOSS_COSTUME       1
#define BITS_VAHZILOK_MEAT_DOC_COSTUME  1
#define BITS_RIKTI_COSTUME              1
#define BITS_PRAETORIA_PD_COSTUME       1
#define BITS_GHOUL_COSTUME              1
#define BITS_MUTANT_PACK                1
#define BITS_CREATE_PRIMALS             1
#define BITS_ROGUE_ACCESS               1
#define BITS_GR_PO_WILL_SEERS           1
#define BITS_GR_PO_CLOCKWORK            1
#define BITS_GR_PO_MIGHT_EMPIRE         1
#define BITS_GR_PO_RESISTANCE           1
#define BITS_ROGUE_COMPLETE_BOX         1
#define BITS_LOYALTY_REWARD             1
#define BITS_CON_2009_BIT5              1
#define BITS_CON_2009_BIT4              1
#define BITS_CON_2009_BIT3              1        // HeroCon - Praetorian Clockwork
#define BITS_CON_2009_BIT2              1        // Comic-Con - Knives of Aretmis
#define BITS_CON_2009_BIT1              1        // PAX - Carnie Ring Mistress
#define BITS_MAGIC_PACK                 1
#define BITS_SUPER_SCIENCE_PACK         1
#define BITS_MARTIAL_ARTS_PACK          1

#define SHIFT_WORD0_RESERVED            0
#define SHIFT_CORALAX_BOSS_COSTUME      10
#define SHIFT_VAHZILOK_MEAT_DOC_COSTUME 11
#define SHIFT_RIKTI_COSTUME             12
#define SHIFT_PRAETORIA_PD_COSTUME      13
#define SHIFT_GHOUL_COSTUME             14
#define SHIFT_MUTANT_PACK               15
#define SHIFT_CREATE_PRIMALS            16
#define SHIFT_ROGUE_ACCESS              17
#define SHIFT_GR_PO_WILL_SEERS          18
#define SHIFT_GR_PO_CLOCKWORK           19
#define SHIFT_GR_PO_MIGHT_EMPIRE        20
#define SHIFT_GR_PO_RESISTANCE          21
#define SHIFT_ROGUE_COMPLETE_BOX        22
#define SHIFT_LOYALTY_REWARD            23
#define SHIFT_CON_2009_BIT5             24
#define SHIFT_CON_2009_BIT4             25
#define SHIFT_CON_2009_BIT3             26
#define SHIFT_CON_2009_BIT2             27
#define SHIFT_CON_2009_BIT1             28
#define SHIFT_MAGIC_PACK                29
#define SHIFT_SUPER_SCIENCE_PACK        30
#define SHIFT_MARTIAL_ARTS_PACK         31

// Word 1
#define WORD_MONTHS_PLAYED            1
#define WORD_RECREATION_PACK        1
#define WORD_HOLIDAY_PACK            1
#define WORD_AURA_PACK                1
#define WORD_VANGUARD_PACK            1
#define WORD_FACEPALM                1
#define WORD_ANIMAL_PACK            1
#define WORD_LIMITED_ROGUE_ACCESS    1
#define WORD_PROMO_2011_A            1
#define WORD_PROMO_2011_B            1
#define WORD_PROMO_2011_C            1
#define WORD_PROMO_2011_D            1
#define WORD_PROMO_2011_E            1
#define WORD_PROMO_2012_A            1
#define WORD_PROMO_2012_B            1
#define WORD_PROMO_2012_C            1
#define WORD_PROMO_2012_D            1
#define WORD_PROMO_2012_E            1
#define WORD_STEAM_PUNK                1

#define BITS_MONTHS_PLAYED            12    // 1    : Auth    --  Reserve 12 bits as a months played counter.  At 3 months per tick, that's 12288 months or 1024 years.
                                        //                    That'll keep us going for a millenium or so.                                                

#define BITS_RECREATION_PACK        1    // 13    : Auth
#define BITS_HOLIDAY_PACK            1    // 14    : Auth
#define BITS_AURA_PACK                1    // 15    : Auth
#define BITS_VANGUARD_PACK            1    // 16    : Auth

#define BITS_FACEPALM                1    // 17    : Auth
#define BITS_ANIMAL_PACK            1    // 18    : Auth
#define BITS_LIMITED_ROGUE_ACCESS    1    // 19    : Auth
#define BITS_STEAM_PUNK                1    // 20    : Auth
#define BITS_PROMO_2011_A            1    // 21    : Db + Auth
#define BITS_PROMO_2011_B            1    // 22    : Db + Auth
#define BITS_PROMO_2011_C            1    // 23    : Db + Auth
#define BITS_PROMO_2011_D            1    // 24    : Db + Auth
#define BITS_PROMO_2011_E            1    // 25    : Db + Auth
#define BITS_PROMO_2012_A            1    // 26    : Db + Auth
#define BITS_PROMO_2012_B            1    // 27    : Db + Auth
#define BITS_PROMO_2012_C            1    // 28    : Db + Auth
#define BITS_PROMO_2012_D            1    // 29    : Db + Auth
#define BITS_PROMO_2012_E            1    // 30    : Db + Auth



#define SHIFT_MONTHS_PLAYED            0
#define SHIFT_RECREATION_PACK        (SHIFT_MONTHS_PLAYED + BITS_MONTHS_PLAYED)
#define SHIFT_HOLIDAY_PACK            (SHIFT_RECREATION_PACK + BITS_RECREATION_PACK)
#define SHIFT_AURA_PACK                (SHIFT_HOLIDAY_PACK + BITS_HOLIDAY_PACK)
#define SHIFT_VANGUARD_PACK            (SHIFT_AURA_PACK + BITS_AURA_PACK)
#define SHIFT_FACEPALM                (SHIFT_VANGUARD_PACK + BITS_VANGUARD_PACK)
#define SHIFT_ANIMAL_PACK            (SHIFT_FACEPALM + BITS_FACEPALM)
#define SHIFT_LIMITED_ROGUE_ACCESS    (SHIFT_ANIMAL_PACK + BITS_ANIMAL_PACK)
#define SHIFT_STEAM_PUNK            (SHIFT_LIMITED_ROGUE_ACCESS + BITS_STEAM_PUNK)
#define SHIFT_PROMO_2011_A            (SHIFT_STEAM_PUNK + BITS_PROMO_2011_A)
#define SHIFT_PROMO_2011_B            (SHIFT_PROMO_2011_A + BITS_PROMO_2011_B)
#define SHIFT_PROMO_2011_C            (SHIFT_PROMO_2011_B + BITS_PROMO_2011_C)
#define SHIFT_PROMO_2011_D            (SHIFT_PROMO_2011_C + BITS_PROMO_2011_D)
#define SHIFT_PROMO_2011_E            (SHIFT_PROMO_2011_D + BITS_PROMO_2011_E)
#define SHIFT_PROMO_2012_A            (SHIFT_PROMO_2011_E + BITS_PROMO_2011_A)
#define SHIFT_PROMO_2012_B            (SHIFT_PROMO_2012_A + BITS_PROMO_2011_B)
#define SHIFT_PROMO_2012_C            (SHIFT_PROMO_2012_B + BITS_PROMO_2011_C)
#define SHIFT_PROMO_2012_D            (SHIFT_PROMO_2012_C + BITS_PROMO_2011_D)
#define SHIFT_PROMO_2012_E            (SHIFT_PROMO_2012_D + BITS_PROMO_2011_E)
// Word 2 - Reserved entirely for MonthsPlayed, one bit being 3 months.
#define MAX_SET_MONTHS_PLAYED   (32*3)

// if you get a list of the auth bits from NCA it will be in a format
// relatively hard to map to our internal format.
// you may get something like this 00000000000000000000000000b08080
//                                 0x00000000000000000000000008000000
// 
// mapping auth bits to these bits:
// auth word 3 = 00-b0-80-80
// nibbles:      12 34 56 78 
// bytes:         1  2  3  4
//
// leave the bytes in order, swap the nibbles, and count from left to right
// 00-b0-80-80 => 00-0b-08-08
//
// byte 4 says '80' from them, but swap nibbles:
// 08 => 0000-0001 (<= bit 32 is 1) : CoH access
//
// byte 3 says '80', but swap nibbles
// 08 => 0000-0001 (<= bit 24 is 1) : CoV access
//
// byte 2: b0
// 0b => 0000-1011 (bits 16,15,13) : jet pack, korean level min, pocket D
//
// the nca summary of this guy is: (they don't know about korean level min)
// 32: CoH Villain Access
// 24: CoH - Hero Access
// 13: CoH - Pocket D VIP pass
// 14: CoH - Jump Pack
 

// AuthUserData "00000000000000003f000000-00-b0-c0-98"
// AuthUserData "00000000000000003f000000-00-b0-c0-98"

// Word 3
#define WORD_ALPHA_PREORDER          3
#define WORD_CYBORG_PACK          3
#define WORD_MAC_PACK              3
#define WORD_IS_RETAIL_ACCOUNT    3
#define WORD_TARGET_PROMO          3
#define WORD_PAX_PROMO              3
#define WORD_GENCON_PROMO          3
#define WORD_COMICON_PROMO          3
#define WORD_ARACHNOS_ACCESS      3
#define WORD_WEDDING_PACK         3
#define WORD_TRIAL_ACCOUNT        3
#define WORD_POCKET_D_VIP         3
#define WORD_JUMP_PACK            3
#define WORD_JET_PACK             3
#define WORD_KOREAN_LEVEL_MINIMUM 3
#define WORD_VALENTINE_2006       3
#define WORD_COV_BETA             3
#define WORD_COV_PREORDER_1       3
#define WORD_COV_PREORDER_2       3
#define WORD_COV_PREORDER_3       3
#define WORD_COV_PREORDER_4       3
#define WORD_COV_SPECIAL_EDITION  3
#define WORD_VILLAIN_ACCESS       3
#define WORD_PREORDER             3
#define WORD_DVDSPECIALEDITION    3
#define WORD_KHELDIAN_ACCESS      3
#define WORD_BETA                 3
#define WORD_TRANSFER_TO_EU       3
#define WORD_HERO_ACCESS          3


#define BITS_WORD3_RESERVED_1      1
#define BITS_ALPHA_PREORDER           1    // 2    : Auth
#define BITS_CYBORG_PACK           1    // 3    : both
#define BITS_MAC_PACK               1    // 4    : both

#define BITS_IS_RETAIL_ACCOUNT     1    // 5    : auth
#define BITS_TARGET_PROMO           1    // 6    : auth set and unset by auth
#define BITS_PAX_PROMO               1    // 7    : both
#define BITS_GENCON_PROMO           1    // 8    : both

#define BITS_COMICON_PROMO           1    // 9    : both
#define BITS_ARACHNOS_ACCESS       1    // 10    : db
#define BITS_WEDDING_PACK          1    // 11    : both
#define BITS_TRIAL_ACCOUNT         1    // 12    : not used

#define BITS_POCKET_D_VIP          1    // 13    : both
#define BITS_JUMP_PACK             1    // 14    : both
#define BITS_JET_PACK              1    // 15    : auth    (Korean?)
#define BITS_KOREAN_LEVEL_MINIMUM  1    // 16 : db (at level 6 people have to pay auth)

#define BITS_VALENTINE_2006        1    // 17    : Auth
#define BITS_COV_BETA              1    // 18    : Auth
#define BITS_COV_PREORDER_1        1    // 19    : Auth
#define BITS_COV_PREORDER_2        1    // 20    : Auth

#define BITS_COV_PREORDER_3        1    // 21    : Auth
#define BITS_COV_PREORDER_4        1    // 22    : Auth
#define BITS_COV_SPECIAL_EDITION   1    // 23    : Auth
#define BITS_VILLAIN_ACCESS        1    // 24    : Auth

#define BITS_PREORDER              3    // 27-25    : Auth
#define BITS_DVDSPECIALEDITION     1    // 28    :Auth

#define BITS_KHELDIAN_ACCESS       1    // 29    :db
#define BITS_BETA                  1    // 30    : auth
#define BITS_TRANSFER_TO_EU        1    // 31    : auth
#define BITS_HERO_ACCESS           1    // 32    : auth


#define SHIFT_WORD3_RESERVED_1        0
#define SHIFT_ALPHA_PREORDER        (SHIFT_WORD3_RESERVED_1+BITS_WORD3_RESERVED_1)
#define SHIFT_CYBORG_PACK           (SHIFT_ALPHA_PREORDER+BITS_ALPHA_PREORDER)
#define SHIFT_MAC_PACK               (SHIFT_CYBORG_PACK+BITS_CYBORG_PACK)
#define SHIFT_IS_RETAIL_ACCOUNT    (SHIFT_MAC_PACK+BITS_MAC_PACK)
#define SHIFT_TARGET_PROMO         (SHIFT_IS_RETAIL_ACCOUNT+BITS_IS_RETAIL_ACCOUNT)
#define SHIFT_PAX_PROMO               (SHIFT_TARGET_PROMO+BITS_TARGET_PROMO)
#define SHIFT_GENCON_PROMO           (SHIFT_PAX_PROMO+BITS_PAX_PROMO)
#define SHIFT_COMICON_PROMO           (SHIFT_GENCON_PROMO+BITS_GENCON_PROMO)
#define SHIFT_ARACHNOS_ACCESS      (SHIFT_COMICON_PROMO+BITS_COMICON_PROMO)
#define SHIFT_WEDDING_PACK         (SHIFT_ARACHNOS_ACCESS+BITS_ARACHNOS_ACCESS)
#define SHIFT_TRIAL_ACCOUNT        (SHIFT_WEDDING_PACK+BITS_WEDDING_PACK)
#define SHIFT_POCKET_D_VIP         (SHIFT_TRIAL_ACCOUNT+BITS_TRIAL_ACCOUNT)
#define SHIFT_JUMP_PACK            (SHIFT_POCKET_D_VIP+BITS_POCKET_D_VIP)
#define SHIFT_JET_PACK             (SHIFT_JUMP_PACK+BITS_JUMP_PACK)
#define SHIFT_KOREAN_LEVEL_MINIMUM (SHIFT_JET_PACK+BITS_JET_PACK)
#define SHIFT_VALENTINE_2006       (SHIFT_KOREAN_LEVEL_MINIMUM+BITS_KOREAN_LEVEL_MINIMUM)
#define SHIFT_COV_BETA             (SHIFT_VALENTINE_2006+BITS_VALENTINE_2006)
#define SHIFT_COV_PREORDER_1       (SHIFT_COV_BETA+BITS_COV_BETA)
#define SHIFT_COV_PREORDER_2       (SHIFT_COV_PREORDER_1+BITS_COV_PREORDER_1)
#define SHIFT_COV_PREORDER_3       (SHIFT_COV_PREORDER_2+BITS_COV_PREORDER_2)
#define SHIFT_COV_PREORDER_4       (SHIFT_COV_PREORDER_3+BITS_COV_PREORDER_3)
#define SHIFT_COV_SPECIAL_EDITION  (SHIFT_COV_PREORDER_4+BITS_COV_PREORDER_4)
#define SHIFT_VILLAIN_ACCESS       (SHIFT_COV_SPECIAL_EDITION+BITS_COV_SPECIAL_EDITION)
#define SHIFT_PREORDER             (SHIFT_VILLAIN_ACCESS+BITS_VILLAIN_ACCESS)
#define SHIFT_DVDSPECIALEDITION    (SHIFT_PREORDER+BITS_PREORDER)
#define SHIFT_KHELDIAN_ACCESS      (SHIFT_DVDSPECIALEDITION+BITS_DVDSPECIALEDITION)
#define SHIFT_BETA                 (SHIFT_KHELDIAN_ACCESS+BITS_KHELDIAN_ACCESS)
#define SHIFT_TRANSFER_TO_EU       (SHIFT_BETA+BITS_BETA)
#define SHIFT_HERO_ACCESS          (SHIFT_TRANSFER_TO_EU+BITS_TRANSFER_TO_EU)


// Word 4
#define WORD_ACEINHOLE_A            4
#define WORD_ACEINHOLE_B            4
#define WORD_ACEINHOLE_C            4
#define WORD_ACEINHOLE_D            4
#define WORD_ACEINHOLE_E            4

#define BITS_ACEINHOLE_A            1    // 1    : Db + Auth
#define BITS_ACEINHOLE_B            1    // 2    : Db + Auth
#define BITS_ACEINHOLE_C            1    // 3    : Db + Auth
#define BITS_ACEINHOLE_D            1    // 4    : Db + Auth
#define BITS_ACEINHOLE_E            1    // 5    : Db + Auth

#define SHIFT_ACEINHOLE_A            0
#define SHIFT_ACEINHOLE_B            (SHIFT_ACEINHOLE_A + BITS_ACEINHOLE_B)
#define SHIFT_ACEINHOLE_C            (SHIFT_ACEINHOLE_B + BITS_ACEINHOLE_C)
#define SHIFT_ACEINHOLE_D            (SHIFT_ACEINHOLE_C + BITS_ACEINHOLE_D)
#define SHIFT_ACEINHOLE_E            (SHIFT_ACEINHOLE_D + BITS_ACEINHOLE_E)


#define SET_BITS(name, val) { \
    U32 mask = (0xffffffffUL & ((1<<BITS_##name)-1)) << SHIFT_##name; \
    U32 u = val & ((1<<BITS_##name)-1); \
    u <<= SHIFT_##name; \
    data[WORD_##name] = (data[WORD_##name] & ~mask) | u; }

#define GET_BITS(name) \
    (((data[WORD_##name] >> SHIFT_##name ) & ((1<<BITS_##name)-1)) | \
    ((g_overridden_auth_bits[WORD_##name] >> SHIFT_##name) & ((1<<BITS_##name)-1)))

#define SET_WORD(name, val) { \
    data[WORD_##name] = val; }

#define GET_WORD(name) \
    data[WORD_##name]

//MS: NOTE, these are the bits that will be set both by us and the Auth db.
//All words are shown BIG-ENDIAN.
//00000001 00000000 00000000 00000000 Con2009Bit5
//00000002 00000000 00000000 00000000 Con2009Bit4
//00000004 00000000 00000000 00000000 Con2009Bit3
//00000008 00000000 00000000 00000000 Con2009Bit2
//00000010 00000000 00000000 00000000 Con2009Bit1
//00000020 00000000 00000000 00000000 MagicPack
//00000040 00000000 00000000 00000000 SupersciencePack
//00000080 00000000 00000000 00000000 MartialArtsPack
//00000100 00000000 00000000 00000000 CreatePrimals
//00000200 00000000 00000000 00000000 RogueAccess
//00000400 00000000 00000000 00000000 GRPreOrderWillOfTheSeers
//00000800 00000000 00000000 00000000 GRPreOrderClockworkEfficiency
//00001000 00000000 00000000 00000000 GRPreOrderMightOfTheEmpire
//00002000 00000000 00000000 00000000 GRPreOrderResistanceTactics
//00004000 00000000 00000000 00000000 RogueCompleteBox
//00008000 00000000 00000000 00000000 LoyaltyReward
//00040000 00000000 00000000 00000000 CoralaxBossCostume
//00080000 00000000 00000000 00000000 VahzilokMeatDocCostume
//00100000 00000000 00000000 00000000 RiktiCostume
//00200000 00000000 00000000 00000000 PraetoriaPDCostume
//00400000 00000000 00000000 00000000 GhoulCostume
//00800000 00000000 00000000 00000000 MutantPack

//00000000 ff0f0000 00000000 00000000 Months played
//00000000 00100000 00000000 00000000 Recreation pack
//00000000 00200000 00000000 00000000 Holiday pack
//00000000 00400000 00000000 00000000 Aura pack
//00000000 00800000 00000000 00000000 Vanguard pack

//00000000 00000100 00000000 00000000 Facepalm
//00000000 00000200 00000000 00000000 Animal pack
//00000000 00000400 00000000 00000000 Limited Rogue Access


//00000000 00000000 00000000 20000000 GRPreOrderSyndicateTechniques
//00000000 00000000 00000000 40000000 CyborgPack
//00000000 00000000 00000000 80000000 MacPack

//00000000 00000000 00000000 01000000 IsRetailAccount
//00000000 00000000 00000000 02000000 TargetPromo
//00000000 00000000 00000000 04000000 PaxPromo
//00000000 00000000 00000000 08000000 GenConPromo

//00000000 00000000 00000000 00010000 ComiconPromo
//00000000 00000000 00000000 00020000 ArachnosAccess
//00000000 00000000 00000000 00040000 WeddingPack
//00000000 00000000 00000000 00080000 TrialAccount

//00000000 00000000 00000000 00100000 PocketDVIP
//00000000 00000000 00000000 00200000 JumpPack
//00000000 00000000 00000000 00400000 JetPack
//00000000 00000000 00000000 00800000 KoreanLevelMinimum

//00000000 00000000 00000000 00000100 Valentine2006
//00000000 00000000 00000000 00000200 CovBeta
//00000000 00000000 00000000 00000400 CovPreorder1
//00000000 00000000 00000000 00000800 CovPreorder2

//00000000 00000000 00000000 00001000 CovPreorder3
//00000000 00000000 00000000 00002000 CovPreorder4
//00000000 00000000 00000000 00004000 CovSpecialEdition
//00000000 00000000 00000000 00008000 VillainAccess

//00000000 00000000 00000000 00000008 DVDSpecialEdition
//00000000 00000000 00000000 00000010 Kheldian
//00000000 00000000 00000000 00000020 Beta
//00000000 00000000 00000000 00000040 TransferToEU
//00000000 00000000 00000000 00000080 HeroAccess

#ifdef DBSERVER
#define TRANSLATE_GET_BITS(name) (1<<BITS_##name)-1
#define TRANSLATE_BITS(name) ((TRANSLATE_GET_BITS(name)) << SHIFT_##name)

//the bits owned by the Auth server
static U32 authUserBitMaskAuth[] = {
    // /e headdesk.  This used to be word 1, however we are C programmers round here, and number stuff from 0.
    // It should be noted that authUserBitMaskGame immediately below gets it right, so I have changed this guy
    // to match.  Maybe, just maybe, it'll reduce the universal level of confusion regarding authbits.
    //the following bits in word 0 are owned by Auth
      TRANSLATE_BITS(WORD0_RESERVED)

    | TRANSLATE_BITS(LOYALTY_REWARD)
    | TRANSLATE_BITS(CON_2009_BIT1)
    | TRANSLATE_BITS(CON_2009_BIT2)
    | TRANSLATE_BITS(CON_2009_BIT3)
    | TRANSLATE_BITS(CON_2009_BIT4)
    | TRANSLATE_BITS(CON_2009_BIT5)
    | TRANSLATE_BITS(MAGIC_PACK)
    | TRANSLATE_BITS(SUPER_SCIENCE_PACK)
    | TRANSLATE_BITS(MARTIAL_ARTS_PACK)
    | TRANSLATE_BITS(ROGUE_COMPLETE_BOX)
    | TRANSLATE_BITS(GR_PO_RESISTANCE)
    | TRANSLATE_BITS(GR_PO_MIGHT_EMPIRE)
    | TRANSLATE_BITS(GR_PO_CLOCKWORK)
    | TRANSLATE_BITS(GR_PO_WILL_SEERS)
    | TRANSLATE_BITS(ROGUE_ACCESS)
    | TRANSLATE_BITS(MUTANT_PACK)
    | TRANSLATE_BITS(GHOUL_COSTUME)
    | TRANSLATE_BITS(PRAETORIA_PD_COSTUME)
    | TRANSLATE_BITS(RIKTI_COSTUME)
    | TRANSLATE_BITS(VAHZILOK_MEAT_DOC_COSTUME)
    | TRANSLATE_BITS(CORALAX_BOSS_COSTUME)
    ,

    //word 1
      TRANSLATE_BITS(RECREATION_PACK)
    | TRANSLATE_BITS(HOLIDAY_PACK)
    | TRANSLATE_BITS(AURA_PACK)
    | TRANSLATE_BITS(VANGUARD_PACK)
    | TRANSLATE_BITS(FACEPALM)
    | TRANSLATE_BITS(ANIMAL_PACK)
    | TRANSLATE_BITS(LIMITED_ROGUE_ACCESS)
    | TRANSLATE_BITS(PROMO_2011_A)
    | TRANSLATE_BITS(PROMO_2011_B)
    | TRANSLATE_BITS(PROMO_2011_C)
    | TRANSLATE_BITS(PROMO_2011_D)
    | TRANSLATE_BITS(PROMO_2011_E)
    | TRANSLATE_BITS(PROMO_2012_A)
    | TRANSLATE_BITS(PROMO_2012_B)
    | TRANSLATE_BITS(PROMO_2012_C)
    | TRANSLATE_BITS(PROMO_2012_D)
    | TRANSLATE_BITS(PROMO_2012_E)
    | TRANSLATE_BITS(STEAM_PUNK)
    ,

    0xffffffff,    //word 2 (months active)

    //word 3 is shared by auth and db.
    //the following are the bits owned by Auth
      TRANSLATE_BITS(WORD3_RESERVED_1)
    | TRANSLATE_BITS(ALPHA_PREORDER)
    | TRANSLATE_BITS(CYBORG_PACK) 
    | TRANSLATE_BITS(MAC_PACK) 
    | TRANSLATE_BITS(IS_RETAIL_ACCOUNT) 
    | TRANSLATE_BITS(TARGET_PROMO) 
    | TRANSLATE_BITS(PAX_PROMO) 
    | TRANSLATE_BITS(GENCON_PROMO) 
    | TRANSLATE_BITS(COMICON_PROMO) 
    | TRANSLATE_BITS(WEDDING_PACK) 
    | TRANSLATE_BITS(TRIAL_ACCOUNT) 
    | TRANSLATE_BITS(POCKET_D_VIP) 
    | TRANSLATE_BITS(JUMP_PACK) 
    | TRANSLATE_BITS(JET_PACK) 
    | TRANSLATE_BITS(VALENTINE_2006) 
    | TRANSLATE_BITS(COV_BETA) 
    | TRANSLATE_BITS(COV_PREORDER_1) 
    | TRANSLATE_BITS(COV_PREORDER_2)
    | TRANSLATE_BITS(COV_PREORDER_3) 
    | TRANSLATE_BITS(COV_PREORDER_4) 
    | TRANSLATE_BITS(COV_SPECIAL_EDITION) 
    | TRANSLATE_BITS(VILLAIN_ACCESS) 
    | TRANSLATE_BITS(PREORDER) 
    | TRANSLATE_BITS(DVDSPECIALEDITION)  
    | TRANSLATE_BITS(BETA) 
    | TRANSLATE_BITS(TRANSFER_TO_EU) 
    | TRANSLATE_BITS(HERO_ACCESS)
    ,

    TRANSLATE_BITS(ACEINHOLE_A)
    | TRANSLATE_BITS(ACEINHOLE_B)
    | TRANSLATE_BITS(ACEINHOLE_C)
    | TRANSLATE_BITS(ACEINHOLE_D)
    | TRANSLATE_BITS(ACEINHOLE_E)
    ,
    0xffffffff,    //word 5
    0xffffffff,    //word 6
    0xffffffff,    //word 7
    0xffffffff,    //word 8
    0xffffffff,    //word 9
    0xffffffff,    //word 10
    0xffffffff,    //word 11
    0xffffffff,    //word 12
    0xffffffff,    //word 13
    0xffffffff,    //word 14
    0xffffffff,    //word 15
    0xffffffff,    //word 16
    0xffffffff,    //word 17
    0xffffffff,    //word 18
    0xffffffff,    //word 19
    0xffffffff,    //word 20
    0xffffffff,    //word 21
    0xffffffff,    //word 22
    0xffffffff,    //word 23
    0xffffffff,    //word 24
    0xffffffff,    //word 25
    0xffffffff,    //word 26
    0xffffffff,    //word 27
    0xffffffff,    //word 28
    0xffffffff,    //word 29
    0xffffffff,    //word 30
    0xffffff00,    //word 31    -- The top byte is not writable by the Authserver for now, this is for testing


};

//the bits owned by the Db server
static U32 authUserBitMaskGame[] = {
    TRANSLATE_BITS(CREATE_PRIMALS),    //word 0
    0x00000000,    //word 1
    0x00000000,    //word 2

    //word 3 is shared by auth and db.
    //the following are the bits owned by db
    TRANSLATE_BITS(CYBORG_PACK) 
    | TRANSLATE_BITS(MAC_PACK) 
    | TRANSLATE_BITS(PAX_PROMO) 
    | TRANSLATE_BITS(GENCON_PROMO) 
    | TRANSLATE_BITS(COMICON_PROMO) 
    | TRANSLATE_BITS(ARACHNOS_ACCESS) 
    | TRANSLATE_BITS(WEDDING_PACK) 
    | TRANSLATE_BITS(TRIAL_ACCOUNT) 
    | TRANSLATE_BITS(POCKET_D_VIP) 
    | TRANSLATE_BITS(JUMP_PACK)  
    | TRANSLATE_BITS(KOREAN_LEVEL_MINIMUM) 
    | TRANSLATE_BITS(KHELDIAN_ACCESS)
    | TRANSLATE_BITS(PROMO_2011_A)
    | TRANSLATE_BITS(PROMO_2011_B)
    | TRANSLATE_BITS(PROMO_2011_C)
    | TRANSLATE_BITS(PROMO_2011_D)
    | TRANSLATE_BITS(PROMO_2011_E)
    | TRANSLATE_BITS(PROMO_2012_A)
    | TRANSLATE_BITS(PROMO_2012_B)
    | TRANSLATE_BITS(PROMO_2012_C)
    | TRANSLATE_BITS(PROMO_2012_D)
    | TRANSLATE_BITS(PROMO_2012_E)
    ,

    TRANSLATE_BITS(ACEINHOLE_A)
    | TRANSLATE_BITS(ACEINHOLE_B)
    | TRANSLATE_BITS(ACEINHOLE_C)
    | TRANSLATE_BITS(ACEINHOLE_D)
    | TRANSLATE_BITS(ACEINHOLE_E)
    ,
    0x00000000, //word 5
    0x00000000, //word 6
    0x00000000, //word 7
    0x00000000, //word 8
    0x00000000, //word 9
    0x00000000, //word 10
    0x00000000, //word 11
    0x00000000, //word 12
    0x00000000, //word 13
    0x00000000,    //word 14
    0x00000000, //word 15
    0x00000000, //word 16
    0x00000000, //word 17
    0x00000000,    //word 18
    0x00000000, //word 19
    0x00000000, //word 20
    0x00000000, //word 21
    0x00000000,    //word 22
    0x00000000, //word 23
    0x00000000, //word 24
    0x00000000, //word 25
    0x00000000,    //word 26
    0x00000000, //word 27
    0x00000000, //word 28
    0x00000000, //word 29
    0x00000000,    //word 30
    0x0000ffff,    //word 31    -- The top two bytes are writable by the game client, for test purposes

};

void authUserMaskAuthBits(U8 *bitsToMask, int owner)
{
    //we want to build these in U32 since this is 
    //  how our definition works.  However, we store the 
    //  Auth format data in U8.
    //I choose to recast here because I want to re-enforce 
    // that this should be used with the Auth format.
    U8 *mask = (U8 *) ((owner == AUTH_USER_AUTHSERVER )? authUserBitMaskAuth:authUserBitMaskGame);
    int i;

    STATIC_INFUNC_ASSERT(sizeof(authUserBitMaskAuth) == AUTH_BYTES);
    STATIC_INFUNC_ASSERT(sizeof(authUserBitMaskGame) == AUTH_BYTES);

    if(!bitsToMask)
        return;

    for(i = 0; i < AUTH_BYTES; i++)
    {
        bitsToMask[i] &= mask[i];
    }
}

#endif


int authUserGetFieldByName(U32 *data, const char *field)
{
#define ELSE_HASBIT(xx)  else if(stricmp(field, #xx)==0) return authUserHas##xx(data);

    if(stricmp(field,"MonthsPlayed")==0)
    {
        return authUserMonthsPlayed(data);
    }
    else if(stricmp(field, "Preorder:EB")==0)
    {
        return (authUserPreorderCode(data) == PREORDER_ELECTRONICSBOUTIQUE);
    }
    else if(stricmp(field, "Preorder:GameStop")==0)
    {
        return (authUserPreorderCode(data) == PREORDER_GAMESTOP);
    }
    else if(stricmp(field, "Preorder:BestBuy")==0)
    {
        return (authUserPreorderCode(data) == PREORDER_BESTBUY);
    }
    else if(stricmp(field, "Preorder:Generic")==0)
    {
        return (authUserPreorderCode(data) == PREORDER_GENERIC);
    }
    else if(stricmp(field,"PreorderCode")==0)
    {
        return authUserPreorderCode(data);
    }
    ELSE_HASBIT(RogueCompleteBox)
    ELSE_HASBIT(LoyaltyReward)
    ELSE_HASBIT(Con2009Bit1)
    ELSE_HASBIT(Con2009Bit2)
    ELSE_HASBIT(Con2009Bit3)
    ELSE_HASBIT(Con2009Bit4)
    ELSE_HASBIT(Con2009Bit5)
    ELSE_HASBIT(MagicPack)
    ELSE_HASBIT(SupersciencePack)
    ELSE_HASBIT(MartialArtsPack)
    ELSE_HASBIT(GRPreOrderSyndicateTechniques)
    ELSE_HASBIT(CyborgPack)
    ELSE_HASBIT(MacPack)
    ELSE_HASBIT(IsRetailAccount)
    ELSE_HASBIT(TargetPromo)
    ELSE_HASBIT(PaxPromo)
    ELSE_HASBIT(GenConPromo)
    ELSE_HASBIT(ComiconPromo)
    ELSE_HASBIT(ArachnosAccess)
    ELSE_HASBIT(WeddingPack)
    ELSE_HASBIT(TrialAccount)
    ELSE_HASBIT(PocketDVIP)
    ELSE_HASBIT(JumpPack)
    ELSE_HASBIT(JetPack)
    ELSE_HASBIT(KoreanLevelMinimum)
    ELSE_HASBIT(Valentine2006)
    ELSE_HASBIT(CovBeta)
    ELSE_HASBIT(CovPreorder1)
    ELSE_HASBIT(CovPreorder2)
    ELSE_HASBIT(CovPreorder3)
    ELSE_HASBIT(CovPreorder4)
    ELSE_HASBIT(CovSpecialEdition)
    ELSE_HASBIT(VillainAccess)
    ELSE_HASBIT(DVDSpecialEdition)
    ELSE_HASBIT(Kheldian)
    ELSE_HASBIT(Beta)
    ELSE_HASBIT(TransferToEU)
    ELSE_HASBIT(HeroAccess)
    ELSE_HASBIT(GRPreOrderResistanceTactics)
    ELSE_HASBIT(GRPreOrderMightOfTheEmpire)
    ELSE_HASBIT(GRPreOrderClockworkEfficiency)
    ELSE_HASBIT(GRPreOrderWillOfTheSeers)
    ELSE_HASBIT(RogueAccess)
//    ELSE_HASBIT(CreatePrimals)
    ELSE_HASBIT(MutantPack)
    ELSE_HASBIT(GhoulCostume)
    ELSE_HASBIT(PraetoriaPDCostume)
    ELSE_HASBIT(RiktiCostume)
    ELSE_HASBIT(VahzilokMeatDocCostume)
    ELSE_HASBIT(CoralaxBossCostume)
    ELSE_HASBIT(RecreationPack)
    ELSE_HASBIT(HolidayPack)
    ELSE_HASBIT(AuraPack)
    ELSE_HASBIT(VanguardPack)
    ELSE_HASBIT(Facepalm)
    ELSE_HASBIT(AnimalPack)
    ELSE_HASBIT(LimitedRogueAccess)
    ELSE_HASBIT(Promo2011A)
    ELSE_HASBIT(Promo2011B)
    ELSE_HASBIT(Promo2011C)
    ELSE_HASBIT(Promo2011D)
    ELSE_HASBIT(Promo2011E)
    ELSE_HASBIT(Promo2012A)
    ELSE_HASBIT(Promo2012B)
    ELSE_HASBIT(Promo2012C)
    ELSE_HASBIT(Promo2012D)
    ELSE_HASBIT(Promo2012E)
    ELSE_HASBIT(SteamPunk)
    ELSE_HASBIT(AceInHoleA)
    ELSE_HASBIT(AceInHoleB)
    ELSE_HASBIT(AceInHoleC)
    ELSE_HASBIT(AceInHoleD)
    ELSE_HASBIT(AceInHoleE)
    else
    {
        return -1;
    }
}

bool authUserSetFieldByName(U32 *data, const char *field, int i)
{
#define ELSE_SETBIT(xx)  else if(stricmp(field, #xx)==0) authUserSet##xx(data,i);

    if(stricmp(field, "Raw0")==0)
    {
        data[0]=i;
    }
    else if(stricmp(field, "Raw1")==0)
    {
        data[1]=i;
    }
    else if(stricmp(field, "Raw2")==0)
    {
        data[2]=i;
    }
    else if(stricmp(field, "Raw3")==0)
    {
        data[3]=i;
    }
    else if(stricmp(field, "OrRaw0")==0)
    {
        data[0]|=i;
    }
    else if(stricmp(field, "OrRaw1")==0)
    {
        data[1]|=i;
    }
    else if(stricmp(field, "OrRaw2")==0)
    {
        data[2]|=i;
    }
    else if(stricmp(field, "OrRaw3")==0)
    {
        data[3]|=i;
    }
    else if(stricmp(field,"MonthsPlayed")==0)
    {
        authUserSetMonthsPlayed(data,i);
    }
    else if(stricmp(field, "Preorder:EB")==0)
    {
        authUserSetPreorderCode(data, i ? PREORDER_ELECTRONICSBOUTIQUE : 0);
    }
    else if(stricmp(field, "Preorder:GameStop")==0)
    {
        authUserSetPreorderCode(data, i ? PREORDER_GAMESTOP : 0);
    }
    else if(stricmp(field, "Preorder:BestBuy")==0)
    {
        authUserSetPreorderCode(data, i ? PREORDER_BESTBUY : 0);
    }
    else if(stricmp(field, "Preorder:Generic")==0)
    {
        authUserSetPreorderCode(data, i ? PREORDER_GENERIC : 0);
    }
    ELSE_SETBIT(RogueCompleteBox)
    ELSE_SETBIT(LoyaltyReward)
    ELSE_SETBIT(Con2009Bit1)
    ELSE_SETBIT(Con2009Bit2)
    ELSE_SETBIT(Con2009Bit3)
    ELSE_SETBIT(Con2009Bit4)
    ELSE_SETBIT(Con2009Bit5)
    ELSE_SETBIT(MagicPack)
    ELSE_SETBIT(SupersciencePack)
    ELSE_SETBIT(MartialArtsPack)
    ELSE_SETBIT(GRPreOrderSyndicateTechniques)
    ELSE_SETBIT(CyborgPack)
    ELSE_SETBIT(MacPack)
    ELSE_SETBIT(IsRetailAccount)
    ELSE_SETBIT(TargetPromo)
    ELSE_SETBIT(PaxPromo)
    ELSE_SETBIT(GenConPromo)
    ELSE_SETBIT(ComiconPromo)
    ELSE_SETBIT(ArachnosAccess)
    ELSE_SETBIT(WeddingPack)
    ELSE_SETBIT(TrialAccount)
    ELSE_SETBIT(PocketDVIP)
    ELSE_SETBIT(JumpPack)
    ELSE_SETBIT(JetPack)
    ELSE_SETBIT(KoreanLevelMinimum)
    ELSE_SETBIT(Valentine2006)
    ELSE_SETBIT(CovBeta)
    ELSE_SETBIT(CovPreorder1)
    ELSE_SETBIT(CovPreorder2)
    ELSE_SETBIT(CovPreorder3)
    ELSE_SETBIT(CovPreorder4)
    ELSE_SETBIT(CovSpecialEdition)
    ELSE_SETBIT(VillainAccess)
    ELSE_SETBIT(PreorderCode)
    ELSE_SETBIT(DVDSpecialEdition)
    ELSE_SETBIT(Kheldian)
    ELSE_SETBIT(Beta)
    ELSE_SETBIT(TransferToEU)
    ELSE_SETBIT(HeroAccess)
    ELSE_SETBIT(GRPreOrderResistanceTactics)
    ELSE_SETBIT(GRPreOrderMightOfTheEmpire)
    ELSE_SETBIT(GRPreOrderClockworkEfficiency)
    ELSE_SETBIT(GRPreOrderWillOfTheSeers)
    ELSE_SETBIT(RogueAccess)
//    ELSE_SETBIT(CreatePrimals)
    ELSE_SETBIT(MutantPack)
    ELSE_SETBIT(GhoulCostume)
    ELSE_SETBIT(PraetoriaPDCostume)
    ELSE_SETBIT(RiktiCostume)
    ELSE_SETBIT(VahzilokMeatDocCostume)
    ELSE_SETBIT(CoralaxBossCostume)
    ELSE_SETBIT(RecreationPack)
    ELSE_SETBIT(HolidayPack)
    ELSE_SETBIT(AuraPack)
    ELSE_SETBIT(VanguardPack)
    ELSE_SETBIT(Facepalm)
    ELSE_SETBIT(AnimalPack)
    ELSE_SETBIT(LimitedRogueAccess)
    ELSE_SETBIT(Promo2011A)
    ELSE_SETBIT(Promo2011B)
    ELSE_SETBIT(Promo2011C)
    ELSE_SETBIT(Promo2011D)
    ELSE_SETBIT(Promo2011E)
    ELSE_SETBIT(Promo2012A)
    ELSE_SETBIT(Promo2012B)
    ELSE_SETBIT(Promo2012C)
    ELSE_SETBIT(Promo2012D)
    ELSE_SETBIT(Promo2012E)
    ELSE_SETBIT(SteamPunk)
    ELSE_SETBIT(AceInHoleA)
    ELSE_SETBIT(AceInHoleB)
    ELSE_SETBIT(AceInHoleC)
    ELSE_SETBIT(AceInHoleD)
    ELSE_SETBIT(AceInHoleE)
    else
    {
        return false;
    }
    STATIC_INFUNC_ASSERT(BITS_WORD3_RESERVED_1 == 1); // update this if new bits added  
    return true;
}

void authUserSetReactivationActive(int isActive) {
    isReactivationActive = isActive;
}

int authUserIsReactivationActive() {
    return isReactivationActive;
}

#ifndef DBSERVER
#include <utilitieslib/language/AppLocale.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/file.h>

#define SECONDS_PER_YEAR    31556926
#define SECONDS_PER_3MONTHS    (SECONDS_PER_YEAR/4)
#define SECONDS_PER_WEEK    604800
#endif

int authUserMonthsPlayed(U32 *data)
{
    U32 u;
    int i = 0;
    for(u = data[2]; u; u >>= 1)
        i++;

#ifndef DBSERVER
    if(getCurrentLocale() != LOCALE_ID_KOREAN && isProductionMode())
    {
        static U32 uSecsToLaunch = 0;
        U32 now = timerSecondsSince2000();
        int max_months;

        if(uSecsToLaunch == 0)
        {
            struct tm d = {0};
            d.tm_mon = 3;    // tm is 0-based for month
            d.tm_year = 104; // tm is 1900-based for year
            d.tm_mday = 28;   // tm is 1 based for day. Go figure.
            uSecsToLaunch = timerGetSecondsSince2000FromTimeStruct(&d);
        }

        max_months = (now+SECONDS_PER_WEEK-uSecsToLaunch)/SECONDS_PER_3MONTHS;
        if(i > max_months)
            i = max_months;
    }
#endif

    return i*3;
}

void authUserSetMonthsPlayed(U32 *data, int i)
{
    if(i > MAX_SET_MONTHS_PLAYED)
        i = MAX_SET_MONTHS_PLAYED;
    data[2] = (1<<(i/3))-1;
}

bool authUserHasCon2009Bit1(U32 *data)
{
    return GET_BITS(CON_2009_BIT1);
}

void authUserSetCon2009Bit1(U32 *data, int i)
{
    SET_BITS(CON_2009_BIT1, i);
}

bool authUserHasCon2009Bit2(U32 *data)
{
    return GET_BITS(CON_2009_BIT2);
}

void authUserSetCon2009Bit2(U32 *data, int i)
{
    SET_BITS(CON_2009_BIT2, i);
}

bool authUserHasCon2009Bit3(U32 *data)
{
    return GET_BITS(CON_2009_BIT3);
}

void authUserSetCon2009Bit3(U32 *data, int i)
{
    SET_BITS(CON_2009_BIT3, i);
}

bool authUserHasCon2009Bit4(U32 *data)
{
    return GET_BITS(CON_2009_BIT4);
}

void authUserSetCon2009Bit4(U32 *data, int i)
{
    SET_BITS(CON_2009_BIT4, i);
}

bool authUserHasCon2009Bit5(U32 *data)
{
    return GET_BITS(CON_2009_BIT5);
}

void authUserSetCon2009Bit5(U32 *data, int i)
{
    SET_BITS(CON_2009_BIT5, i);
}

bool authUserHasLoyaltyReward(U32 *data)
{
    return GET_BITS(LOYALTY_REWARD);
}

void authUserSetLoyaltyReward(U32 *data, int i)
{
    SET_BITS(LOYALTY_REWARD, i);
}

bool authUserHasMagicPack(U32 *data)
{
    return GET_BITS(MAGIC_PACK);
}

void authUserSetMagicPack(U32 *data, int i)
{
    SET_BITS(MAGIC_PACK, i);
}

bool authUserHasSupersciencePack(U32 *data)
{
    return GET_BITS(SUPER_SCIENCE_PACK);
}

void authUserSetSupersciencePack(U32 *data, int i)
{
    SET_BITS(SUPER_SCIENCE_PACK, i);
}

bool authUserHasMartialArtsPack(U32 *data)
{
    return GET_BITS(MARTIAL_ARTS_PACK);
}

void authUserSetMartialArtsPack(U32 *data, int i)
{
    SET_BITS(MARTIAL_ARTS_PACK, i);
}

bool authUserHasGRPreOrderSyndicateTechniques(U32 *data)
{
    return GET_BITS(ALPHA_PREORDER);
}

void authUserSetGRPreOrderSyndicateTechniques(U32 *data, int i)
{
    SET_BITS(ALPHA_PREORDER, i);
}

bool authUserHasRogueCompleteBox(U32 *data)
{
    return GET_BITS(ROGUE_COMPLETE_BOX);
}

void authUserSetRogueCompleteBox(U32 *data, int i)
{
    SET_BITS(ROGUE_COMPLETE_BOX, i);
}

bool authUserHasGRPreOrderResistanceTactics(U32 *data)
{
    return GET_BITS(GR_PO_RESISTANCE);
}

void authUserSetGRPreOrderResistanceTactics(U32 *data, int i)
{
    SET_BITS(GR_PO_RESISTANCE, i);
}

bool authUserHasGRPreOrderMightOfTheEmpire(U32 *data)
{
    return GET_BITS(GR_PO_MIGHT_EMPIRE);
}

void authUserSetGRPreOrderMightOfTheEmpire(U32 *data, int i)
{
    SET_BITS(GR_PO_MIGHT_EMPIRE, i);
}
bool authUserHasGRPreOrderClockworkEfficiency(U32 *data)
{
    return GET_BITS(GR_PO_CLOCKWORK);
}

void authUserSetGRPreOrderClockworkEfficiency(U32 *data, int i)
{
    SET_BITS(GR_PO_CLOCKWORK, i);
}

bool authUserHasGRPreOrderWillOfTheSeers(U32 *data)
{
    return GET_BITS(GR_PO_WILL_SEERS);
}

void authUserSetGRPreOrderWillOfTheSeers(U32 *data, int i)
{
    SET_BITS(GR_PO_WILL_SEERS, i);
}

bool authUserHasRogueAccess(U32 *data)
{
    return GET_BITS(ROGUE_ACCESS);
}

bool authUserHasProvisionalRogueAccess(U32 *data)
{
    return authUserHasRogueAccess(data) || 
        authUserHasLimitedRogueAccess(data) ||
        authUserIsReactivationActive();
}

void authUserSetRogueAccess(U32 *data, int i)
{
    SET_BITS(ROGUE_ACCESS, i);
}

/*
// CREATE_PRIMALS auth bit is now deprecated
bool authUserHasCreatePrimals(U32 *data)
{
    return GET_BITS(CREATE_PRIMALS);
}

void authUserSetCreatePrimals(U32 *data, int i)
{
    SET_BITS(CREATE_PRIMALS, i);
}
*/

bool authUserHasMutantPack(U32 *data)
{
    return GET_BITS(MUTANT_PACK);
}

void authUserSetMutantPack(U32 *data, int i)
{
    SET_BITS(MUTANT_PACK, i);
}

bool authUserHasGhoulCostume(U32 *data)
{
    return GET_BITS(GHOUL_COSTUME);
}

void authUserSetGhoulCostume(U32 *data, int i)
{
    SET_BITS(GHOUL_COSTUME, i);
}

bool authUserHasPraetoriaPDCostume(U32 *data)
{
    return GET_BITS(PRAETORIA_PD_COSTUME);
}

void authUserSetPraetoriaPDCostume(U32 *data, int i)
{
    SET_BITS(PRAETORIA_PD_COSTUME, i);
}

bool authUserHasRiktiCostume(U32 *data)
{
    return GET_BITS(RIKTI_COSTUME);
}

void authUserSetRiktiCostume(U32 *data, int i)
{
    SET_BITS(RIKTI_COSTUME, i);
}

bool authUserHasVahzilokMeatDocCostume(U32 *data)
{
    return GET_BITS(VAHZILOK_MEAT_DOC_COSTUME);
}

void authUserSetVahzilokMeatDocCostume(U32 *data, int i)
{
    SET_BITS(VAHZILOK_MEAT_DOC_COSTUME, i);
}

bool authUserHasCoralaxBossCostume(U32 *data)
{
    return GET_BITS(CORALAX_BOSS_COSTUME);
}

void authUserSetCoralaxBossCostume(U32 *data, int i)
{
    SET_BITS(CORALAX_BOSS_COSTUME, i);
}

bool authUserHasCyborgPack(U32 *data)
{
    return GET_BITS(CYBORG_PACK);
}

void authUserSetCyborgPack(U32 *data, int i)
{
    SET_BITS(CYBORG_PACK, i);
}

bool authUserHasMacPack(U32 *data)
{
    return GET_BITS(MAC_PACK);
}

void authUserSetMacPack(U32 *data, int i)
{
    SET_BITS(MAC_PACK, i);
}

bool authUserHasIsRetailAccount(U32 *data)
{
    return true;
}

void authUserSetIsRetailAccount(U32 *data, int i)
{
    SET_BITS(IS_RETAIL_ACCOUNT, i);
}

bool authUserHasTargetPromo(U32 *data)
{
    return GET_BITS(TARGET_PROMO);
}

void authUserSetTargetPromo(U32 *data, int i)
{
    SET_BITS(TARGET_PROMO, i);
}

bool authUserHasPaxPromo(U32 *data)
{
    return GET_BITS(PAX_PROMO);
}

void authUserSetPaxPromo(U32 *data, int i)
{
    SET_BITS(PAX_PROMO, i);
}

bool authUserHasGenConPromo(U32 *data)
{
    return GET_BITS(GENCON_PROMO);
}

void authUserSetGenConPromo(U32 *data, int i)
{
    SET_BITS(GENCON_PROMO, i);
}

bool authUserHasComiconPromo(U32 *data)
{
    return GET_BITS(COMICON_PROMO);
}

void authUserSetComiconPromo(U32 *data, int i)
{
    SET_BITS(COMICON_PROMO, i);
}

bool authUserHasArachnosAccess(U32 *data)
{
    return GET_BITS(ARACHNOS_ACCESS);
}

void authUserSetArachnosAccess(U32 *data, int i)
{
    SET_BITS(ARACHNOS_ACCESS, i);
}

bool authUserHasWeddingPack(U32 *data)
{
    return GET_BITS(WEDDING_PACK);
}

void authUserSetWeddingPack(U32 *data, int i)
{
    SET_BITS(WEDDING_PACK, i);
}

bool authUserHasTrialAccount(U32 *data)
{
    return GET_BITS(TRIAL_ACCOUNT);
}

void authUserSetTrialAccount(U32 *data, int i)
{
    SET_BITS(TRIAL_ACCOUNT, i);
}

bool authUserHasPocketDVIP(U32 *data)
{
    return GET_BITS(POCKET_D_VIP);
}

void authUserSetPocketDVIP(U32 *data, int i)
{
    SET_BITS(POCKET_D_VIP, i);
}

bool authUserHasJumpPack(U32 *data)
{
    return GET_BITS(JUMP_PACK);
}

void authUserSetJumpPack(U32 *data, int i)
{
    SET_BITS(JUMP_PACK, i);
}

bool authUserHasJetPack(U32 *data)
{
    return GET_BITS(JET_PACK);
}

void authUserSetJetPack(U32 *data, int i)
{
    SET_BITS(JET_PACK, i);
}

bool authUserHasKoreanLevelMinimum(U32 *data)
{
    return GET_BITS(KOREAN_LEVEL_MINIMUM);
}

void authUserSetKoreanLevelMinimum(U32 *data, int i)
{
    SET_BITS(KOREAN_LEVEL_MINIMUM, i);
}

bool authUserHasValentine2006(U32 *data)
{
    return GET_BITS(VALENTINE_2006);
}

void authUserSetValentine2006(U32 *data, int i)
{
    SET_BITS(VALENTINE_2006, i);
}

bool authUserHasCovBeta(U32 *data)
{
    return GET_BITS(COV_BETA);
}

void authUserSetCovBeta(U32 *data, int i)
{
    SET_BITS(COV_BETA, i);
}

bool authUserHasCovPreorder1(U32 *data)
{
    return GET_BITS(COV_PREORDER_1);
}

void authUserSetCovPreorder1(U32 *data, int i)
{
    SET_BITS(COV_PREORDER_1, i);
}

bool authUserHasCovPreorder2(U32 *data)
{
    return GET_BITS(COV_PREORDER_2);
}

void authUserSetCovPreorder2(U32 *data, int i)
{
    SET_BITS(COV_PREORDER_2, i);
}

bool authUserHasCovPreorder3(U32 *data)
{
    return GET_BITS(COV_PREORDER_3);
}

void authUserSetCovPreorder3(U32 *data, int i)
{
    SET_BITS(COV_PREORDER_3, i);
}

bool authUserHasCovPreorder4(U32 *data)
{
    return GET_BITS(COV_PREORDER_4);
}

void authUserSetCovPreorder4(U32 *data, int i)
{
    SET_BITS(COV_PREORDER_4, i);
}

bool authUserHasCovSpecialEdition(U32 *data)
{
    return GET_BITS(COV_SPECIAL_EDITION);
}

void authUserSetCovSpecialEdition(U32 *data, int i)
{
    SET_BITS(COV_SPECIAL_EDITION, i);
}

bool authUserHasVillainAccess(U32 *data)
{
    return true;
}

void authUserSetVillainAccess(U32 *data, int i)
{
    SET_BITS(VILLAIN_ACCESS, i);
}

PreorderCode authUserPreorderCode(U32 *data)
{
    return GET_BITS(PREORDER);
}

void authUserSetPreorderCode(U32 *data, int i)
{
    SET_BITS(PREORDER, i);
}

bool authUserHasDVDSpecialEdition(U32 *data)
{
    return GET_BITS(DVDSPECIALEDITION);
}

void authUserSetDVDSpecialEdition(U32 *data, int i)
{
    SET_BITS(DVDSPECIALEDITION, i);
}

bool authUserHasKheldian(U32 *data)
{
    return GET_BITS(KHELDIAN_ACCESS);
}

void authUserSetKheldian(U32 *data, int i)
{
    SET_BITS(KHELDIAN_ACCESS, i);
}

bool authUserHasBeta(U32 *data)
{
    return GET_BITS(BETA);
}

void authUserSetBeta(U32 *data, int i)
{
    SET_BITS(BETA, i);
}

bool authUserHasTransferToEU(U32 *data)
{
    return GET_BITS(TRANSFER_TO_EU);
}

void authUserSetTransferToEU(U32 *data, int i)
{
    SET_BITS(TRANSFER_TO_EU, i);
}

bool authUserHasHeroAccess(U32 *data)
{
    return true;
}

void authUserSetHeroAccess(U32 *data, int i)
{
    SET_BITS(HERO_ACCESS, i);
}

bool authUserHasRecreationPack(U32 *data)
{
    return GET_BITS(RECREATION_PACK);
}

void authUserSetRecreationPack(U32 *data, int i)
{
    SET_BITS(RECREATION_PACK, i);
}

bool authUserHasHolidayPack(U32 *data)
{
    return GET_BITS(HOLIDAY_PACK);
}

void authUserSetHolidayPack(U32 *data, int i)
{
    SET_BITS(HOLIDAY_PACK, i);
}

bool authUserHasAuraPack(U32 *data)
{
    return GET_BITS(AURA_PACK);
}

void authUserSetAuraPack(U32 *data, int i)
{
    SET_BITS(AURA_PACK, i);
}

bool authUserHasVanguardPack(U32 *data)
{
    return GET_BITS(VANGUARD_PACK);
}

void authUserSetVanguardPack(U32 *data, int i)
{
    SET_BITS(VANGUARD_PACK, i);
}

bool authUserHasFacepalm(U32 *data)
{
    return GET_BITS(FACEPALM);
}

void authUserSetFacepalm(U32 *data, int i)
{
    SET_BITS(FACEPALM, i);
}

bool authUserHasAnimalPack(U32 *data)
{
    return GET_BITS(ANIMAL_PACK);
}

void authUserSetAnimalPack(U32 *data, int i)
{
    SET_BITS(ANIMAL_PACK, i);
}

bool authUserHasLimitedRogueAccess(U32 *data)
{
    return false;
}

void authUserSetLimitedRogueAccess(U32 *data, int i)
{
    SET_BITS(LIMITED_ROGUE_ACCESS, i);
}

bool authUserHasSteamPunk(U32 *data)
{
    return GET_BITS(STEAM_PUNK);
}

void authUserSetSteamPunk(U32 *data, int i)
{
    SET_BITS(STEAM_PUNK, i);
}

//AUTH BIT for 2011 cons
bool authUserHasPromo2011A(U32 *data)
{
    return GET_BITS(PROMO_2011_A);
}

void authUserSetPromo2011A(U32 *data, int i)
{
    SET_BITS(PROMO_2011_A, i);
}

bool authUserHasPromo2011B(U32 *data)
{
    return GET_BITS(PROMO_2011_B);
}

void authUserSetPromo2011B(U32 *data, int i)
{
    SET_BITS(PROMO_2011_B, i);
}

bool authUserHasPromo2011C(U32 *data)
{
    return GET_BITS(PROMO_2011_C);
}

void authUserSetPromo2011C(U32 *data, int i)
{
    SET_BITS(PROMO_2011_C, i);
}

bool authUserHasPromo2011D(U32 *data)
{
    return GET_BITS(PROMO_2011_D);
}

void authUserSetPromo2011D(U32 *data, int i)
{
    SET_BITS(PROMO_2011_D, i);
}

bool authUserHasPromo2011E(U32 *data)
{
    return GET_BITS(PROMO_2011_E);
}

void authUserSetPromo2011E(U32 *data, int i)
{
    SET_BITS(PROMO_2011_E, i);
}

//AUTH BIT for 2012 cons
bool authUserHasPromo2012A(U32 *data)
{
    return GET_BITS(PROMO_2012_A);
}

void authUserSetPromo2012A(U32 *data, int i)
{
    SET_BITS(PROMO_2012_A, i);
}

bool authUserHasPromo2012B(U32 *data)
{
    return GET_BITS(PROMO_2012_B);
}

void authUserSetPromo2012B(U32 *data, int i)
{
    SET_BITS(PROMO_2012_B, i);
}

bool authUserHasPromo2012C(U32 *data)
{
    return GET_BITS(PROMO_2012_C);
}

void authUserSetPromo2012C(U32 *data, int i)
{
    SET_BITS(PROMO_2012_C, i);
}

bool authUserHasPromo2012D(U32 *data)
{
    return GET_BITS(PROMO_2012_D);
}

void authUserSetPromo2012D(U32 *data, int i)
{
    SET_BITS(PROMO_2012_D, i);
}

bool authUserHasPromo2012E(U32 *data)
{
    return GET_BITS(PROMO_2012_E);
}

void authUserSetPromo2012E(U32 *data, int i)
{
    SET_BITS(PROMO_2012_E, i);
}

//AUTH BIT for your most secret, most evil promotional plans.  
//I'm going to get these requested from Austin now in case something comes up and we need them.
bool authUserHasAceInHoleA(U32 *data)
{
    return GET_BITS(ACEINHOLE_A);
}

void authUserSetAceInHoleA(U32 *data, int i)
{
    SET_BITS(ACEINHOLE_A, i);
}

bool authUserHasAceInHoleB(U32 *data)
{
    return GET_BITS(ACEINHOLE_B);
}

void authUserSetAceInHoleB(U32 *data, int i)
{
    SET_BITS(ACEINHOLE_B, i);
}

bool authUserHasAceInHoleC(U32 *data)
{
    return GET_BITS(ACEINHOLE_C);
}

void authUserSetAceInHoleC(U32 *data, int i)
{
    SET_BITS(ACEINHOLE_C, i);
}

bool authUserHasAceInHoleD(U32 *data)
{
    return GET_BITS(ACEINHOLE_D);
}

void authUserSetAceInHoleD(U32 *data, int i)
{
    SET_BITS(ACEINHOLE_D, i);
}

bool authUserHasAceInHoleE(U32 *data)
{
    return GET_BITS(ACEINHOLE_E);
}

void authUserSetAceInHoleE(U32 *data, int i)
{
    SET_BITS(ACEINHOLE_E, i);
}


bool authUserCanGenderChange(U32*data)
{
    //    for now .. its just the super science pack
    if ( authUserHasSupersciencePack(data) )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

#ifndef DBSERVER

static char* strPrintAuthUserData(U32 *data, int base)
{
    static char *estr = 0;

    int i;
    U8 *puch;

    data = &data[base];
    puch = (U8*)data;

    estrClear(&estr);

    estrConcatStaticCharArray(&estr, "   as bytes: 0x");
    for(i=0; i<16; i++)
    {
        estrConcatf(&estr, "%02x", (int)puch[i]);
    }

    estrConcatStaticCharArray(&estr, "\n   as words:");
    for(i=0; i<4; i++)
    {
        estrConcatf(&estr, " (%02d) 0x%08x", base + i, data[i]);
    }
    estrConcatChar(&estr, '\n');

    return estr;
}

#ifdef SERVER
#include "gameComm/sendToClient.h"
#include "language/langServerUtil.h"
#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "player/clientEntityLink.h"
#define CON_PRINT_F(...) conPrintf(client,__VA_ARGS__)
#define LOCALIZED_F(...) localizedPrintf(e,__VA_ARGS__)
#else
#include "UI/uiConsole.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "clientcomm/dbclient.h"
#define CON_PRINT_F(...) conPrintf(__VA_ARGS__)
#define LOCALIZED_F(...) textStd(__VA_ARGS__)
#endif

#ifdef SERVER
bool conShowAuthUserData(ClientLink *client)
{
    Entity *e = clientGetControlledEntity(client);
    U32 *data = (e && e->pl) ? e->pl->auth_user_data : NULL;
#else
bool conShowAuthUserData()
{
    U32 *data = (U32*)db_info.auth_user_data;
#endif

#define PRINTBIT(xx) CON_PRINT_F("   %s " #xx "\n", authUserHas##xx(data) ? "*" : " ");
    PreorderCode code;

    if(!data)
        return false;

    CON_PRINT_F(LOCALIZED_F("AUDRaw"));
    CON_PRINT_F(strPrintAuthUserData(data, 0));
    CON_PRINT_F(strPrintAuthUserData(data, 4));

    CON_PRINT_F(LOCALIZED_F("AUDDecoded"));
    CON_PRINT_F("   %i MonthsPlayed\n",authUserMonthsPlayed(data));
    PRINTBIT(Con2009Bit1)
    PRINTBIT(Con2009Bit2)
    PRINTBIT(Con2009Bit3)
    PRINTBIT(Con2009Bit4)
    PRINTBIT(Con2009Bit5)
    PRINTBIT(LoyaltyReward)
    PRINTBIT(MagicPack)
    PRINTBIT(SupersciencePack)
    PRINTBIT(MartialArtsPack)
    PRINTBIT(RogueCompleteBox)
    PRINTBIT(GRPreOrderResistanceTactics)
    PRINTBIT(GRPreOrderMightOfTheEmpire)
    PRINTBIT(GRPreOrderClockworkEfficiency)
    PRINTBIT(GRPreOrderWillOfTheSeers)
    PRINTBIT(RogueAccess)
//    PRINTBIT(CreatePrimals)
    PRINTBIT(MutantPack)
    PRINTBIT(GhoulCostume)
    PRINTBIT(PraetoriaPDCostume)
    PRINTBIT(RiktiCostume)
    PRINTBIT(VahzilokMeatDocCostume)
    PRINTBIT(CoralaxBossCostume)
    PRINTBIT(GRPreOrderSyndicateTechniques);
    PRINTBIT(CyborgPack);
    PRINTBIT(MacPack);
    PRINTBIT(IsRetailAccount);
    PRINTBIT(TargetPromo);
    PRINTBIT(PaxPromo);
    PRINTBIT(GenConPromo);
    PRINTBIT(ComiconPromo);
    PRINTBIT(ArachnosAccess);
    PRINTBIT(WeddingPack);
    PRINTBIT(TrialAccount);
    PRINTBIT(PocketDVIP);
    PRINTBIT(JumpPack);
    PRINTBIT(JetPack);
    PRINTBIT(KoreanLevelMinimum);
    PRINTBIT(Valentine2006);
    PRINTBIT(CovBeta);
    PRINTBIT(CovPreorder1);
    PRINTBIT(CovPreorder2);
    PRINTBIT(CovPreorder3);
    PRINTBIT(CovPreorder4);
    PRINTBIT(CovSpecialEdition);
    PRINTBIT(VillainAccess);
    code = authUserPreorderCode(data);
    CON_PRINT_F("   %s Preorder:EB\n", code==PREORDER_ELECTRONICSBOUTIQUE ? "*" : " ");
    CON_PRINT_F("   %s Preorder:GameStop\n", code==PREORDER_GAMESTOP ? "*" : " ");
    CON_PRINT_F("   %s Preorder:BestBuy\n", code==PREORDER_BESTBUY ? "*" : " ");
    CON_PRINT_F("   %s Preorder:Generic\n", code==PREORDER_GENERIC ? "*" : " ");
    PRINTBIT(DVDSpecialEdition);
    PRINTBIT(Kheldian);
    PRINTBIT(Beta);
    PRINTBIT(TransferToEU);
    PRINTBIT(HeroAccess);

    PRINTBIT(RecreationPack);
    PRINTBIT(HolidayPack);
    PRINTBIT(AuraPack);
    PRINTBIT(VanguardPack);
    PRINTBIT(Facepalm);
    PRINTBIT(AnimalPack);
    PRINTBIT(LimitedRogueAccess);
    PRINTBIT(Promo2011A);
    PRINTBIT(Promo2011B);
    PRINTBIT(Promo2011C);
    PRINTBIT(Promo2011D);
    PRINTBIT(Promo2011E);
    PRINTBIT(Promo2012A);
    PRINTBIT(Promo2012B);
    PRINTBIT(Promo2012C);
    PRINTBIT(Promo2012D);
    PRINTBIT(Promo2012E);
    PRINTBIT(SteamPunk);
    PRINTBIT(AceInHoleA);
    PRINTBIT(AceInHoleB);
    PRINTBIT(AceInHoleC);
    PRINTBIT(AceInHoleD);
    PRINTBIT(AceInHoleE);

    CON_PRINT_F(LOCALIZED_F("AUDHelp"));

    STATIC_INFUNC_ASSERT( BITS_WORD3_RESERVED_1 == 1 ); // update this function if new bits set

    return true;
}

#ifdef SERVER 
bool conSetAuthUserData(ClientLink *client, char *pch, int i)
{
    Entity *e = clientGetControlledEntity(client);
    U32 *data = (e && e->pl) ? e->pl->auth_user_data : NULL;
    int isAccessLevel = e ? e->access_level : 0;
#else
bool conSetAuthUserData(char *pch, int i)
{
    U32 *data = (U32*)db_info.auth_user_data;
    int isAccessLevel = isDevelopmentOrQAMode();
#endif
    U32 auOld[4];

    if(!data)
        return false;

    memcpy(auOld, data, sizeof(auOld));

    if(!authUserSetFieldByName(data,pch,i))
    {
        CON_PRINT_F(LOCALIZED_F("AUDUnknown"));
        return false;
    }

    if (isAccessLevel)
    {
        CON_PRINT_F(LOCALIZED_F("AUDStatus", pch, i) );
        CON_PRINT_F(LOCALIZED_F("AUDWas"));
        CON_PRINT_F(strPrintAuthUserData(auOld, 0));
        CON_PRINT_F(strPrintAuthUserData(auOld, 4));
        CON_PRINT_F(LOCALIZED_F("AUDNow"));
        CON_PRINT_F(strPrintAuthUserData(data, 0));
        CON_PRINT_F(strPrintAuthUserData(data, 4));
    }
#ifdef SERVER
    if(client->link && data)
    {
        int i;

        START_PACKET(pak, client->entity, SERVER_AUTH_BITS);
        for (i = 0; i < 4; i++)
        {
            pktSendBitsAuto(pak, data[i]);
        }
        END_PACKET
    }
#endif

    return true;
}

#endif // #ifndef DBSERVER

/* End of File */
