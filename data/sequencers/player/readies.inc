#############################################################
## readies.inc - This should ALWAYS be the first include in any sequencer
#############################################################

## READY - This move HAS to be first in this sequencer ###########################################################

Move Ready
	Include sequencers\Members\Readies.inc
	Flags		Cycle
	CycleMove	Ready2
	Priority        0

	Type		male
	  Anim          male\Skel_ready2 1
	TEnd

	Type		fem
	  Anim          fem\Skel_ready2 1
	TEnd

	Type            huge
	  Anim          huge\Skel_ready 1 60
	TEnd

	Type		Vahzilok
	  Anim          Vahzilok\skel_vah_combat_cycle 1
	TEnd

	Type		Wisp
	  Anim          wisp\float 1
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_float 1
	TEnd

	Type		Crystal
	  Anim          male\MonComb_cycle 1
	TEnd

	Type		goblin
	  Anim          male\MonComb_cycle 1
	TEnd

	Type		Bobcat
	  Anim          fem\bobcat_contact_on_couch 1
	TEnd
MEnd

#############################################################

Move Ready2
	Include sequencers\Members\Readies.inc
	Flags		Cycle
	CycleMove	Ready_Look
	CycleMove	Ready_2Hips

	Type		male
	  Anim          male\Skel_ready2 1
	TEnd

	Type		fem
	  Anim          fem\Skel_ready2 1 60
	TEnd

	Type            huge
	  Anim          huge\Skel_ready 1 60
	TEnd

	Type		Crystal
	  Anim          male\MonComb_cycle 1
	TEnd

	Type		goblin
	  Anim          male\MonComb_cycle 1
	TEnd

	Type		Bobcat
	  Anim          fem\bobcat_contact_on_couch 1
	TEnd
MEnd

#############################################################

Move Ready_Look
	Include sequencers\Members\Readies.inc
	Flags		Cycle
	CycleMove	Ready2
	CycleMove	Ready_2Hips
	Scale		0.5

	Type		male
	  Anim          male\idle_look 1
	TEnd

	Type		fem
	  Anim          fem\idle_look 1
	TEnd

	Type            huge
	  Anim          huge\idle_look 1
	TEnd

	Type		Crystal
	  Anim          male\MonComb_cycle 1
	TEnd

	Type		goblin
	  Anim          male\MonComb_cycle 1
	TEnd

	Type		Bobcat
	  Anim          fem\bobcat_contact_on_couch 1
	TEnd
MEnd

#############################################################
## IDLE - HANDS ON HIPS
#############################################################

## READY TO HIPS ###########################################################

Move Ready_2Hips
	Include sequencers\Members\Readies.inc
	Flags		Cycle
	CycleMove	Ready_2Hips_look
	CycleMove	Ready_2Hips_look
	CycleMove	Ready_Xarms
	CycleMove	Ready_2Hips_post
	Priority        0
	Scale		0.5
	Interpolate	15

	Type		male
	  Anim          male\stand_2Hhips 1 64
	TEnd

	Type		huge
	  Anim          huge\stand_2Hhips 1 64
	TEnd

	Type		fem
	  Anim          fem\stand_2Hhips 1 64
	TEnd

	Type		Crystal
	  Anim          male\MonComb_cycle 1
	TEnd

	Type		goblin
	  Anim          male\MonComb_cycle 1
	TEnd

	Type		Bobcat
	  Anim          fem\bobcat_contact_on_couch 1
	TEnd
MEnd

#############################################################

Move Ready_2Hips_look
	Include sequencers\Members\Readies.inc
	Flags		Cycle
	CycleMove	Ready_2Hips_look
	CycleMove	Ready_Xarms
	CycleMove	Ready_Xarms
	CycleMove	Ready_2Hips_post
	Priority        0
	Scale		0.5

	Type		male
	  Anim          male\idle_2hips_look 1
	TEnd

	Type		huge
	  Anim          huge\idle_2hips_look 1
	TEnd

	Type		fem
	  Anim          fem\idle_2hips_look 1
	TEnd

	Type		Crystal
	  Anim          male\MonComb_cycle 1
	TEnd

	Type		goblin
	  Anim          male\MonComb_cycle 1
	  Scale		1
	TEnd

	Type		Bobcat
	  Anim          fem\bobcat_contact_on_couch 1
	TEnd
MEnd

## HIPS TO READY ###########################################################

Move Ready_2Hips_post
	Include sequencers\Members\Readies.inc
	Flags		Cycle
	CycleMove	Ready_Look
	CycleMove	Ready2
	Priority        0
	Scale		0.5
	Interpolate	15

	Type		male
	  Anim          male\stand_2Hhips 109
	TEnd

	Type		huge
	  Anim          huge\stand_2Hhips 109
	TEnd

	Type		fem
	  Anim          fem\stand_2Hhips 109
	TEnd

	Type		Crystal
	  Anim          male\MonComb_cycle 1
	TEnd

	Type		goblin
	  Anim          male\MonComb_cycle 1
	TEnd

	Type		Bobcat
	  Anim          fem\bobcat_contact_on_couch 1
	TEnd
MEnd

#############################################################
## IDLE - CROSS ARMS
#############################################################

## HIPS TO XARMS ###########################################################

Move Ready_Xarms
	Include sequencers\Members\Readies.inc
	Flags		Cycle
	CycleMove	Ready_Xarms_look
	CycleMove	Ready_Xarms_look
	CycleMove	Ready_Xarms_post
	Priority        0
	Scale		0.6
	Interpolate	15

	Type		male
	  Anim          male\stand_Xarms 1 68
	TEnd

	Type		huge
	  Anim          huge\stand_Xarms 1 68
	TEnd

	Type		fem
	  Anim          fem\stand_Xarms 1 68
	TEnd


	Type		Crystal
	  Anim          male\MonComb_cycle 1
	TEnd

	Type		goblin
	  Anim          male\MonComb_cycle 1
	TEnd

	Type		Bobcat
	  Anim          fem\bobcat_contact_on_couch 1
	TEnd
MEnd

#############################################################

Move Ready_Xarms_look
	Include sequencers\Members\Readies.inc
	Flags		Cycle
	CycleMove	Ready_Xarms_look
	CycleMove	Ready_Xarms_post
	Priority        0
	Scale		0.5

	Type		male
	  Anim          male\idle_xarms_look 1
	TEnd

	Type		huge
	  Anim          huge\stand_Xarms 68 90
	TEnd

	Type		fem
	  Anim          fem\idle_xarms_look 1
	TEnd

	Type		Crystal
	  Anim          male\MonComb_cycle 1
	TEnd

	Type		goblin
	  Anim          male\MonComb_cycle 1
	TEnd

	Type		Bobcat
	  Anim          fem\bobcat_contact_on_couch 1
	TEnd
MEnd

## XARMS T0 HIPS ###########################################################

Move Ready_Xarms_post
	Include sequencers\Members\Readies.inc
	Flags		Cycle
	CycleMove	Ready_2Hips_look
	Priority        0
	Scale		0.8
	Interpolate	15

	Type		male
	  Anim          male\stand_Xarms 90
	TEnd

	Type		huge
	  Anim          huge\stand_Xarms 90
	TEnd

	Type		fem
	  Anim          fem\stand_Xarms 90
	TEnd

	Type		Crystal
	  Anim          male\MonComb_cycle 1
	TEnd

	Type		goblin
	  Anim          male\MonComb_cycle 1
	TEnd

	Type		Bobcat
	  Anim          fem\bobcat_contact_on_couch 1
	TEnd
MEnd

#############################################################

Move postReady
	Include sequencers\Members\Readies.inc
	NextMove        Ready
	Priority        0
	Scale           0.80
	Interpolate     8

	Type		male
	  Anim          male\post_ready 7 24
	TEnd

	Type		fem
	  Anim          fem\post_ready 10
	TEnd

	Type		Crystal
	  Anim          male\MonComb_cycle 1
	TEnd

	Type		Bobcat
	  Anim          fem\bobcat_contact_on_couch 1
	TEnd
MEnd

#############################################################

Move postReady_back
	Include sequencers\Members\Readies.inc
	NextMove        Ready
	Priority        0

	Type		male
	  Anim          male\postback 3
	TEnd

	Type		fem
	  Anim          fem\post_ready 1
	TEnd

	Type		Crystal
	  Anim          male\MonComb_cycle 1
	TEnd

	Type		Bobcat
	  Anim          fem\bobcat_contact_on_couch 1
	TEnd
MEnd

#############################################################

Move postReady_right
	Include sequencers\Members\Readies.inc
	NextMove        Ready
	Priority        0

	Type		male
	  Anim          male\postback_right 1
	TEnd

	Type		huge
	  Anim		huge\postback_right 1
	TEnd

	Type		fem
	  Anim		fem\post_ready 1
	TEnd

	Type		Crystal
	  Anim          male\MonComb_cycle 1
	TEnd

	Type		Bobcat
	  Anim          fem\bobcat_contact_on_couch 1
	TEnd
MEnd

## PROFILE SCREEN READY ###########################################################

Move ProfileReady
	Requires	PROFILE
	Interrupts	<Ready>
	Include sequencers\Members\Readies.inc
	Flags		Cycle
	Priority        1

	Type		male
	  Anim          male\Skel_ready2 1
	TEnd

	Type		huge
	  Anim          huge\Skel_ready 1 60
	TEnd

	Type		fem
	  Anim          fem\Skel_ready2 1
	TEnd

MEnd

## COSTUME SELECT SCREEN READY ###########################################################

Move CostumeReady
	Requires	PROFILE HIGH
	Interrupts	<Ready> BodyReady
	Include sequencers\Members\Readies.inc
	Flags		Cycle
	Priority        2

	Type		male
	  Anim          male\costume_ready 1
	TEnd

	Type		huge
	  Anim          huge\costume_ready 1
	TEnd

	Type		fem
	  Anim          fem\costume_ready 1
	TEnd

MEnd

## COSTUME SELECT SCREEN COMBAT READY ###########################################################

Move Costume_CombatReady
	Requires	PROFILE COMBAT
	Interrupts	<Ready> BodyReady
	Include sequencers\Members\Readies.inc
	Flags		Cycle FIXWEAPONREG
	Priority        2
	Scale		0.375

	Type		male
	  Anim          male\Profile_CombatReady 1
	TEnd
MEnd

## COSTUME SELECT SCREEN RIGHTHAND WEAPON READY ###########################################################

Move Costume_RightHand_Weapon_Ready
	Requires	PROFILE WEAPON COMBAT RIGHTHAND
	Priority        4
	Interrupts	<Ready> BodyReady
	Include sequencers\Members\Readies.inc
	Flags		Cycle FIXWEAPONREG
	Scale           0.50

	Type		male
	  Anim          male\Profile_CombatReady_Righthand 1 31
	TEnd
MEnd

## ZOOMED - DUAL PISTOLS ###########################################################

Move Costume_Sword_Zoomed
	Requires	PROFILE WEAPON COMBAT RIGHTHAND SWORD VERYCLOSE
	Priority        5
	Interrupts	<Ready> BodyReady
	Include sequencers\Members\Readies.inc
	Flags		Cycle FIXWEAPONREG
	Scale           0.50

	Type		male
	  Anim          male\profile_RighthandWeapon_Ready 1
	TEnd
MEnd

## COSTUME SELECT SCREEN RIGHTHAND WEAPON READY ###########################################################

Move Costume_EpicRightHand_Weapon_Ready
	Requires	PROFILE WEAPON COMBAT EPICRIGHTHAND
	Priority        4
	Interrupts	<Ready> BodyReady
	Include sequencers\Members\Readies.inc
	Flags		Cycle FIXWEAPONREG
	Scale           0.50

	Type		male
	  Anim          male\wep_mace_projectile 112
	TEnd
MEnd

## COSTUME SELECT SCREEN DUALHAND WEAPON READY ###########################################################

Move Costume_DualHand_Weapon_Ready
	Requires	PROFILE WEAPON COMBAT DUALHAND
	Priority        4
	Interrupts	<Ready> BodyReady
	Include sequencers\Members\Readies.inc
	Flags		Cycle FIXWEAPONREG
	Scale		0.50

	Type		male
	  Anim		male\Profile_CombatReady_Dualhand 1 31
	TEnd
MEnd

## ZOOMED - CLAWS ###########################################################

Move Costume_Claws_Zoomed
	Requires	PROFILE WEAPON COMBAT DUALHAND CLAW VERYCLOSE
	Priority        5
	Interrupts	<Ready> BodyReady
	Include sequencers\Members\Readies.inc
	Flags		Cycle FIXWEAPONREG
	Scale		0.50

	Type		male
	  Anim		male\profile_Claws_Ready 1
	TEnd
MEnd

## ZOOMED - DUAL PISTOLS ###########################################################

Move Costume_DualPistol_Zoomed
	Requires	PROFILE WEAPON COMBAT DUALHAND PISTOLS VERYCLOSE
	Priority        5
	Interrupts	<Ready> BodyReady
	Include sequencers\Members\Readies.inc
	Flags		Cycle FIXWEAPONREG
	Scale		0.50

	Type		male
	  Anim		male\profile_DualPistol_Ready 1
	TEnd
MEnd

## ZOOMED - DUAL BLADES ###########################################################

Move Costume_DualBlades_Zoomed
	Requires	PROFILE WEAPON COMBAT DUALHAND SWORD VERYCLOSE
	Priority        5
	Interrupts	<Ready> BodyReady
	Include sequencers\Members\Readies.inc
	Flags		Cycle FIXWEAPONREG
	Scale		0.50

	Type		male
	  Anim		male\profile_DualPistol_Ready 1
	TEnd
MEnd

## COSTUME SELECT SCREEN BOW READY ###########################################################

Move Costume_Bow_Ready
	Requires	PROFILE WEAPON COMBAT LEFTHAND
	Priority	4
	Interrupts	<Ready> BodyReady
	Include sequencers\Members\Readies.inc
	Flags		Cycle FIXWEAPONREG
	Scale		0.50

	Type		male
	  Anim		male\Profile_CombatReady_Bow 1 31
	TEnd
MEnd

## ZOOMED - BOW ###########################################################

Move Costume_Bow_Zoomed
	Requires	PROFILE WEAPON COMBAT LEFTHAND BOW
	Priority	5
	Interrupts	<Ready> BodyReady
	Include sequencers\Members\Readies.inc
	Flags		Cycle FIXWEAPONREG
	Scale		0.50

	Type		male
	  Anim		male\Profile_CombatReady_Bow 1 31
	TEnd
MEnd

## COSTUME SELECT SCREEN SHIELD READY ###########################################################

Move Costume_Shield_Ready
	Requires	PROFILE WEAPON COMBAT SHIELD RIGHTHAND ##RIGHTHAND IS USED SO WEAPON WILL APPEAR TOO
	Priority        15
	Interrupts	<Ready> BodyReady
	Include sequencers\Members\Readies.inc
	Flags		Cycle FIXWEAPONREG
	Scale		0.50

	Type		male
	  Anim		male\profile_Shield_Ready 1 31
	TEnd
MEnd

## COSTUME SELECT SCREEN RIFLE READY ###########################################################

Move Costume_Rifle_Ready
	Requires	PROFILE WEAPON COMBAT TWOHAND GUN
	Interrupts	<Ready> BodyReady
	Include sequencers\Members\Readies.inc
	Flags		Cycle FIXWEAPONREG
	Priority        11
	Scale		0.5

	Type		male
	  Anim          male\Profile_CombatReady_Shotgun 1
	TEnd

	Type		fem
	  Anim          fem\Profile_CombatReady_Shotgun 1
	TEnd

	Type		huge
	  Anim          huge\Profile_CombatReady_Shotgun 1
	TEnd
MEnd

## COSTUME SELECT SCREEN BEAM RIFLE READY ###########################################################

Move Costume_Beam_Rifle_Ready
	Requires	PROFILE WEAPON COMBAT TWOHAND BEAMRIFLE
	Interrupts	<Ready> BodyReady
	Include sequencers\Members\Readies.inc
	Flags		Cycle FIXWEAPONREG
	Priority        15
	Scale		0.5

	Type		male
	  Anim          male\beamrifle_ready 1 41
	TEnd

	Type		huge
	  Anim          huge\beamrifle_ready 1 41
	TEnd

	Type		fem
	  Anim          fem\beamrifle_ready 1 41
	TEnd

MEnd

## COSTUME SELECT SCREEN RIGHTHAND WEAPON READY ###########################################################

Move Costume_Titan_Weapon_Ready
	Requires	PROFILE WEAPON COMBAT 2HLARGE
	Priority        14
	Interrupts	<Ready> BodyReady
	Include sequencers\Members\Readies.inc
	Flags		Cycle FIXWEAPONREG
	Scale           0.50

	Type		male
	  Anim          male\2hlarge_ready 1 31
	TEnd

	Type		huge
	  Anim          huge\2hlarge_ready 1 31
	TEnd

	Type		fem
	  Anim          fem\2hlarge_ready 1 31
	TEnd

MEnd

## COSTUME SELECT SCREEN KATANA READY ###########################################################

Move Costume_Katana_Ready
	Requires	PROFILE WEAPON COMBAT KATANA LEFTHAND
	Interrupts	<Ready> BodyReady
	Include sequencers\Members\Readies.inc
	Flags		Cycle FIXWEAPONREG
	Priority        11
	Scale           0.50

	Type		male
	  Anim          male\Profile_CombatReady_Katana 1 31
	TEnd

	Type		fem
	  Anim          fem\Profile_CombatReady_Katana 1 31
	TEnd

	Type		huge
	  Anim          huge\Profile_CombatReady_Katana 1 31
	TEnd
MEnd

## 'PROP' READY ###########################################################

Move Prop_ready_1
	Requires	PROP
	Interrupts	Ready
	Include sequencers\Members\Readies.inc
	Flags		Cycle
	Priority        1
	Interpolate	8

	Type		male
	  Anim          male\shotgun_observe_L 1
	  PlayFx        WEAPONS\SHOTGUN.FX  0 Constant
	  Scale		0.5
	TEnd

	Type		swat
	  Anim          male\shotgun_observe_L 1
	  PlayFx        WEAPONS\SHOTGUN.FX  0 Constant
	  Scale		0.5
	TEnd

	Type		shield
	  Anim          male\shield_lookright 1
	  Scale		0.5
	TEnd
MEnd

Move Prop_ready_2
	Requires	PROP
	Interrupts	Ready
	Include sequencers\Members\Readies.inc
	Flags		Cycle
	Priority        1
	Interpolate	8

	Type		male
	  Anim          male\shotgun_look 1
	  PlayFx        WEAPONS\SHOTGUN.FX  0 Constant
	  Scale		0.3
	TEnd

	Type		swat
	  Anim          male\shotgun_look 1
	  PlayFx        WEAPONS\SHOTGUN.FX  0 Constant
	  Scale		0.3
	TEnd

	Type		shield
	  Anim          male\shield_lookright 1
	  Scale		0.5
	TEnd
MEnd

Move Prop_ready_3
	Requires	PROP
	Interrupts	Ready
	Include sequencers\Members\Readies.inc
	Flags		Cycle
	Priority        1
	Interpolate	8

	Type		male
	  Anim          male\shield_lookright 1
	  PlayFx        WEAPONS\swat_shield.fx  0 Constant
	  PlayFx        WEAPONS\Riot_Baton.fx  0 Constant
	  Scale		0.5
	TEnd

	Type		swat
	  Anim          male\shield_lookright 1
	  PlayFx        WEAPONS\swat_shield.fx  0 Constant
	  PlayFx        WEAPONS\Riot_Baton.fx  0 Constant
	  Scale		0.5
	TEnd

	Type		shield
	  Anim          male\shield_lookright 1
	  Scale		0.5
	TEnd
MEnd

## COMBAT READY ###########################################################

Move PreCombat
	Requires	COMBAT
	Priority        1
	Interrupts	<Ready> PostWepReady
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <Combat> <AttackIRQ> <MoveIRQ> <IRQEmotesCombat>
	Flags		ReqInputs
	NextMove        CombatReady
	Scale           0.50

	Type		male
	  Anim          male\pre_combat 3
	TEnd

	Type		huge
	  Anim          huge\precombat 1 27
	TEnd

	Type		fem
	  Anim          fem\pre_combat 4
	TEnd
MEnd

Move CombatReady
	Requires	COMBAT
	Priority        1
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <Combat> <AttackIRQ> <MoveIRQ> <IRQEmotesCombat>
	Flags		Cycle
	NextMove        postReady_back
	Scale           0.50

	Type		male
	  Anim          male\combat_cycle 1 31
	TEnd

	Type		huge
	  Anim          huge\combat_cycle 1 31
	TEnd

	Type		fem
	  Anim          fem\combat_cycle 1 31
	TEnd

	Type		Vahzilok
	  Anim          Vahzilok\skel_vah_combat_cycle 1
	TEnd
MEnd

Move CombatBlend
	Requires	COMBAT
	Priority        1
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <Combat> <AttackIRQ> <MoveIRQ> <IRQEmotesCombat>
	Flags		ReqInputs
	NextMove        CombatReady
	Scale           0.50

	Type		male
	  Anim          male\combat_cycle 1 31
	TEnd

	Type		huge
	  Anim          huge\combat_cycle 1 31
	TEnd

	Type		fem
	  Anim          fem\combat_cycle 1 31
	TEnd

	Type		Vahzilok
	  Anim          Vahzilok\skel_vah_combat_cycle 1
	TEnd
MEnd

Move Combat_half
	Requires	COMBAT
	Priority        1
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <Combat> <AttackIRQ> <MoveIRQ> <IRQEmotesCombat>
	Flags		ReqInputs
	NextMove        CombatReady
	Scale           0.70

	Type		male
	  Anim          male\combat_cycle 15 31
	TEnd

	Type		huge
	  Anim          huge\combat_cycle 16 31
	TEnd

	Type		fem
	  Anim          fem\combat_cycle 15 31
	TEnd
MEnd

Move Lazy_CombatReady
	Requires	COMBAT
	Priority        1
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <Combat> <AttackIRQ> <MoveIRQ> <IRQEmotesCombat>
	Flags		Cycle
	NextMove        postReady_back
	Scale           0.50

	Type		male
	  Anim          male\combat_cycle 1 31
	TEnd
MEnd

Move Lazy_Combat_half
	Requires	COMBAT
	Priority        1
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <Combat> <AttackIRQ> <MoveIRQ> <IRQEmotesCombat>
	Flags		ReqInputs
	NextMove        Lazy_CombatReady
	Scale           0.70

	Type		male
	  Anim          male\combat_cycle 15 31
	TEnd
MEnd

Move Lazy_Combat_Blend
	Requires	COMBAT
	Priority        1
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <Combat> <AttackIRQ> <MoveIRQ> <IRQEmotesCombat>
	Flags		ReqInputs
	NextMove        Lazy_CombatReady
	Scale           0.5
	Interpolate     15

	Type		male
	  Anim          male\combat_cycle 1 31
	TEnd
MEnd

## WEAPON COMBAT READY ###########################################################

Move PreWep
	Requires	COMBAT WEAPON
	Priority        2
	Interrupts	<Ready> PreCombat  ##<Combat>
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <WepCombat> <AttackIRQ> <MoveIRQ> <IRQEmotesRightHand>
	Flags		ReqInputs
	NextMove        WepReady
	Scale           0.50

	Type		male
	  Anim          male\pre_wep 3
	TEnd
MEnd

#############################################################

#Move Ready_To_WepReady
#	Requires	COMBAT WEAPON
#	Priority        2
#	Interrupts	<Ready> <Combat>
#	Include sequencers\Members\Combat_Weapon.inc
#	NextMove        WepReady
#	Flags		NOSIZESCALE
#	SticksOnChild	WEPSPAWN
#
#	Type		male
#	  Anim          male\draw_axe 1 30
#	TEnd
#MEnd

#############################################################

Move WepReady
	Requires	COMBAT WEAPON
	Priority        2
	Interrupts	<Ready> <Combat>
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <WepCombat> <AttackIRQ> <MoveIRQ> <IRQEmotesRightHand>
	Flags		Cycle
	NextMove        postReady_back
	Scale           0.50

	Type		male
	  Anim          male\wep_cycle 1 31
	TEnd
MEnd

Move PostWepReady
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <WepCombat> <AttackIRQ> <MoveIRQ> <IRQEmotesRightHand>
	NextMove        Ready
	Priority        0

	Type		male
	  Anim          male\post_wep 1
	TEnd
MEnd

#############################################################

Move Wep_half
	Requires	COMBAT WEAPON
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <WepCombat> <AttackIRQ> <MoveIRQ> <IRQEmotesRightHand>
	Flags		ReqInputs
	NextMove        WepReady
	Priority        2
	Scale           0.50

	Type		male
	  Anim          male\wep_cycle 15 31
	TEnd
MEnd

#############################################################

Move Lazy_WepReady
	Requires	COMBAT WEAPON
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <WepCombat> <AttackIRQ> <MoveIRQ> <IRQEmotesRightHand>
	Flags		Cycle
	NextMove        postReady_back
	Priority        2
	Scale           0.50

	Type		male
	  Anim          male\wep_cycle 1 31
	TEnd
MEnd

Move Lazy_WepReady_half
	Requires	COMBAT WEAPON
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <WepCombat> <AttackIRQ> <MoveIRQ> <IRQEmotesRightHand>
	NextMove        Lazy_WepReady
	Priority        2
	Scale           0.50

	Type		male
	  Anim          male\wep_cycle 15 31
	TEnd
MEnd

Move Blend_WepReady
	Requires	COMBAT WEAPON
	Priority        2
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <WepCombat> <AttackIRQ> <MoveIRQ> <IRQEmotesRightHand>
	Flags		Cycle
	NextMove        postReady_back
	Scale           0.50

	Type		male
	  Anim          male\wep_cycle 1 31
	TEnd
MEnd

## DUAL WIELD READY ###########################################################

#Move Ready_To_DualWield
#	Requires	COMBAT WEAPON DUAL
#	Priority	3
#	Interrupts	<Ready> <Combat> <WepCombat>
#	Include sequencers\Members\Combat_DualWeapon.inc
#	NextMove        DualWield_Ready
#	Flags		NOSIZESCALE
#	SticksOnChild	WEPSPAWN
#
#	Type		male
#	  Anim		male\DualBlade_Draw 1
#	TEnd
#MEnd

#############################################################

Move DualWield_Ready
	Requires	COMBAT WEAPON DUAL
#	Priority	3
#	Interrupts	<Ready> <combat> <WepCombat>
	Include sequencers\Members\Combat_DualWeapon.inc
	Flags		Cycle
	NextMove	postReady_back
	Scale		0.50

	Type		male
	  Anim		male\dual_cycle	1 31
	TEnd
MEnd

## BACK TO READY ###########################################################

Move BackToNeutralReady
	Requires	CLEAR
	Priority	1
	Interrupts	<Ready> <MonsterGroup> <ReactIRQ> <HitIRQ> <Combat> <WepCombat> <TwoHandCombat> <PostMoveCycle> <Block>
	Include sequencers\Members\Readies.inc
	NextMove	Ready_2Hips_look
	Interpolate     15
	Scale		0.5

	Type		male
	  Anim          male\stand_2Hhips 1 64
	TEnd

	Type		huge
	  Anim          huge\stand_2Hhips 1 64
	TEnd

	Type		fem
	  Anim          fem\stand_2Hhips 1 64
	TEnd
MEnd

## STAND CYCLE ###########################################################

Move Stand_Cycle
	Include sequencers\Members\Readies.inc
	Flags		Cycle

	Type		male
	  Anim          male\what 99 100
	TEnd

	Type		huge
	  Anim          huge\thumbsup 1
	TEnd

	Type		fem
	  Anim          fem\thumbsup 1
	TEnd
MEnd

## CROSS ARMS CYCLE ###########################################################

Move Xarms_Cycle
	Include sequencers\Members\Readies.inc
	Flags		Cycle

	Type		male
	  Anim          male\no 110 115
	TEnd

	Type		huge
	  Anim          huge\no 91 95
	TEnd

	Type		fem
	  Anim          fem\no 91 95
	TEnd
MEnd

## HANDS ON HIPS CYCLE ###########################################################

Move Hips_Cycle
	Include sequencers\Members\Readies.inc
	Flags		Cycle

	Type		male
	  Anim          male\thumbsup 99 100
	TEnd

	Type		huge
	  Anim          huge\yes 70 75
	TEnd

	Type		fem
	  Anim          fem\yes 70 75
	TEnd
MEnd

## THREAT CYCLE ###########################################################

Move Threat_Cycle
	Include sequencers\Members\Readies.inc
	Flags		Cycle

	Type		male
	  Anim          male\threat_cycle 1
	TEnd

	Type		huge
	  Anim          huge\threat_cycle 1
	TEnd

	Type		fem
	  Anim          fem\threat_cycle 1
	TEnd
MEnd

## ACTION CYCLE ###########################################################

Move Action_Cycle
	Include sequencers\Members\Readies.inc
	Flags		Cycle
	Scale           0.50

	Type		male
	  Anim          male\action_cycle 1 31
	TEnd

	Type		huge
	  Anim          huge\thumbsup 1
	TEnd

	Type		fem
	  Anim          fem\thumbsup 1
	TEnd
MEnd

## BRIAN EMOTE CYCLE ###########################################################

Move Ready_2Hips_Bri
	Include sequencers\Members\Readies.inc
	Nextmove	Ready_2Hips_look_Bri
	Scale		0.5
	Interpolate	15

	Type		male
	  Anim          male\stand_2Hhips 1 64
	TEnd

	Type		huge
	  Anim          huge\stand_2Hhips 1 64
	TEnd

	Type		fem
	  Anim          fem\stand_2Hhips 1 64
	TEnd
MEnd

Move Ready_2Hips_look_Bri
	Include sequencers\Members\Readies.inc
	Flags		Cycle
	Scale		0.5

	Type		male
	  Anim          male\idle_2hips_look 1
	TEnd

	Type		huge
	  Anim          huge\idle_2hips_look 1
	TEnd

	Type		fem
	  Anim          fem\idle_2hips_look 1
	TEnd

	Type		Crystal
	  Anim          male\MonComb_cycle 1
	TEnd

	Type		goblin
	  Anim          male\MonComb_cycle 1
	TEnd
MEnd

## SIMPLE READY ###########################################################

Move Simple_Ready
	Requires	SIMPLEREADY
	Priority        1
	Interrupts	Ready
	Include sequencers\Members\Readies.inc
	Flags		Cycle
	CycleMove	Simple_Ready_Look
	CycleMove	Simple_Ready

	Type		male
	  Anim          male\Skel_ready2 1
	TEnd

	Type		huge
	  Anim          huge\Skel_ready 1 60
	TEnd

	Type		fem
	  Anim          fem\Skel_ready2 1 60
	TEnd
MEnd

Move Simple_Ready_Look
	Requires	SIMPLEREADY
	Priority        1
	Include sequencers\Members\Readies.inc
	Flags		Cycle
	CycleMove	Simple_Ready
	CycleMove	Simple_Ready_Look
	CycleMove	Simple_Ready
	Scale		0.5

	Type		male
	  Anim          male\idle_look 1
	TEnd

	Type		huge
	  Anim          huge\idle_look 1
	TEnd

	Type		fem
	  Anim          fem\idle_look 1
	TEnd
MEnd

#############################################################
## KNEEL
#############################################################

Move Kneel
	Requires	REST
	Priority	51
	Interrupts	<IRQRest> <MonsterGroup> <Ready> <Combat> <WepCombat> <TwoHandCombat>
	Member		<DeathIRQ> <AttackIRQ> <MoveIRQ>
	Flags		Cycle
	CycleMove	Kneel_Cycle
	##Sets		CANTMOVE

	Type		male
	  Anim          male\rest_kneel 1 50
	TEnd
MEnd

Move Kneel_Cycle
	Requires	REST
	Member		<DeathIRQ> <AttackIRQ> <MoveIRQ>
	Flags		Cycle
	NextMove        Kneel_getup
	##Sets		CANTMOVE

	Type		male
	  Anim          male\rest_kneel 50 60
	TEnd
MEnd

Move Kneel_getup
	Include sequencers\Members\Readies.inc
	NextMove        Ready
	Priority	0
	Interpolate     8

	Type		male
	  Anim          male\Loot 26
	TEnd

	Type		huge
	  Anim          huge\Loot 26
	TEnd
MEnd

## KNEEL (Rest)	###########################################################

Move DualWield_Kneel_Pre
	Requires	REST COMBAT WEAPON DUAL
	Priority	54
	Interrupts	<IRQRest> <MonsterGroup> <Ready> <Combat> <WepCombat> <DualHandCombat>
	Member		<DeathIRQ> <AttackIRQ> <MoveIRQ> <IRQEmotesDualhand>
	NextMove	DualWield_Kneel
	##Sets		CANTMOVE

	Type		male
	  Anim		male\dual_rest 1 50
	TEnd
MEnd

Move DualWield_Kneel
	Requires	REST
	Member		<DeathIRQ> <AttackIRQ> <MoveIRQ> <IRQEmotesDualhand>
	NextMove	DualWield_GetUp
	Flags		Cycle
	##Sets		CANTMOVE

	Type		male
	  Anim		male\dual_rest 50 60
	TEnd
MEnd

Move DualWield_GetUp
	Requires	REST
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove	Fake_Ready_dual
	Interpolate	8

	Type		male
	  Anim		male\Loot 26
	TEnd

	Type		huge
	  Anim		huge\Loot 26
	TEnd
MEnd

## SHOTGUN KNEEL ###########################################################

Move Shotgun_Kneel
	Requires	REST COMBAT WEAPON SHOTGUN
	Priority	54
	Interrupts	<IRQRest> <MonsterGroup> <Ready> <Combat> <WepCombat> <TwoHandCombat> <DualHandCombat>
	Member		<DeathIRQ> <AttackIRQ> <MoveIRQ>
	Flags		Cycle
	CycleMove	Shotgun_Kneel
	CycleMove	Shotgun_Kneel2
	NextMove        Kneel_getup
	##Sets		CANTMOVE
	Scale		0.4

	Type		male
	  Anim          male\shotgun_look_kneel 1
	  PlayFx        WEAPONS\SHOTGUN.FX  0 Constant
	TEnd
MEnd

Move Shotgun_Kneel2
	Requires	REST COMBAT WEAPON SHOTGUN
	Member		<DeathIRQ> <AttackIRQ> <MoveIRQ>
	Flags		Cycle
	CycleMove	Shotgun_Kneel2
	CycleMove	Shotgun_Kneel
	NextMove        Kneel_getup
	##Sets		CANTMOVE
	Scale		0.6

	Type		male
	  Anim          male\shotgun_look_kneel 1
	  PlayFx        WEAPONS\SHOTGUN.FX  0 Constant
	TEnd
MEnd

## SHIELD KNEEL (Rest) ###########################################################

Move Shield_Kneel_Pre
	Requires	REST SHIELD #COMBAT WEAPON
	Priority	64
	Interrupts	<IRQRest> <ShieldReady>
	Member		<DeathIRQ> <AttackIRQ> <MoveIRQ>
	NextMove	Shield_Kneel
	##Sets		CANTMOVE

	Type		male
	  Anim		male\shield_rest 1 50
	TEnd
MEnd

Move Shield_Kneel
	Requires	REST
	Member		<DeathIRQ> <AttackIRQ> <MoveIRQ>
	NextMove	Shield_GetUp
	Flags		Cycle ReqInputs
	##Sets		CANTMOVE

	Type		male
	  Anim		male\shield_rest 50 60
	TEnd
MEnd

Move Shield_GetUp
	Requires	REST
	##Member		<DeathIRQ> <ShieldCombat> <ReactIRQ> <HitIRQ> <AttackIRQ> <MoveIRQ>
	Include sequencers\Members\Combat_Shield.inc
	NextMove	Shield_Ready
	Interpolate	15

	Type		male
	  Anim		male\Loot 26
	TEnd
MEnd

## KNEEL (Rest)	###########################################################

Move TwoHandedLarge_Kneel_Pre
	Requires	REST WEAPON COMBAT 2HLARGE
	Priority	54
	Interrupts	<IRQRest> <MonsterGroup> <Ready> <Combat> <WepCombat> <TwoHandCombat>
	Member		<DeathIRQ> <AttackIRQ> <MoveIRQ> <IRQEmotesDualhand>
	NextMove	TwoHandedLarge_Kneel
	##Sets		CANTMOVE

	Type		male
	  Anim		male\dual_rest 1 50
	TEnd
MEnd

Move TwoHandedLarge_Kneel
	Requires	REST
	Member		<DeathIRQ> <AttackIRQ> <MoveIRQ> <IRQEmotesDualhand>
	NextMove	TwoHandedLarge_GetUp
	Flags		Cycle
	##Sets		CANTMOVE

	Type		male
	  Anim		male\dual_rest 50 60
	TEnd
MEnd

Move TwoHandedLarge_GetUp
	Requires	REST
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove	Fake_Ready_dual
	Interpolate	8

	Type		male
	  Anim		male\Loot 26
	TEnd

	Type		huge
	  Anim		huge\Loot 26
	TEnd
MEnd

#############################################################

Include sequencers\player\readies_fake.inc
Include sequencers\player\readies_air.inc