#############################################################
## encounters_CustomWeapons.inc
#############################################################

## ARMOR POSE 01 ###########################################################

Move Enc_CustomRighthandWeapon_ArmorPose01
	Requires	ENCOUNTER CUSTOMWEAPON POSE ASLEEP ONE
	Priority        5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale ReqInputs NotSelectable
	SticksOnChild	RIGHTHAND
	Sets		CANTMOVE

	Type		male
	  Anim          male\ArmorPose01 1 2
	TEnd
MEnd

## CUSTOM 'SWORD' - OBSERVE ###########################################################

Move Enc_CustomRighthandWeapon_Observe_Cycle1
	Requires	ENCOUNTER CUSTOMWEAPON POSE OBSERVE LOW RIGHTHAND
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove	Enc_CustomRighthandWeapon_Observe_Cycle1
	CycleMove	Enc_CustomRighthandWeapon_Observe_Cycle2
	SticksOnChild	RIGHTHAND
	Scale           0.50

	Type		male
	  Anim          male\RighthandWeapon&Shield_Observe_Right 1
	TEnd
MEnd

Move Enc_CustomRighthandWeapon_Observe_Cycle2
	Requires	ENCOUNTER CUSTOMWEAPON POSE OBSERVE LOW RIGHTHAND
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove	Enc_CustomRighthandWeapon_Observe_Cycle1
	CycleMove	Enc_CustomRighthandWeapon_Observe_Cycle2
	SticksOnChild	RIGHTHAND
	Scale           0.7

	Type		male
	  Anim          male\RighthandWeapon&Shield_Observe_Left 1
	TEnd
MEnd

## CUSTOM 'SWORD' - ALERT ###########################################################

Move Enc_CustomRighthandWeapon_Alert
	Requires	ENCOUNTER CUSTOMWEAPON POSE OBSERVE HIGH
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	SticksOnChild	RIGHTHAND
	Scale		0.5

	Type		male
	  Anim          male\wep_cycle 1
	TEnd

	Type		huge
	  Anim          huge\wep_cycle 1 31
	TEnd

	Type		fem
	  Anim          fem\wep_cycle 1 31
	TEnd
MEnd

## CUSTOM 'SWORD' - GUARD ###########################################################

Move Enc_CustomRighthandWeapon_Guard
	Requires	ENCOUNTER CUSTOMWEAPON POSE LOOKOUT LOW
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	SticksOnChild	RIGHTHAND
	Scale		0.3

	Type		male
	  Anim          male\guard_sword_l 1
	TEnd
MEnd

## CUSTOM 'SWORD' - MENACE ###########################################################

Move Enc_CustomRighthandWeapon_MenaceA
	Requires	ENCOUNTER COMBAT WEAPON POSE MENACE LOW CUSTOMWEAPON
	Priority	7
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	SticksOnChild	RIGHTHAND
	Flags		Cycle PICKRANDOMLY

	Type		male
	  Anim          male\bat_twirl 1
	TEnd

	Type		Tsoo
	  Anim          male\bat_twirl 1
	TEnd
MEnd

Move Enc_CustomRighthandWeapon_MenaceB
	Requires	ENCOUNTER CUSTOMWEAPON POSE MENACE LOW
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	SticksOnChild	RIGHTHAND
	Flags		Cycle PICKRANDOMLY

	Type		male
	  Anim          male\bat_cycle 1
	TEnd
MEnd

## CUSTOM 'SWORD' & 'SHIELD' - OBSERVE ###########################################################

Move Enc_CustomRighthandWeaponShield_Observe_Cycle1
	Requires	ENCOUNTER CUSTOMWEAPON POSE OBSERVE LOW RIGHTHAND SHIELD
	Priority	7
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove	Enc_CustomRighthandWeaponShield_Observe_Cycle1
	CycleMove	Enc_CustomRighthandWeaponShield_Observe_Cycle2
	SticksOnChild	RIGHTHAND SHIELD
	Scale           0.50

	Type		male
	  Anim          male\RighthandWeapon&Shield_Observe_Right 1
	TEnd
MEnd

Move Enc_CustomRighthandWeaponShield_Observe_Cycle2
	Requires	ENCOUNTER CUSTOMWEAPON POSE OBSERVE LOW RIGHTHAND SHIELD
	Priority	7
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove	Enc_CustomRighthandWeaponShield_Observe_Cycle1
	CycleMove	Enc_CustomRighthandWeaponShield_Observe_Cycle2
	SticksOnChild	RIGHTHAND SHIELD
	Scale           0.7

	Type		male
	  Anim          male\RighthandWeapon&Shield_Observe_Left 1
	TEnd
MEnd


## CUSTOM 'SHIELD' - BLOCK ###########################################################

Move Enc_CustomShield_Block
	Requires	ENCOUNTER CUSTOMWEAPON POSE BLOCK SHIELD
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	SticksOnChild	SHIELD
	Flags		Cycle

	Type		male
	  Anim          male\shield_block 1
	TEnd

	Type		5cn
	  Anim          male\shield_block 1
	TEnd
MEnd

## CUSTOM 'SWORD' & 'SHIELD' - OBSERVE ###########################################################

Move Enc_CustomShield_Observe_Cycle1
	Requires	ENCOUNTER CUSTOMWEAPON POSE OBSERVE LOW SHIELD
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove	Enc_CustomShield_Observe_Cycle1
	CycleMove	Enc_CustomShield_Observe_Cycle2
	SticksOnChild	SHIELD
	Scale           0.50

	Type		male
	  Anim          male\RighthandWeapon&Shield_Observe_Right 1
	TEnd
MEnd

Move Enc_CustomShield_Observe_Cycle2
	Requires	ENCOUNTER CUSTOMWEAPON POSE OBSERVE LOW SHIELD
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove	Enc_CustomShield_Observe_Cycle1
	CycleMove	Enc_CustomShield_Observe_Cycle2
	SticksOnChild	SHIELD
	Scale           0.7

	Type		male
	  Anim          male\RighthandWeapon&Shield_Observe_Left 1
	TEnd
MEnd

## CUSTOM 'RIFLE' - OBSERVE ###########################################################

Move Enc_CustomRifle_LookoutA
	Requires	ENCOUNTER CUSTOMWEAPON RIFLE POSE OBSERVE LOW
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove       Enc_CustomRifle_LookoutA
	CycleMove       Enc_CustomRifle_LookoutB
	SticksOnChild	TWOHAND
	Scale		0.5

	Type		male
	  Anim          male\shotgun_look 1
	TEnd
MEnd

Move Enc_CustomRifle_LookoutB
	Requires	ENCOUNTER CUSTOMWEAPON RIFLE POSE OBSERVE LOW
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove       Enc_CustomRifle_LookoutA
	CycleMove       Enc_CustomRifle_LookoutB
	SticksOnChild	TWOHAND
	Scale		0.7

	Type		male
	  Anim          male\shotgun_look 1
	TEnd
MEnd

## CUSTOM 'RIFLE' - ALERT ###########################################################

Move Enc_CustomRifle_AlertA
	Requires	ENCOUNTER CUSTOMWEAPON RIFLE POSE OBSERVE HIGH
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	SticksOnChild	TWOHAND
	Scale		0.5

	Type		male
	  Anim          male\rocket_cycle 1 31
	TEnd
MEnd

Move Enc_CustomRifle_AlertB
	Requires	ENCOUNTER CUSTOMWEAPON RIFLE POSE OBSERVE HIGH
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	SticksOnChild	TWOHAND
	Scale		0.4

	Type		male
	  Anim          male\shotgun_cycle 1 31
	TEnd
MEnd

## CUSTOM 'RIFLE' - POINT ###########################################################

Move Enc_CustomRifle_Point
	Requires	ENCOUNTER CUSTOMWEAPON RIFLE POSE POINT
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	SticksOnChild	TWOHAND
	Scale		0.5

	Type		male
	  Anim          male\shotgun_point 1 100
	TEnd

	Type		fem
	  Anim          fem\shotgun_point 1 100
	TEnd
MEnd

## CUSTOM 'RIFLE' - SHOOTING ###########################################################

Move Enc_CustomRifle_ShootingA
	Requires	ENCOUNTER CUSTOMWEAPON RIFLE POSE ATTACK MID
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	SticksOnChild	TWOHAND
	CycleMove       Enc_CustomRifle_ShootingB

	Type		male
	  Anim          male\sniper_new 35
	  PlayFx        WEAPONFX\ENCOUNTER_CustomRifle_Slug.fx 56
	TEnd
MEnd

Move Enc_CustomRifle_ShootingB
	Requires	ENCOUNTER CUSTOMWEAPON RIFLE POSE ATTACK MID
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove       Enc_CustomRifle_ShootingA
	CycleMove       Enc_CustomRifle_ShootingC
	SticksOnChild	TWOHAND
	Scale           0.50

	Type		male
	  Anim          male\rocket_cycle 1 31
	TEnd
MEnd

Move Enc_CustomRifle_ShootingC
	Requires	ENCOUNTER CUSTOMWEAPON RIFLE POSE ATTACK MID
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	SticksOnChild	TWOHAND
	CycleMove       Enc_CustomRifle_ShootingA

	Type		male
	  Anim          male\sniper_cock 3
	  PlayFx        WEAPONS\COCK2.FX 27
	TEnd
MEnd

## CUSTOM 'RIFLE' - SHOOTING HIGH ###########################################################

Move Enc_CustomRifle_ShootingHigh_1
	Requires	ENCOUNTER CUSTOMWEAPON RIFLE POSE ATTACK HIGH
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove       Enc_CustomRifle_ShootingHigh_2
	CycleMove       Enc_CustomRifle_ShootingHigh_Cock
	SticksOnChild	TWOHAND

	Type		male
	  Anim          male\sniper 1
	  PlayFx        WEAPONFX\ENCOUNTER_CustomRifle_SingleShot.fx 34
	TEnd
MEnd

Move Enc_CustomRifle_ShootingHigh_2
	Requires	ENCOUNTER CUSTOMWEAPON RIFLE POSE ATTACK HIGH
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove       Enc_CustomRifle_ShootingHigh_3
	CycleMove       Enc_CustomRifle_ShootingHigh_1
	CycleMove       Enc_CustomRifle_ShootingHigh_Cock
	SticksOnChild	TWOHAND
	Scale           0.50

	Type		male
	  Anim          male\shotgun_cycle 1
	TEnd
MEnd

Move Enc_CustomRifle_ShootingHigh_3
	Requires	ENCOUNTER CUSTOMWEAPON RIFLE POSE ATTACK HIGH
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove       Enc_CustomRifle_ShootingHigh_4
	CycleMove       Enc_CustomRifle_ShootingHigh_3B
	SticksOnChild	TWOHAND

	Type		male
	  Anim          male\sniper 1 60
	  PlayFx        WEAPONFX\ENCOUNTER_CustomRifle_SingleShot.fx 34
	TEnd
MEnd

Move Enc_CustomRifle_ShootingHigh_3B
	Requires	ENCOUNTER CUSTOMWEAPON RIFLE POSE ATTACK HIGH
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove       Enc_CustomRifle_ShootingHigh_3B
	CycleMove       Enc_CustomRifle_ShootingHigh_4
	SticksOnChild	TWOHAND

	Type		male
	  Anim          male\sniper 34 60
	  PlayFx        WEAPONFX\ENCOUNTER_CustomRifle_SingleShot.fx
	TEnd
MEnd

Move Enc_CustomRifle_ShootingHigh_4
	Requires	ENCOUNTER CUSTOMWEAPON RIFLE POSE ATTACK HIGH
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove       Enc_CustomRifle_ShootingHigh_2
	CycleMove       Enc_CustomRifle_ShootingHigh_Cock
	SticksOnChild	TWOHAND

	Type		male
	  Anim          male\sniper 61
	TEnd
MEnd

Move Enc_CustomRifle_ShootingHigh_Cock
	Requires	ENCOUNTER CUSTOMWEAPON RIFLE POSE ATTACK HIGH
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove       Enc_CustomRifle_ShootingHigh_3
	CycleMove       Enc_CustomRifle_ShootingHigh_1
	CycleMove       Enc_CustomRifle_ShootingHigh_2
	SticksOnChild	TWOHAND

	Type		male
	  Anim          male\sniper_cock 3
	  PlayFx        WEAPONS\COCK2.FX 27
	TEnd
MEnd

## CUSTOM 'KATANA' - OBSERVE ###########################################################

Move Enc_Custom_Katana_ObserveA
	Requires	ENCOUNTER CUSTOMWEAPON KATANA POSE OBSERVE LOW
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Katana_Observe_B
	CycleMove       Katana_Observe_A
	SticksOnChild	LEFTHAND
	Scale           0.50

	Type		male
	  Anim          male\bat_look_R_down 1
	TEnd
MEnd

Move Enc_Custom_Katana_ObserveB
	Requires	ENCOUNTER CUSTOMWEAPON KATANA POSE OBSERVE LOW
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Katana_Observe_A
	CycleMove       Katana_Observe_B
	SticksOnChild	LEFTHAND
	Scale           0.7

	Type		male
	  Anim          male\bat_look_L_down 1
	TEnd
MEnd

## CUSTOM 'KATANA' - LOOKOUT ###########################################################

Move Enc_Custom_Katana_LookoutA
	Requires	ENCOUNTER CUSTOMWEAPON KATANA POSE LOOKOUT HIGH
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Enc_Custom_Katana_LookoutA
	CycleMove       Enc_Custom_Katana_LookoutB
	SticksOnChild	LEFTHAND
	Scale		0.5

	Type		male
	  Anim          male\suspicious 1
	TEnd
MEnd

Move Enc_Custom_Katana_LookoutB
	Requires	ENCOUNTER CUSTOMWEAPON KATANA POSE LOOKOUT HIGH
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       Enc_Custom_Katana_LookoutA
	CycleMove       Enc_Custom_Katana_LookoutB
	SticksOnChild	LEFTHAND
	Scale		0.8

	Type		male
	  Anim          male\lookquick 1
	TEnd
MEnd

## CUSTOM 'KATANA' - GUARD ###########################################################

Move Enc_Custom_Katana_Guard
	Requires	ENCOUNTER CUSTOMWEAPON KATANA POSE LOOKOUT LOW
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	SticksOnChild	LEFTHAND
	Scale		0.3

	Type		male
	  Anim          male\guard_sword_l 1
	TEnd
MEnd

## CUSTOM 'KATANA' - ALERT ###########################################################

Move Enc_Custom_Katana_Alert
	Requires	ENCOUNTER CUSTOMWEAPON KATANA POSE MID
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle FixArmReg
	SticksOnChild	LEFTHAND
	Scale           0.50

	Type		male
	  Anim          male\katana_cycle 1 31
	TEnd

	Type		fem
	  Anim          fem\katana_cycle 1 31
	TEnd

	Type		huge
	  Anim          huge\katana_cycle 1 31
	TEnd
MEnd

## CUSTOM 'KATANA' - POINT ###########################################################

Move Enc_Custom_Katana_Point
	Requires	ENCOUNTER CUSTOMWEAPON KATANA POSE POINT
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	SticksOnChild	LEFTHAND

	Type		male
	  Anim          male\katana_point 1
	TEnd
MEnd

## CROSS SWORDS ###########################################################

Move Enc_Custom_Katana_CrossSwords
	Requires	ENCOUNTER CUSTOMWEAPON POSE CROSS
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	SticksOnChild	LEFTHAND

	Type		male
	  Anim          male\block_sword_l 1
	TEnd
MEnd

## CUSTOM 'BOW' - SHOOT ###########################################################

Move Enc_Custom_Bow_Shoot1
	Requires	ENCOUNTER CUSTOMWEAPON ARCHERY POSE ATTACK
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove	Enc_Custom_Bow_Shoot1
	CycleMove	Enc_Custom_Bow_Shoot2
	CycleMove	Enc_Custom_Bow_Shoot3
	SticksOnChild   COMBAT WEAPON DUAL REV BOW MID ENCOUNTER LEFTHAND

	Type		male
	  Anim          male\bow_midshoot 1 120
	  PlayFX	WEAPONS\Bow\Encounter_CustomBow_Mid.fx 0
	TEnd
MEnd

Move Enc_Custom_Bow_Shoot2
	Requires	ENCOUNTER CUSTOMWEAPON ARCHERY POSE ATTACK
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove	Enc_Custom_Bow_Shoot1
	CycleMove	Enc_Custom_Bow_Shoot2
	CycleMove	Enc_Custom_Bow_Shoot3
	SticksOnChild   COMBAT WEAPON DUAL REV BOW STRONG ENCOUNTER LEFTHAND

	Type		male
	  Anim          male\bow_snipershoot 1 130
	  PlayFX	WEAPONS\Bow\Encounter_CustomBow_sniper.fx 0
	TEnd
MEnd

Move Enc_Custom_Bow_Shoot3
	Requires	ENCOUNTER CUSTOMWEAPON ARCHERY POSE ATTACK
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove	Enc_Custom_Bow_Shoot1
	CycleMove	Enc_Custom_Bow_Shoot2
	CycleMove	Enc_Custom_Bow_Shoot3
	SticksOnChild   COMBAT WEAPON DUAL REV BOW CONE ENCOUNTER LEFTHAND

	Type		male
	  Anim          male\bow_coneshoot 1 120
	  PlayFX	WEAPONS\Bow\Encounter_CustomBow_cone.fx 0
	TEnd
MEnd

## CUSTOM 'SWORD' - OBSERVE ###########################################################

Move Enc_CustomDualWeapon_Observe_Cycle1
	Requires	ENCOUNTER CUSTOMWEAPON POSE OBSERVE LOW DUALHAND
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove	Enc_CustomDualWeapon_Observe_Cycle1
	CycleMove	Enc_CustomDualWeapon_Observe_Cycle2
	SticksOnChild	DUALHAND
	Scale           0.50

	Type		male
	  Anim          male\RighthandWeapon&Shield_Observe_Right 1
	TEnd
MEnd

Move Enc_CustomDualWeapon_Observe_Cycle2
	Requires	ENCOUNTER CUSTOMWEAPON POSE OBSERVE LOW DUALHAND
	Priority	6
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove	Enc_CustomDualWeapon_Observe_Cycle1
	CycleMove	Enc_CustomDualWeapon_Observe_Cycle2
	SticksOnChild	DUALHAND
	Scale           0.7

	Type		male
	  Anim          male\RighthandWeapon&Shield_Observe_Left 1
	TEnd
MEnd

## KNEEL Rifle Low Alert ###########################################################

Move Encounter_Rifle_Alert_Kneeling
	Requires	ENCOUNTER CUSTOMWEAPON KNEEL LOW ALTERNATE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle nosizescale
	CycleMove	Encounter_Rifle_Alert_Kneeling
	CycleMove	Encounter_Rifle_Alert_Kneeling
	CycleMove	Encounter_Rifle_Alert_Kneeling
	CycleMove	Encounter_Rifle_Alert_Kneeling_Break01
	NextMove        Kneel_getup
	SticksOnChild	TWOHAND
	Scale		0.25

	Type		male
	  Anim          male\emote_sniper_alert_kneeling 1
	  #PlayFx        WEAPONS\SNIPER_RIFLE.FX  0 Constant
	TEnd

MEnd


Move Encounter_Rifle_Alert_Kneeling_Break01
	Requires	ENCOUNTER CUSTOMWEAPON KNEEL LOW ALTERNATE
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle nosizescale
	CycleMove	Encounter_Rifle_Alert_Kneeling
	NextMove        Kneel_getup
	SticksOnChild	TWOHAND
	Scale		0.5

	Type		male
	  Anim          male\emote_sniper_alert_kneeling_break1 1
	  #PlayFx        WEAPONS\SNIPER_RIFLE.FX  0 Constant
	TEnd

MEnd

## DUST Patrol Alert ###########################################################

Move Encounter_CustomRifle_Alert_Patrol_Pre
	Requires	LOITER CUSTOMWEAPON ALERT LOW
	Priority	4
	Include sequencers\Interrupts\Encounters_Motion.inc
	Include sequencers\Members\Encounters_Motion.inc
	Flags		ReqInputs Cycle NoSizeScale
	CycleMove        Encounter_CustomRifle_Alert_Patrol_Pre
	CycleMove        Encounter_CustomRifle_Alert_Patrol_Post
	CycleMove        Encounter_CustomRifle_Alert_Patrol_Cycle
	CycleMove        Encounter_CustomRifle_Alert_Patrol_Post
	SticksonChild	TWOHAND

	Type		male
	  Anim          male\dust_alert_patrol 1
	TEnd

	Type		fem
	  Anim          male\dust_alert_patrol 1
	TEnd

	Type		huge
	  Anim          male\dust_alert_patrol 1
	TEnd

MEnd

Move Encounter_CustomRifle_Alert_Patrol_Cycle
	Requires	 LOITER CUSTOMWEAPON ALERT LOW
	Include sequencers\Members\Encounters_Motion.inc
	Flags		Cycle NoSizeScale
	CycleMove        Encounter_CustomRifle_Alert_Patrol_Pre
	CycleMove        Encounter_CustomRifle_Alert_Patrol_Post
	CycleMove        Encounter_CustomRifle_Alert_Patrol_Post
	CycleMove        Encounter_CustomRifle_Alert_Patrol_Cycle
	SticksonChild	 TWOHAND

	Type		male
	  Anim          male\dust_alert_patrol 1
	TEnd

	Type		fem
	  Anim          male\dust_alert_patrol 1
	TEnd

	Type		huge
	  Anim          male\dust_alert_patrol 1
	TEnd

MEnd

Move Encounter_CustomRifle_Alert_Patrol_Post
	Requires	LOITER CUSTOMWEAPON ALERT LOW
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove        Encounter_CustomRifle_Alert_Patrol_Post
	CycleMove        Encounter_CustomRifle_Alert_Patrol_Post
	CycleMove        Encounter_CustomRifle_Alert_Patrol_Cycle
	CycleMove        Encounter_CustomRifle_Alert_Patrol_Pre
	SticksonChild	TWOHAND

	Type		male
	  Anim          male\dust_alert_patrol 1
	TEnd

	Type		fem
	  Anim          male\dust_alert_patrol 1
	TEnd

	Type		huge
	  Anim          male\dust_alert_patrol 1
	TEnd


MEnd

