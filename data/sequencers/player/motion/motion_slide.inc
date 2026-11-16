#############################################################
## SLIDE MOVEMENT
#############################################################

#############################################################
## SLIDING - Slides are environmental sliding (ice, ski slope, etc)
#############################################################

Move Slide
	Requires	SLIDING
	Priority	11
	Interrupts	<Ready> <MoveIRQ> <IRQSlide>
	Member		<DeathIRQ> <AttackIRQ> <BasicSlide> <GroundMoveALL>
	Flags		Cycle
	CycleMove	Slide_Cycle
	NextMove	Slide_Post

	Type		male
	  Anim		male\iceslide_forward 15 31
	  PlayFX	feet\oilslick.fx  0 Constant
	TEnd

	Type		huge
	  Anim		huge\iceslide_forward 15 31
	  PlayFX	feet\oilslick.fx  0 Constant
	TEnd

	Type		fem
	  Anim		fem\iceslide_forward 15	31
	  PlayFX	feet\oilslick.fx  0 Constant
	TEnd
MEnd

Move Slide_Cycle
	Requires	SLIDING
	Member		<DeathIRQ> <AttackIRQ> <BasicSlide> <GroundMoveALL>
	Flags		Cycle ##FinishCycle
	NextMove	Slide_Post

	Type		male
	  Anim		male\iceslide_forward 1	31
	  PlayFX	feet\oilslick.fx  0 Constant
	TEnd
MEnd

#############################################################

Move Slide_Post
	Include sequencers\Members\Readies.inc
	NextMove	Ready
	Scale		0.80

	Type		male
	  Anim		male\post_ready	7 24
	TEnd

	Type		huge
	  Anim		huge\postback 3	40
	TEnd

	Type		fem
	  Anim		fem\post_ready 7 24
	TEnd
MEnd

## SLIDING LEFT ###########################################################

Move Slide_Left_Pre
	Requires	SLIDING	STEPLEFT
	Priority	12
	Interrupts	<Ready> <MoveIRQ> <IRQSlide> Slide_Pre
	Member		<DeathIRQ> <AttackIRQ> <BasicSlide> <GroundMoveALL>
	Flags		Cycle
	CycleMove	Slide_Left_Cycle
	NextMove	Slide_Post


	Type		male
	  Anim		male\iceslide_34left 15	31   ##slippery_left 10
	  PlayFX	feet\oilslick.fx  0 Constant
	TEnd
MEnd

Move Slide_Left_Cycle
	Requires	SLIDING	STEPLEFT
	Interrupts	Slide_Cycle
	Member		<DeathIRQ> <AttackIRQ> <BasicSlide> <GroundMoveALL>
	Flags		Cycle
	NextMove	Slide_Post

	Type		male
	  Anim		male\iceslide_34left 1 31   ##iceslide_forward 1 31
	  PlayFX	feet\oilslick.fx  0 Constant
	TEnd
MEnd

## SLIDING RIGHT ###########################################################

Move Slide_Right_Pre
	Requires	SLIDING	STEPRIGHT
	Priority	12
	Interrupts	<Ready> <MoveIRQ> <IRQSlide> Slide_Pre
	Member		<DeathIRQ> <AttackIRQ> <BasicSlide> <GroundMoveALL>
	Flags		Cycle
	CycleMove	Slide_Right_Cycle
	NextMove	Slide_Post

	Type		male
	  Anim		male\iceslide_34right 15 31
	  PlayFX	feet\oilslick.fx  0 Constant
	TEnd
MEnd

Move Slide_Right_Cycle
	Requires	SLIDING	STEPRIGHT
	Priority	12
	Interrupts	Slide_Cycle
	Member		<DeathIRQ> <AttackIRQ> <BasicSlide> <GroundMoveALL>
	Flags		Cycle
	NextMove	Slide_Post

	Type		male
	  Anim		male\iceslide_34right 1	31
	  PlayFX	feet\oilslick.fx  0 Constant
	TEnd
MEnd

## COMBAT ICESLIDE POST ###########################################################

Move Slide_Combat_Post
	Requires	COMBAT
	Priority	1
	Interrupts	SLIDING
	Include sequencers\Members\combat.inc
	NextMove	CombatReady
	Scale		0.50

	Type		male
	  Anim		male\combat_cycle 21 31
	TEnd

	Type		huge
	  Anim		huge\combat_cycle 21 31
	TEnd

	Type		fem
	  Anim		fem\combat_cycle 21 31
	TEnd
MEnd

## WEAPON SLIDE FORWARD ###########################################################

Move Slide_Weapon
	Requires	SLIDING COMBAT WEAPON #CLUB
	Priority	14
	Interrupts	<Ready> <MoveIRQ> <IRQSlide> <BasicSlide>
	Member		<DeathIRQ> <AttackIRQ> <WepSlide> <GroundMoveALL>
	Flags		Cycle
	CycleMove	Slide_Weapon_Cycle
	NextMove	Slide_Weapon_Post

	Type		male
	  Anim		male\wep_iceslide_forward 15 31
	TEnd
MEnd

Move Slide_Weapon_Cycle
	Requires	COMBAT WEAPON SLIDING
	Member		<DeathIRQ> <AttackIRQ> <WepSlide> <GroundMoveALL>
	Flags		Cycle
	NextMove	W_IceslidePost

	Type		male
	  Anim		male\wep_iceslide_forward 1 31
	TEnd
MEnd

#############################################################

Move Slide_Weapon_Post
	Requires	COMBAT WEAPON
	Include sequencers\Members\combat_weapon.inc
	NextMove	WepReady
	Scale		0.50

	Type		male
	  Anim		male\wep_cycle 15 31
	TEnd
MEnd

## WEAPON SLIDE BACK ###########################################################

Move Slide_Weapon_Back_Pre
	Requires	COMBAT WEAPON SLIDING BACKWARD #CLUB
	Priority	15
	Interrupts	<Ready> <MoveIRQ> <IRQSlide> <BasicSlide>
	Member		<DeathIRQ> <AttackIRQ> <WepSlide> <GroundMoveALL>
	Flags		Cycle
	CycleMove	Slide_Weapon_Back_Cycle
	NextMove	Slide_Weapon_Post

	Type		male
	  Anim		male\wep_iceslide_backward 15 31
	TEnd
MEnd

Move Slide_Weapon_Back_Cycle
	Requires	COMBAT WEAPON SLIDING BACKWARD
	Member		<DeathIRQ> <AttackIRQ> <WepSlide> <GroundMoveALL>
	Flags		Cycle
	NextMove	Slide_Weapon_Post

	Type		male
	  Anim		male\wep_iceslide_backward 1 31
	TEnd
MEnd

## WEAPON SLIDE RIGHT ###########################################################

Move Slide_Weapon_Right_Pre
	Requires	STEPRIGHT COMBAT WEAPON	SLIDING #CLUB
	Priority	15
	Interrupts	<Ready> <MoveIRQ> <IRQSlide> <BasicSlide> Slide_Weapon
	Member		<DeathIRQ> <AttackIRQ> <WepSlide> <GroundMoveALL>
	Flags		Cycle
	CycleMove	Slide_Weapon_Right_Cycle
	NextMove	Slide_Weapon_Post

	Type		male
	  Anim		male\wep_iceslide_right	15 31
	TEnd
MEnd

Move Slide_Weapon_Right_Cycle
	Requires	STEPRIGHT COMBAT WEAPON	SLIDING #CLUB
	Priority	15
	Interrupts	Slide_Weapon_Cycle
	Member		<DeathIRQ> <AttackIRQ> <WepSlide> <GroundMoveALL>
	Flags		Cycle
	NextMove	Slide_Weapon_Post

	Type		male
	  Anim		male\wep_iceslide_right	1 31
	TEnd
MEnd

## WEAPON SLIDE LEFT ###########################################################

Move Slide_Weapon_Left_Pre
	Requires	STEPLEFT COMBAT	WEAPON SLIDING #CLUB
	Priority	15
	Interrupts	<Ready> <MoveIRQ> <IRQSlide> <BasicSlide> Slide_Weapon
	Member		<DeathIRQ> <AttackIRQ> <WepSlide> <GroundMoveALL>
	Flags		Cycle
	CycleMove	Slide_Weapon_Left_Cycle
	NextMove	Slide_Weapon_Post

	Type		male
	  Anim		male\wep_iceslide_left 15 31
	TEnd
MEnd

Move Slide_Weapon_Left_Cycle
	Requires	STEPLEFT COMBAT	WEAPON SLIDING #CLUB
	Priority	15
	Interrupts	Slide_Weapon_Cycle
	Member		<DeathIRQ> <AttackIRQ> <WepSlide> <GroundMoveALL>
	Flags		Cycle
	NextMove	Slide_Weapon_Post

	Type		male
	  Anim		male\wep_iceslide_left 1 31
	TEnd
MEnd

## SHOTGUN SLIDE POST ###########################################################

Move Slide_Gun_Post
	Requires	COMBAT WEAPON GUN #SLIDING
	Priority	3
	Interrupts	Slide_Weapon_Post
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove	ShotgunReady
	Flags		FixArmReg #Cycle
	Scale		0.50

	Type		male
	  Anim		male\rocket_cycle 15 31
	TEnd

	Type		huge
	  Anim		huge\rocket_cycle 15 31
	TEnd

	Type		fem
	  Anim		fem\rocket_cycle 15 31
	TEnd
MEnd

#############################################################
## DUAL WEILD SLIDE
#############################################################

## DUAL SLIDE FORWARD ###########################################################

Move Slide_Dual
	Requires	SLIDING COMBAT WEAPON DUAL #CLAW
	Priority	15
	Interrupts	<Ready> <MoveIRQ> <IRQSlide> <BasicSlide> <WepSlide>
	Member		<DeathIRQ> <AttackIRQ> <DualHandSlide> <GroundMoveALL>
	Flags		Cycle
	CycleMove	Slide_Dual_Cycle
	NextMove	Slide_Dual_Post

	Type		male
	  Anim		male\dual_iceslide_forward 15 31
	TEnd
MEnd

Move Slide_Dual_Cycle
	Requires	SLIDING COMBAT WEAPON
	Member		<DeathIRQ> <AttackIRQ> <DualHandSlide> <GroundMoveALL>
	Flags		Cycle
	NextMove	Slide_Dual_Post

	Type		male
	  Anim		male\dual_iceslide_forward 1 31
	TEnd
MEnd

## DUAL SLIDE POST ###########################################################

Move Slide_Dual_Post
	Requires	COMBAT WEAPON
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove	DualWield_Ready
	Flags		Cycle
	Scale		0.50

	Type		male
	  Anim		male\dual_cycle	1 31
	TEnd
MEnd

## DUAL SLIDE BACK ###########################################################

Move Slide_Dual_Back
	Requires	COMBAT WEAPON SLIDING BACKWARD DUAL #CLAW
	Priority	16
	Interrupts	<Ready> <MoveIRQ> <IRQSlide> <BasicSlide> <WepSlide>
	Member		<DeathIRQ> <AttackIRQ> <DualHandSlide> <GroundMoveALL>
	Flags		Cycle
	CycleMove	Slide_Dual_Back_Cycle
	NextMove	Slide_Dual_Post

	Type		male
	  Anim		male\dual_iceslide_backward 15 31
	TEnd
MEnd

Move Slide_Dual_Back_Cycle
	Requires	COMBAT	WEAPON SLIDING BACKWARD
	Member		<DeathIRQ> <AttackIRQ> <DualHandSlide> <GroundMoveALL>
	Flags		Cycle
	NextMove	Slide_Dual_Post

	Type		male
	  Anim		male\dual_iceslide_backward 1 31
	TEnd
MEnd

## DUAL SLIDE RIGHT ###########################################################

Move Slide_Dual_Right
	Requires	STEPRIGHT COMBAT WEAPON	SLIDING DUAL #CLAW
	Priority	16
	Interrupts	<Ready> <MoveIRQ> <IRQSlide> <BasicSlide> <WepSlide> Slide_Dual
	Member		<DeathIRQ> <AttackIRQ> <DualHandSlide> <GroundMoveALL>
	Flags		Cycle
	CycleMove	Slide_Dual_Right_Cycle
	NextMove	Slide_Dual_Post

	Type		male
	  Anim		male\dual_iceslide_right 15 31
	TEnd
MEnd

Move Slide_Dual_Right_Cycle
	Requires	STEPRIGHT COMBAT WEAPON	SLIDING DUAL #CLAW
	Priority	16
	Interrupts	Slide_Dual_Cycle
	Member		<DeathIRQ> <AttackIRQ> <DualHandSlide> <GroundMoveALL>
	Flags		Cycle
	NextMove	Slide_Dual_Post

	Type		male
	  Anim		male\dual_iceslide_right 1 31
	TEnd
MEnd

## DUAL SLIDE LEFT ###########################################################

Move Slide_Dual_Left
	Requires	STEPLEFT COMBAT	WEAPON SLIDING DUAL #CLAW
	Priority	16
	Interrupts	<Ready> <MoveIRQ> <IRQSlide> <BasicSlide> <WepSlide> Slide_Dual
	Member		<DeathIRQ> <AttackIRQ> <DualHandSlide> <GroundMoveALL>
	Flags		Cycle
	CycleMove	Slide_Dual_Left_Cycle
	NextMove	Slide_Dual_Post

	Type		male
	  Anim		male\dual_iceslide_left 15 31
	TEnd
MEnd

Move Slide_Dual_Left_Cycle
	Requires	STEPLEFT COMBAT	WEAPON SLIDING DUAL #CLAW
	Priority	16
	Interrupts	Slide_Dual_Cycle
	Member		<DeathIRQ> <AttackIRQ> <DualHandSlide> <GroundMoveALL>
	Flags		Cycle
	NextMove	Slide_Dual_Post

	Type		male
	  Anim		male\dual_iceslide_left 1 31
	TEnd
MEnd

#############################################################
## SHIELD SLIDE
#############################################################

## SHIELD SLIDE FORWARD ###########################################################

Move Slide_Shield
	Requires	SLIDING SHIELD #COMBAT WEAPON
	Priority	24
	Interrupts	<Ready> <ShieldReady> <MoveIRQ> <IRQSlide> <BasicSlide> <CombatSlide> <WepSlide>
	Member		<DeathIRQ> <AttackIRQ> <ShieldSlide> <GroundMoveALL> <IRQJumps>
	Flags		Cycle
	CycleMove	Slide_Shield_Cycle
	NextMove	Slide_Shield_Post

	Type		male
	  Anim		male\shield_iceslide_forward 15 31
	TEnd
MEnd

Move Slide_Shield_Cycle
	Requires	SLIDING SHIELD #COMBAT WEAPON
	Member		<DeathIRQ> <AttackIRQ> <ShieldSlide> <GroundMoveALL> <IRQJumps>
	Flags		Cycle
	NextMove	Slide_Shield_Post

	Type		male
	  Anim		male\shield_iceslide_forward 1 31
	TEnd
MEnd

############################################################

Move Slide_Shield_Post
	Requires	SHIELD #COMBAT WEAPON
	Include sequencers\Members\Combat_Shield.inc
	NextMove	Shield_CombatReady
	Scale		0.50

	Type		male
	  Anim		male\shield_Ready 1 31
	TEnd
MEnd

## SHIELD SLIDE BACK ###########################################################

Move Slide_Shield_Back
	Requires	SHIELD SLIDING BACKWARD #COMBAT WEAPON
	Priority	25
	Interrupts	<Ready> <ShieldReady> <MoveIRQ> <IRQSlide> <BasicSlide> <CombatSlide> <WepSlide>
	Member		<DeathIRQ> <AttackIRQ> <ShieldSlide> <GroundMoveALL> <IRQJumps>
	Flags		Cycle
	CycleMove	Slide_Shield_Back_Cycle
	NextMove	Slide_Shield_Post

	Type		male
	  Anim		male\shield_iceslide_backward 15 31
	TEnd
MEnd

Move Slide_Shield_Back_Cycle
	Requires	SHIELD SLIDING BACKWARD #COMBAT WEAPON
	Member		<DeathIRQ> <AttackIRQ> <ShieldSlide> <GroundMoveALL> <IRQJumps>
	Flags		Cycle
	NextMove	Slide_Shield_Post

	Type		male
	  Anim		male\shield_iceslide_backward 1 31
	TEnd
MEnd

## SHIELD SLIDE RIGHT ###########################################################

Move Slide_Shield_Right
	Requires	SHIELD SLIDING STEPRIGHT #COMBAT WEAPON
	Priority	25
	Interrupts	<Ready> <ShieldReady> <MoveIRQ> <IRQSlide> <BasicSlide> <CombatSlide> <WepSlide> Slide_Shield
	Member		<DeathIRQ> <AttackIRQ> <ShieldSlide> <GroundMoveALL> <IRQJumps>
	Flags		Cycle
	CycleMove	Slide_Shield_Right_Cycle
	NextMove	Slide_Shield_Post

	Type		male
	  Anim		male\shield_iceslide_right 15 31
	TEnd
MEnd

Move Slide_Shield_Right_Cycle
	Requires	SHIELD SLIDING STEPRIGHT #COMBAT WEAPON
	Priority	25
	Interrupts	Slide_Shield_Cycle
	Member		<DeathIRQ> <AttackIRQ> <ShieldSlide> <GroundMoveALL> <IRQJumps>
	Flags		Cycle
	NextMove	Slide_Shield_Post

	Type		male
	  Anim		male\shield_iceslide_right 1 31
	TEnd
MEnd

## SHIELD SLIDE LEFT ###########################################################

Move Slide_Shield_Left
	Requires	SHIELD SLIDING STEPLEFT #COMBAT WEAPON
	Priority	25
	Interrupts	<Ready> <ShieldReady> <MoveIRQ> <IRQSlide> <BasicSlide> <CombatSlide> <WepSlide> Slide_Shield
	Member		<DeathIRQ> <AttackIRQ> <ShieldSlide> <GroundMoveALL> <IRQJumps>
	Flags		Cycle
	CycleMove	Slide_Shield_Left_Cycle
	NextMove	Slide_Shield_Post

	Type		male
	  Anim		male\shield_iceslide_left 15 31
	TEnd
MEnd

Move Slide_Shield_Left_Cycle
	Requires	SHIELD SLIDING STEPLEFT #COMBAT WEAPON
	Priority	25
	Interrupts	Slide_Shield_Cycle
	Member		<DeathIRQ> <AttackIRQ> <ShieldSlide> <GroundMoveALL> <IRQJumps>
	Flags		Cycle
	NextMove	Slide_Shield_Post

	Type		male
	  Anim		male\shield_iceslide_left 1 31
	TEnd
MEnd

#############################################################
## "ICESLIDE" - Used by Prestige Power Slide
#############################################################

Move IceslidePre
	Requires	FORWARD	ICESLIDE
	Priority	12
	Interrupts	<Ready> <MoveIRQ> <IRQSlide>
	Member		<DeathIRQ> <AttackIRQ> <BasicSlide> <GroundMoveALL>
	Flags		Cycle
	CycleMove	IceslideCycle
	NextMove	Slide_Post

	Type		male
	  Anim		male\iceslide_forward 15 31
	TEnd

	Type		huge
	  Anim		huge\iceslide_forward 15 31
	TEnd

	Type		fem
	  Anim		fem\iceslide_forward 15	31
	TEnd
MEnd

Move IceslideCycle
	Requires	FORWARD	ICESLIDE
	Member		<DeathIRQ> <AttackIRQ> <BasicSlide> <GroundMoveALL>
	Flags		Cycle
	NextMove	Slide_Post

	Type		male
	  Anim		male\iceslide_forward 1	31
	TEnd

	Type		huge
	  Anim		huge\iceslide_forward 1	31
	TEnd

	Type		fem
	  Anim		fem\iceslide_forward 1 31
	TEnd
MEnd

## ICESLIDE BACKWARD ###########################################################

Move IceslidePre_B
	Requires	BACKWARD ICESLIDE
	Priority	12
	Interrupts	<Ready> <MoveIRQ> <IRQSlide>
	Member		<DeathIRQ> <AttackIRQ> <BasicSlide> <GroundMoveALL>
	Flags		Cycle
	CycleMove	IceslideCycle_B
	NextMove	Slide_Post

	Type		male
	  Anim		male\iceslide_backward 15 31
	TEnd

	Type		huge
	  Anim		huge\iceslide_backward 15 31
	TEnd

	Type		fem
	  Anim		fem\iceslide_backward 15 31
	TEnd
MEnd

Move IceslideCycle_B
	Requires	BACKWARD ICESLIDE
	Interrupts	<Ready> <MoveIRQ> <IRQSlide>
	Member		<DeathIRQ> <AttackIRQ> <BasicSlide> <GroundMoveALL>
	Flags		Cycle
	NextMove	Slide_Post

	Type		male
	  Anim		male\iceslide_backward 1 31
	TEnd

	Type		huge
	  Anim		huge\iceslide_backward 1 31
	TEnd

	Type		fem
	  Anim		fem\iceslide_backward 1	31
	TEnd
MEnd

## ICESLIDE LEFT ###########################################################

Move IceslidePre_L
	Requires	STEPLEFT ICESLIDE
	Priority	12
	Interrupts	<Ready> <MoveIRQ> <IRQSlide> IceslidePre
	Member		<DeathIRQ> <AttackIRQ> <BasicSlide> <GroundMoveALL>
	Flags		Cycle
	CycleMove	IceslideCycle_L
	NextMove	Slide_Post

	Type		male
	  Anim		male\iceslide_34left 15	31
	TEnd

	Type		huge
	  Anim		huge\iceslide_left 15 31
	TEnd

	Type		fem
	  Anim		fem\iceslide_left 15 31
	TEnd
MEnd

Move IceslideCycle_L
	Requires	STEPLEFT ICESLIDE
	Priority	12
	Interrupts	IceslideCycle
	Member		<DeathIRQ> <AttackIRQ> <BasicSlide> <GroundMoveALL>
	Flags		Cycle
	NextMove	Slide_Post

	Type		male
	  Anim		male\iceslide_34left 1 31
	TEnd

	Type		huge
	  Anim		huge\iceslide_left 1 31
	TEnd

	Type		fem
	  Anim		fem\iceslide_left 1 31
	TEnd
MEnd

## ICESLIDE RIGHT ###########################################################

Move IceslidePre_R
	Requires	STEPRIGHT ICESLIDE
	Priority	12
	Interrupts	<Ready> <MoveIRQ> <IRQSlide> IceslidePre
	Member		<DeathIRQ> <AttackIRQ> <BasicSlide> <GroundMoveALL>
	Flags		Cycle
	CycleMove	IceslideCycle_R
	NextMove	Slide_Post

	Type		male
	  Anim		male\iceslide_34right 15 31
	TEnd

	Type		huge
	  Anim		huge\iceslide_right 15 31
	TEnd

	Type		fem
	  Anim		fem\iceslide_right 15 31
	TEnd
MEnd

Move IceslideCycle_R
	Requires	STEPRIGHT ICESLIDE
	Priority	12
	Interrupts	IceslideCycle
	Member		<DeathIRQ> <AttackIRQ> <BasicSlide> <GroundMoveALL>
	Flags		Cycle
	NextMove	Slide_Post

	Type		male
	  Anim		male\iceslide_34right 1	31
	TEnd

	Type		huge
	  Anim		huge\iceslide_right 1 31
	TEnd

	Type		fem
	  Anim		fem\iceslide_right 1 31
	TEnd
MEnd

#############################################################
## WEAPON ICESLIDE
#############################################################

Move W_IceslidePre
	Requires	COMBAT WEAPON ICESLIDE FORWARD #CLUB
	Priority	15
	Interrupts	<Ready> <MoveIRQ> <IRQSlide> <BasicSlide>
	Member		<DeathIRQ> <AttackIRQ> <WepSlide> <GroundMoveALL>
	Flags		Cycle
	CycleMove	W_IceslideCycle
	NextMove	Slide_Weapon_Post

	Type		male
	  Anim		male\wep_iceslide_forward 15 31
	TEnd

	Type		huge
	  Anim		huge\wep_iceslide_forward 15 31
	TEnd

	Type		fem
	  Anim		fem\wep_iceslide_forward 15 31
	TEnd
MEnd

Move W_IceslideCycle
	Requires	COMBAT WEAPON ICESLIDE FORWARD
	Member		<DeathIRQ> <AttackIRQ> <WepSlide> <GroundMoveALL>
	Flags		Cycle
	NextMove	Slide_Weapon_Post

	Type		male
	  Anim		male\wep_iceslide_forward 1 31
	TEnd

	Type		huge
	  Anim		huge\wep_iceslide_forward 1 31
	TEnd

	Type		fem
	  Anim		fem\wep_iceslide_forward 1 31
	TEnd
MEnd

## WEAPON ICESLIDE BACK ###########################################################

Move W_IceslidePre_B
	Requires	COMBAT WEAPON ICESLIDE BACKWARD #CLUB
	Priority	15
	Interrupts	<Ready> <MoveIRQ> <IRQSlide> <BasicSlide>
	Member		<DeathIRQ> <AttackIRQ> <WepSlide> <GroundMoveALL>
	Flags		Cycle
	CycleMove	W_IceslideCycle_B
	NextMove	Slide_Weapon_Post

	Type		male
	  Anim		male\wep_iceslide_backward 15 31
	TEnd

	Type		huge
	  Anim		huge\wep_iceslide_backward 15 31
	TEnd

	Type		fem
	  Anim		fem\wep_iceslide_backward 15 31
	TEnd
MEnd

Move W_IceslideCycle_B
	Requires	COMBAT WEAPON ICESLIDE BACKWARD
	Member		<DeathIRQ> <AttackIRQ> <WepSlide> <GroundMoveALL>
	Flags		Cycle
	NextMove	Slide_Weapon_Post

	Type		male
	  Anim		male\wep_iceslide_backward 1 31
	TEnd

	Type		huge
	  Anim		huge\wep_iceslide_backward 1 31
	TEnd

	Type		fem
	  Anim		fem\wep_iceslide_backward 1 31
	TEnd
MEnd

## WEAPON ICESLIDE RIGHT ###########################################################

Move W_IceslidePre_R
	Requires	STEPRIGHT COMBAT WEAPON	ICESLIDE #CLUB
	Priority	15
	Interrupts	<Ready> <MoveIRQ> <IRQSlide> <BasicSlide> W_IceslidePre
	Member		<DeathIRQ> <AttackIRQ> <WepSlide> <GroundMoveALL>
	Flags		Cycle
	CycleMove	W_IceslideCycle_R
	NextMove	Slide_Weapon_Post

	Type		male
	  Anim		male\wep_iceslide_right	15 31
	TEnd

	Type		huge
	  Anim		huge\wep_iceslide_right	15 31
	TEnd

	Type		fem
	  Anim		fem\wep_iceslide_right 15 31
	TEnd
MEnd

Move W_IceslideCycle_R
	Requires	STEPRIGHT COMBAT WEAPON	ICESLIDE #CLUB
	Priority	15 #1 Higher to take priority over pre move
	Interrupts	W_IceslideCycle
	Member		<DeathIRQ> <AttackIRQ> <WepSlide> <GroundMoveALL>
	Flags		Cycle
	NextMove	Slide_Weapon_Post

	Type		male
	  Anim		male\wep_iceslide_right	1 31
	TEnd

	Type		huge
	  Anim		huge\wep_iceslide_right	1 31
	TEnd

	Type		fem
	  Anim		fem\wep_iceslide_right 1 31
	TEnd
MEnd

## WEAPON ICESLIDE LEFT ###########################################################

Move W_IceslidePre_L
	Requires	STEPLEFT COMBAT	WEAPON ICESLIDE #CLUB
	Priority	15
	Interrupts	<Ready> <MoveIRQ> <IRQSlide> <BasicSlide> W_IceslidePre
	Member		<DeathIRQ> <AttackIRQ> <WepSlide> <GroundMoveALL>
	Flags		Cycle
	CycleMove	W_IceslideCycle_L
	NextMove	Slide_Weapon_Post

	Type		male
	  Anim		male\wep_iceslide_left 15 31
	TEnd

	Type		huge
	  Anim		huge\wep_iceslide_left 15 31
	TEnd

	Type		fem
	  Anim		fem\wep_iceslide_left 15 31
	TEnd
MEnd

Move W_IceslideCycle_L
	Requires	STEPLEFT COMBAT	WEAPON ICESLIDE #CLUB
	Priority	15 #1 Higher to take priority over pre move
	Interrupts	W_IceslideCycle
	Member		<DeathIRQ> <AttackIRQ> <WepSlide> <GroundMoveALL>
	Flags		Cycle
	NextMove	Slide_Weapon_Post

	Type		male
	  Anim		male\wep_iceslide_left 1 31
	TEnd

	Type		huge
	  Anim		huge\wep_iceslide_left 1 31
	TEnd

	Type		fem
	  Anim		fem\wep_iceslide_left 1	31
	TEnd
MEnd

#############################################################
## DUAL WEILD ICESLIDE
#############################################################

Move Dual_IceslidePre
	Requires	COMBAT WEAPON ICESLIDE FORWARD DUAL #CLAW
	Priority	16
	Interrupts	<Ready> <MoveIRQ> <IRQSlide> <BasicSlide> <WepSlide>
	Member		<DeathIRQ> <AttackIRQ> <DualHandSlide> <GroundMoveALL>
	Flags		Cycle
	CycleMove	Dual_IceslideCycle
	NextMove	Slide_Dual_Post

	Type		male
	  Anim		male\dual_iceslide_forward 15 31
	TEnd
MEnd

Move Dual_IceslideCycle
	Requires	COMBAT WEAPON ICESLIDE FORWARD
	Member		<DeathIRQ> <AttackIRQ> <DualHandSlide> <GroundMoveALL>
	Flags		Cycle
	NextMove	Slide_Dual_Post

	Type		male
	  Anim		male\dual_iceslide_forward 1 31
	TEnd
MEnd

## DUAL ICESLIDE BACK ###########################################################

Move Dual_IceslidePre_Back
	Requires	COMBAT WEAPON ICESLIDE BACKWARD DUAL #CLAW
	Priority	16
	Interrupts	<Ready> <MoveIRQ> <IRQSlide> <BasicSlide> <WepSlide>
	Member		<DeathIRQ> <AttackIRQ> <DualHandSlide> <GroundMoveALL>
	Flags		Cycle
	CycleMove	Dual_Iceslide_Back_Cycle
	NextMove	Slide_Dual_Post

	Type		male
	  Anim		male\dual_iceslide_backward 15 31
	TEnd
MEnd

Move Dual_Iceslide_Back_Cycle
	Requires	COMBAT	WEAPON ICESLIDE BACKWARD
	Member		<DeathIRQ> <AttackIRQ> <DualHandSlide> <GroundMoveALL>
	Flags		Cycle
	NextMove	Slide_Dual_Post

	Type		male
	  Anim		male\dual_iceslide_backward 1 31
	TEnd
MEnd

## DUAL ICESLIDE RIGHT ###########################################################

Move Dual_IceslidePre_Right
	Requires	STEPRIGHT COMBAT WEAPON	ICESLIDE DUAL #CLAW
	Priority	16
	Interrupts	<Ready> <MoveIRQ> <IRQSlide> <BasicSlide> <WepSlide> Dual_IceslidePre
	Member		<DeathIRQ> <AttackIRQ> <DualHandSlide> <GroundMoveALL>
	Flags		Cycle
	CycleMove	Dual_Iceslide_Right_Cycle
	NextMove	Slide_Dual_Post

	Type		male
	  Anim		male\dual_iceslide_right 15 31
	TEnd
MEnd

Move Dual_Iceslide_Right_Cycle
	Requires	STEPRIGHT COMBAT WEAPON	ICESLIDE DUAL #CLAW
	Priority	17 #1 Higher to take priority over pre move
	Interrupts	<Ready> <MoveIRQ> <IRQSlide> <BasicSlide> <WepSlide> Dual_IceslideCycle
	Member		<DeathIRQ> <AttackIRQ> <DualHandSlide> <GroundMoveALL>
	Flags		Cycle
	NextMove	Slide_Dual_Post

	Type		male
	  Anim		male\dual_iceslide_right 1 31
	TEnd
MEnd

## DUAL ICESLIDE LEFT ###########################################################

Move Dual_Iceslide_Left
	Requires	STEPLEFT COMBAT	WEAPON ICESLIDE DUAL #CLAW
	Priority	16
	Interrupts	<Ready> <MoveIRQ> <IRQSlide> <BasicSlide> <WepSlide> Dual_IceslidePre
	Member		<DeathIRQ> <AttackIRQ> <DualHandSlide> <GroundMoveALL>
	Flags		Cycle
	CycleMove	Dual_Iceslide_Left_Cycle
	NextMove	Slide_Dual_Post

	Type		male
	  Anim		male\dual_iceslide_left 15 31
	TEnd
MEnd

Move Dual_Iceslide_Left_Cycle
	Requires	STEPLEFT COMBAT	WEAPON ICESLIDE DUAL #CLAW
	Priority	17 #1 Higher to take priority over pre move
	Interrupts	<Ready> <MoveIRQ> <IRQSlide> <BasicSlide> <WepSlide> Dual_IceslideCycle
	Member		<DeathIRQ> <AttackIRQ> <DualHandSlide> <GroundMoveALL>
	Flags		Cycle
	NextMove	Slide_Dual_Post

	Type		male
	  Anim		male\dual_iceslide_left 1 31
	TEnd
MEnd

#############################################################
## SHIELD ICESLIDE
#############################################################

Move Shield_Iceslide_Forward_Pre
	Requires	SHIELD ICESLIDE FORWARD #COMBAT WEAPON
	Priority	25
	Interrupts	<Ready> <ShieldReady> <MoveIRQ> <IRQSlide> <BasicSlide> <CombatSlide> <WepSlide>
	Member		<DeathIRQ> <AttackIRQ> <ShieldSlide> <GroundMoveALL>
	Flags		Cycle
	CycleMove	Shield_Iceslide_Forward
	NextMove	Slide_Shield_Post

	Type		male
	  Anim		male\shield_iceslide_forward 15 31
	TEnd
MEnd

Move Shield_Iceslide_Forward
	Requires	SHIELD ICESLIDE FORWARD #COMBAT WEAPON
	Member		<DeathIRQ> <AttackIRQ> <ShieldSlide> <GroundMoveALL>
	Flags		Cycle
	NextMove	Slide_Shield_Post

	Type		male
	  Anim		male\shield_iceslide_forward 1 31
	TEnd
MEnd

## SHIELD ICESLIDE BACK ###########################################################

Move Shield_Iceslide_Back_Pre
	Requires	SHIELD ICESLIDE BACKWARD #COMBAT WEAPON
	Priority	25
	Interrupts	<Ready> <ShieldReady> <MoveIRQ> <IRQSlide> <BasicSlide> <CombatSlide> <WepSlide><Ready> <ShieldReady> <MoveIRQ> <IRQSlide> <BasicSlide> <CombatSlide> <WepSlide>
	Member		<DeathIRQ> <AttackIRQ> <ShieldSlide> <GroundMoveALL>
	Flags		Cycle
	CycleMove	Shield_Iceslide_Back
	NextMove	Slide_Shield_Post

	Type		male
	  Anim		male\shield_iceslide_backward 15 31
	TEnd
MEnd

Move Shield_Iceslide_Back
	Requires	SHIELD ICESLIDE BACKWARD #COMBAT WEAPON
	Member		<DeathIRQ> <AttackIRQ> <ShieldSlide> <GroundMoveALL>
	Flags		Cycle
	NextMove	Slide_Shield_Post

	Type		male
	  Anim		male\shield_iceslide_backward 1 31
	TEnd
MEnd

## SHIELD ICESLIDE RIGHT ###########################################################

Move Shield_Iceslide_Right_Pre
	Requires	SHIELD ICESLIDE STEPRIGHT #COMBAT WEAPON
	Priority	25
	Interrupts	<Ready> <ShieldReady> <MoveIRQ> <IRQSlide> <BasicSlide> <CombatSlide> <WepSlide> Shield_Iceslide_Forward_Pre
	Member		<DeathIRQ> <AttackIRQ> <ShieldSlide> <GroundMoveALL>
	Flags		Cycle
	CycleMove	Shield_Iceslide_Right
	NextMove	Slide_Shield_Post

	Type		male
	  Anim		male\shield_iceslide_right 15 31
	TEnd
MEnd

Move Shield_Iceslide_Right
	Requires	SHIELD ICESLIDE STEPRIGHT #COMBAT WEAPON
	Priority	26 #1 Higher to take priority over pre move
	Interrupts	Shield_Iceslide_Forward
	Member		<DeathIRQ> <AttackIRQ> <ShieldSlide> <GroundMoveALL>
	Flags		Cycle
	NextMove	Slide_Shield_Post

	Type		male
	  Anim		male\shield_iceslide_right 1 31
	TEnd
MEnd

## SHIELD ICESLIDE LEFT ###########################################################

Move Shield_Iceslide_Left_Pre
	Requires	SHIELD ICESLIDE STEPLEFT #COMBAT WEAPON
	Priority	25
	Interrupts	<Ready> <ShieldReady> <MoveIRQ> <IRQSlide> <BasicSlide> <CombatSlide> <WepSlide> Shield_Iceslide_Forward_Pre
	Member		<DeathIRQ> <AttackIRQ> <ShieldSlide> <GroundMoveALL>
	Flags		Cycle
	CycleMove	Shield_Iceslide_Left
	NextMove	Slide_Shield_Post

	Type		male
	  Anim		male\shield_iceslide_left 15 31
	TEnd
MEnd

Move Shield_Iceslide_Left
	Requires	SHIELD ICESLIDE STEPLEFT #COMBAT WEAPON
	Priority	26 #1 Higher to take priority over pre move
	Interrupts	Shield_Iceslide_Forward
	Member		<DeathIRQ> <AttackIRQ> <ShieldSlide> <GroundMoveALL>
	Flags		Cycle
	NextMove	Slide_Shield_Post

	Type		male
	  Anim		male\shield_iceslide_left 1 31
	TEnd
MEnd

####################################
## 2h Large Ice Slide Movement
################################

## WEAPON SLIDE FORWARD ###########################################################

Move 2hlarge_IceSlide_Weapon
	Requires	ICESLIDE COMBAT WEAPON 2HLARGE FORWARD
	Priority	16
	Interrupts	<Ready> <MoveIRQ> <IRQSlide> <BasicSlide>
	Member		<DeathIRQ> <AttackIRQ> <WepSlide> <GroundMoveALL>
	Flags		Cycle
	CycleMove	2hlarge_IceSlide_Weapon_Cycle
	NextMove	2hlarge_IceSlide_Weapon_Post

	Type		male
	  Anim		male\wep_iceslide_forward_2hlarge 15 31
	TEnd

	Type		huge
	  Anim		huge\wep_iceslide_forward_2hlarge 15 31
	TEnd

	Type		fem
	  Anim		fem\wep_iceslide_forward_2hlarge 15 31
	TEnd
MEnd

Move 2hlarge_IceSlide_Weapon_Cycle
	Requires	ICESLIDE COMBAT WEAPON 2HLARGE FORWARD
	Member		<DeathIRQ> <AttackIRQ> <WepSlide> <GroundMoveALL>
	Flags		Cycle
	NextMove	2hlarge_RunPost

	Type		male
	  Anim		male\wep_iceslide_forward_2hlarge 1 31
	TEnd

	Type		huge
	  Anim		huge\wep_iceslide_forward_2hlarge 1 31
	TEnd

	Type		fem
	  Anim		fem\wep_iceslide_forward_2hlarge 1 31
	TEnd
MEnd

#############################################################

Move 2hlarge_IceSlide_Weapon_Post
	Requires	COMBAT WEAPON 2HLARGE
	Include sequencers\Members\combat_weapon.inc
	NextMove	2hlargeReady
	Scale		0.50

	Type		male
	  Anim		male\2hlarge_ready 1
	TEnd

	Type		huge
	  Anim		huge\2hlarge_ready 1
	TEnd

	Type		fem
	  Anim		fem\2hlarge_ready 1
	TEnd
MEnd

## WEAPON SLIDE BACK ###########################################################

Move 2hlarge_IceSlide_Weapon_Back_Pre
	Requires	COMBAT WEAPON ICESLIDE BACKWARD 2HLARGE
	Priority	16
	Interrupts	<Ready> <MoveIRQ> <IRQSlide> <BasicSlide>
	Member		<DeathIRQ> <AttackIRQ> <WepSlide> <GroundMoveALL>
	Flags		Cycle
	CycleMove	2hlarge_IceSlide_Weapon_Back_Cycle
	NextMove	2hlarge_IceSlide_Weapon_Post

	Type		male
	  Anim		male\wep_iceslide_backward_2hlarge 15 31
	TEnd

	Type		huge
	  Anim		huge\wep_iceslide_backward_2hlarge 15 31
	TEnd

	Type		fem
	  Anim		fem\wep_iceslide_backward_2hlarge 15 31
	TEnd
MEnd

Move 2hlarge_IceSlide_Weapon_Back_Cycle
	Requires	COMBAT WEAPON ICESLIDE BACKWARD
	Member		<DeathIRQ> <AttackIRQ> <WepSlide> <GroundMoveALL>
	Flags		Cycle
	NextMove	2hlarge_IceSlide_Weapon_Post

	Type		male
	  Anim		male\wep_iceslide_backward_2hlarge 1 31
	TEnd

	Type		huge
	  Anim		huge\wep_iceslide_backward_2hlarge 1 31
	TEnd

	Type		fem
	  Anim		fem\wep_iceslide_backward_2hlarge 1 31
	TEnd
MEnd

## WEAPON SLIDE RIGHT ###########################################################

Move 2hlarge_IceSlide_Weapon_Right_Pre
	Requires	STEPRIGHT COMBAT WEAPON	ICESLIDE 2HLARGE
	Priority	16
	Interrupts	<Ready> <MoveIRQ> <IRQSlide> <BasicSlide>
	Member		<DeathIRQ> <AttackIRQ> <WepSlide> <GroundMoveALL>
	Flags		Cycle
	CycleMove	2hlarge_IceSlide_Weapon_Right_Cycle
	NextMove	2hlarge_IceSlide_Weapon_Post

	Type		male
	  Anim		male\wep_iceslide_right_2hlarge	15 31
	TEnd

	Type		huge
	  Anim		huge\wep_iceslide_right_2hlarge	15 31
	TEnd

	Type		fem
	  Anim		fem\wep_iceslide_right_2hlarge	15 31
	TEnd

MEnd

Move 2hlarge_IceSlide_Weapon_Right_Cycle
	Requires	STEPRIGHT COMBAT WEAPON	ICESLIDE 2HLARGE
	Priority	16
	Member		<DeathIRQ> <AttackIRQ> <WepSlide> <GroundMoveALL>
	Flags		Cycle
	NextMove	2hlarge_IceSlide_Weapon_Post

	Type		male
	  Anim		male\wep_iceslide_right_2hlarge	1 31
	TEnd

	Type		huge
	  Anim		huge\wep_iceslide_right_2hlarge	1 31
	TEnd

	Type		fem
	  Anim		fem\wep_iceslide_right_2hlarge	1 31
	TEnd
MEnd

## WEAPON SLIDE LEFT ###########################################################

Move 2hlarge_IceSlide_Weapon_Left_Pre
	Requires	STEPLEFT COMBAT	WEAPON ICESLIDE 2HLARGE
	Priority	16
	Interrupts	<Ready> <MoveIRQ> <IRQSlide> <BasicSlide>
	Member		<DeathIRQ> <AttackIRQ> <WepSlide> <GroundMoveALL>
	Flags		Cycle
	CycleMove	2hlarge_IceSlide_Weapon_Left_Cycle
	NextMove	2hlarge_IceSlide_Weapon_Post

	Type		male
	  Anim		male\wep_iceslide_left_2hlarge 15 31
	TEnd

	Type		huge
	  Anim		huge\wep_iceslide_left_2hlarge 15 31
	TEnd

	Type		fem
	  Anim		fem\wep_iceslide_left_2hlarge 15 31
	TEnd
MEnd

Move 2hlarge_IceSlide_Weapon_Left_Cycle
	Requires	STEPLEFT COMBAT	WEAPON ICESLIDE 2HLARGE #CLUB
	Priority	16
	Member		<DeathIRQ> <AttackIRQ> <WepSlide> <GroundMoveALL>
	Flags		Cycle
	NextMove	2hlarge_IceSlide_Weapon_Post

	Type		male
	  Anim		male\wep_iceslide_left_2hlarge 1 31
	TEnd

	Type		huge
	  Anim		huge\wep_iceslide_left_2hlarge 1 31
	TEnd

	Type		fem
	  Anim		fem\wep_iceslide_left_2hlarge 1 31
	TEnd
MEnd

####################################
## 2h Large Ice Slide Movement
################################

## WEAPON SLIDE FORWARD ###########################################################

Move 2hlarge_Slide_Weapon
	Requires	SLIDING COMBAT WEAPON 2HLARGE
	Priority	14
	Interrupts	<Ready> <MoveIRQ> <IRQSlide> <BasicSlide>
	Member		<DeathIRQ> <AttackIRQ> <WepSlide> <GroundMoveALL>
	Flags		Cycle
	CycleMove	2hlarge_Slide_Weapon_Cycle
	NextMove	2hlarge_Slide_Weapon_Post

	Type		male
	  Anim		male\wep_iceslide_forward_2hlarge 15 31
	TEnd

	Type		huge
	  Anim		huge\wep_iceslide_forward_2hlarge 15 31
	TEnd

	Type		fem
	  Anim		fem\wep_iceslide_forward_2hlarge 15 31
	TEnd
MEnd

Move 2hlarge_Slide_Weapon_Cycle
	Requires	COMBAT WEAPON SLIDING 2HLARGE
	Member		<DeathIRQ> <AttackIRQ> <WepSlide> <GroundMoveALL>
	Flags		Cycle
	NextMove	2hlarge_RunPost

	Type		male
	  Anim		male\wep_iceslide_forward_2hlarge 1 31
	TEnd

	Type		huge
	  Anim		huge\wep_iceslide_forward_2hlarge 1 31
	TEnd

	Type		fem
	  Anim		fem\wep_iceslide_forward_2hlarge 1 31
	TEnd
MEnd

#############################################################

Move 2hlarge_Slide_Weapon_Post
	Requires	COMBAT WEAPON 2HLARGE
	Include sequencers\Members\combat_weapon.inc
	NextMove	2hlargeReady
	Scale		0.50

	Type		male
	  Anim		male\2hlarge_ready 1
	TEnd

	Type		huge
	  Anim		huge\2hlarge_ready 1
	TEnd

	Type		fem
	  Anim		fem\2hlarge_ready 1
	TEnd
MEnd

### WEAPON SLIDE BACK ###########################################################
#
#Move 2hlarge_Slide_Weapon_Back_Pre
#	Requires	COMBAT WEAPON SLIDING BACKWARD 2HLARGE
#	Priority	15
#	Interrupts	<Ready> <MoveIRQ> <IRQSlide> <BasicSlide>
#	Member		<DeathIRQ> <AttackIRQ> <WepSlide> <GroundMoveALL>
#	Flags		Cycle
#	CycleMove	2hlarge_Slide_Weapon_Back_Cycle
#	NextMove	2hlarge_Slide_Weapon_Post
#
#	Type		male
#	  Anim		male\wep_iceslide_backward_2hlarge 15 31
#	TEnd
#
#	Type		huge
#	  Anim		huge\wep_iceslide_backward_2hlarge 15 31
#	TEnd
#
#	Type		fem
#	  Anim		fem\wep_iceslide_backward_2hlarge 15 31
#	TEnd
#MEnd
#
#Move 2hlarge_Slide_Weapon_Back_Cycle
#	Requires	COMBAT WEAPON SLIDING BACKWARD
#	Member		<DeathIRQ> <AttackIRQ> <WepSlide> <GroundMoveALL>
#	Flags		Cycle
#	NextMove	2hlarge_Slide_Weapon_Post
#
#	Type		male
#	  Anim		male\wep_iceslide_backward_2hlarge 1 31
#	TEnd
#
#	Type		huge
#	  Anim		huge\wep_iceslide_backward_2hlarge 1 31
#	TEnd
#
#	Type		fem
#	  Anim		fem\wep_iceslide_backward_2hlarge 1 31
#	TEnd
#MEnd
#
### WEAPON SLIDE RIGHT ###########################################################
#
#Move 2hlarge_Slide_Weapon_Right_Pre
#	Requires	STEPRIGHT COMBAT WEAPON	SLIDING 2HLARGE
#	Priority	15
#	Interrupts	<Ready> <MoveIRQ> <IRQSlide> <BasicSlide>
#	Member		<DeathIRQ> <AttackIRQ> <WepSlide> <GroundMoveALL>
#	Flags		Cycle
#	CycleMove	2hlarge_Slide_Weapon_Right_Cycle
#	NextMove	2hlarge_Slide_Weapon_Post
#
#	Type		male
#	  Anim		male\wep_iceslide_right_2hlarge	15 31
#	TEnd
#
#	Type		huge
#	  Anim		huge\wep_iceslide_right_2hlarge	15 31
#	TEnd
#
#	Type		fem
#	  Anim		fem\wep_iceslide_right_2hlarge	15 31
#	TEnd
#
#MEnd
#
#Move 2hlarge_Slide_Weapon_Right_Cycle
#	Requires	STEPRIGHT COMBAT WEAPON	SLIDING
#	Priority	15
#	Member		<DeathIRQ> <AttackIRQ> <WepSlide> <GroundMoveALL>
#	Flags		Cycle
#	NextMove	2hlarge_Slide_Weapon_Post
#
#	Type		male
#	  Anim		male\wep_iceslide_right_2hlarge	1 31
#	TEnd
#
#	Type		huge
#	  Anim		huge\wep_iceslide_right_2hlarge	1 31
#	TEnd
#
#	Type		fem
#	  Anim		fem\wep_iceslide_right_2hlarge	1 31
#	TEnd
#MEnd
#
### WEAPON SLIDE LEFT ###########################################################
#
#Move 2hlarge_Slide_Weapon_Left_Pre
#	Requires	STEPLEFT COMBAT	WEAPON SLIDING 2HLARGE
#	Priority	15
#	Interrupts	<Ready> <MoveIRQ> <IRQSlide> <BasicSlide>
#	Member		<DeathIRQ> <AttackIRQ> <WepSlide> <GroundMoveALL>
#	Flags		Cycle
#	CycleMove	2hlarge_Slide_Weapon_Left_Cycle
#	NextMove	2hlarge_Slide_Weapon_Post
#
#	Type		male
#	  Anim		male\wep_iceslide_left_2hlarge 15 31
#	TEnd
#
#	Type		huge
#	  Anim		huge\wep_iceslide_left_2hlarge 15 31
#	TEnd
#
#	Type		fem
#	  Anim		fem\wep_iceslide_left_2hlarge 15 31
#	TEnd
#MEnd
#
#Move 2hlarge_Slide_Weapon_Left_Cycle
#	Requires	STEPLEFT COMBAT	WEAPON SLIDING #CLUB
#	Priority	15
#	Member		<DeathIRQ> <AttackIRQ> <WepSlide> <GroundMoveALL>
#	Flags		Cycle
#	NextMove	2hlarge_Slide_Weapon_Post
#
#	Type		male
#	  Anim		male\wep_iceslide_left_2hlarge 1 31
#	TEnd
#
#	Type		huge
#	  Anim		huge\wep_iceslide_left_2hlarge 1 31
#	TEnd
#
#	Type		fem
#	  Anim		fem\wep_iceslide_left_2hlarge 1 31
#	TEnd
#MEnd