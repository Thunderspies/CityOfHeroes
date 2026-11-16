#############################################################
## readies_air.inc - Fly mode readys
#############################################################

## FLY READY ###########################################################

Move Preflyready
	Requires	FLY
	Priority	1
	Interrupts	<Ready>	<Jumps>	<PostMoveCycle>
	Include sequencers\Members\Readies_Air.inc
	NextMove	flyready

	Type		male
	  Anim		male\pre_fly 2
	  PlayFx	FEET\FLIGHT2.FX
	TEnd

	Type		huge
	  Anim		huge\pre_fly 2
	  PlayFx	FEET\FLIGHT2.FX
	TEnd

	Type		fem
	  Anim		fem\pre_fly 2
	  PlayFx	FEET\FLIGHT2.FX
	TEnd

	Type		Rularuu_male
	  Anim		male\mystic_float 1
	TEnd

	Type		Wisp
	  Anim		wisp\float 1
	TEnd
MEnd

Move flyready
	Requires	FLY
	Priority	1
	Include sequencers\Members\Readies_Air.inc
	Flags		Cycle
	NextMove	PreFall

	Type		male
	  Anim		male\float2 1
	TEnd

	Type		huge
	  Anim		huge\float 1
	TEnd

	Type		fem
	  Anim		fem\fly_ready 1
	TEnd

	Type		Rularuu_male
	  Anim		male\mystic_float 1
	TEnd

	Type		Wisp
	  Anim		wisp\float 1
	TEnd
MEnd

## FLY FROM FALL ###########################################################

Move Fall_Fready
	Requires	FLY
	Priority	1
	Interrupts	<Fall>
	Include sequencers\Members\Readies_Air.inc
	NextMove	flyready

	Type		male
	  Anim		male\flydown 180
	TEnd

	Type		huge
	  Anim		huge\flydown 180
	TEnd

	Type		fem
	  Anim		fem\fly_down 180
	TEnd

	Type		Wisp
	  Anim		wisp\float 1
	TEnd
MEnd


#############################################################
## COMBAT MODE
#############################################################

## FLY TO COMBAT FLY TRANSITION ###########################################################

Move air_T_airCombat
	Requires	FLY COMBAT
	Priority	2
	Interrupts	<FlyReady>
	Include sequencers\Members\Readies_Air.inc
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <BlockIRQ> <FlyCombat> <AttackIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove	C_flyready

	Type		male
	  Anim		male\air_combatready 2 24
	TEnd

	Type		huge
	  Anim		huge\air_combatready 2 24
	TEnd

	Type		fem
	  Anim		fem\air_combatready 2 24
	TEnd

	Type		Rularuu_male
	  Anim		male\mystic_float 1
	TEnd

	Type		Wisp
	  Anim		wisp\float 1
	TEnd
MEnd

## COMBAT FALL TO COMBAT FLY ###########################################################

Move C_Fall_Fready
	Requires	FLY COMBAT
	Priority	2
	Interrupts	C_FallCycle C_PreFall C_Ffly_PreFall C_Ffly_PreFall_2
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <BlockIRQ> <FlyCombat> <AttackIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove	C_flyready

	Type		male
	  Anim		male\flydown 180
	TEnd

	Type		huge
	  Anim		huge\flydown 180
	TEnd

	Type		fem
	  Anim		fem\fly_down 180
	TEnd

	Type		Rularuu_male
	  Anim		male\mystic_float 1
	TEnd

	Type		Wisp
	  Anim		wisp\float 1
	TEnd
MEnd

## COMBAT FLY READY ###########################################################

Move C_Preflyready
	Requires	FLY COMBAT
	Priority	2
	Interrupts	<Combat> <CombatMove> <Jumps> <Claw> <CombatAttack> Preflyready
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <BlockIRQ> <FlyCombat> <AttackIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove	C_flyready

	Type		male
	  Anim		male\pre_aircombat 2
	  PlayFx	FEET\FLIGHT2.FX
	TEnd

	Type		huge
	  Anim		huge\pre_aircombat 2
	  PlayFx	FEET\FLIGHT2.FX
	TEnd

	Type		fem
	  Anim		fem\pre_aircombat 2
	TEnd

	Type		Rularuu_male
	  Anim		male\mystic_float 1
	TEnd

	Type		Wisp
	  Anim		wisp\float 1
	TEnd
MEnd

Move C_flyready
	Requires	FLY COMBAT
	Priority	2
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <BlockIRQ> <FlyCombat> <AttackIRQ> <MoveIRQ> <IRQEmotesCombat>
	Flags		Cycle
	NextMove	C_PreFall

	Type		male
	  Anim		male\air_combatready 25	65
	TEnd

	Type		huge
	  Anim		huge\air_combatready 25	65
	TEnd

	Type		fem
	  Anim		fem\air_combatready 25 65
	TEnd

	Type		Rularuu_male
	  Anim		male\mystic_float 1
	TEnd

	Type		Wisp
	  Anim		wisp\float 1
	TEnd
MEnd

## COMBAT FLYCYCLE ###########################################################

Move C_flycycle
	Requires	FLY COMBAT
	Priority	2
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <BlockIRQ> <FlyCombat> <AttackIRQ> <MoveIRQ> <IRQEmotesCombat> #flyready was trumping these combat cycles so i ocmmented these out
	Include sequencers\Members\Combat_Air.inc
	Flags		Cycle
	NextMove	flyready
	Scale		0.50

	Type		male
	  Anim		male\air_combat_cycle 1	31
	TEnd

	Type		huge
	  Anim		huge\air_combat_cycle 1	31
	TEnd

	Type		fem
	  Anim		fem\air_combat_cycle 1 31
	TEnd

	Type		Rularuu_male
	  Anim		male\mystic_float 1
	TEnd

	Type		Wisp
	  Anim		wisp\Float 1
	TEnd
MEnd

Move C_flycycle_half
	Requires	FLY COMBAT
	Priority	2
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <BlockIRQ> <FlyCombat> <AttackIRQ> <MoveIRQ> <IRQEmotesCombat> #flyready was trumping these combat cycles so i ocmmented these out
	Include sequencers\Members\Combat_Air.inc
	NextMove	C_flycycle
	Scale		0.50

	Type		male
	  Anim		male\air_combat_cycle 16 31
	TEnd

	Type		huge
	  Anim		huge\air_combat_cycle 16 31
	TEnd

	Type		fem
	  Anim		fem\air_combat_cycle 16	31
	TEnd

	Type		Rularuu_male
	  Anim		male\mystic_float 1
	TEnd

	Type		Wisp
	  Anim		wisp\float 1
	TEnd
MEnd

## LAZY COMBAT FLY CYCLE ###########################################################

Move Lazy_C_flycycle
	Requires	FLY COMBAT
	Priority	2
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <BlockIRQ> <FlyCombat> <AttackIRQ> <MoveIRQ> <IRQEmotesCombat> #flyready was trumping these combat cycles so i ocmmented these out
	Include sequencers\Members\Combat_Air.inc
	Flags		Cycle
	NextMove	flyready
	Scale		0.50

	Type		male
	  Anim		male\air_combat_cycle 1	31
	TEnd
MEnd

Move Lazy_C_flycycle_half
	Requires	FLY COMBAT
	Priority	2
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <BlockIRQ> <FlyCombat> <AttackIRQ> <MoveIRQ> <IRQEmotesCombat> #flyready was trumping these combat cycles so i ocmmented these out
	Include sequencers\Members\Combat_Air.inc
	NextMove	Lazy_C_flycycle
	Scale		0.50

	Type		male
	  Anim		male\air_combat_cycle 16 31
	TEnd
MEnd

Move Lazy_C_flycycle_Blend
	Requires	FLY COMBAT
	Priority	2
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <BlockIRQ> <FlyCombat> <AttackIRQ> <MoveIRQ> <IRQEmotesCombat> #flyready was trumping these combat cycles so i ocmmented these out
	Include sequencers\Members\Combat_Air.inc
	NextMove	Lazy_C_flycycle
	Interpolate	15
	Scale		0.50

	Type		male
	  Anim		male\air_combat_cycle 1	31
	TEnd
MEnd


#############################################################
## WEAPON FLY MODE
#############################################################

## WEAPON FLY READY ###########################################################

Move W_Fall_Fready
	Requires	FLY COMBAT WEAPON
	Interrupts	W_FallCycle W_PreFall W_Ffly_PreFall W_Ffly_PreFall_2
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <BlockIRQ> <FlyWepCombat> <AttackIRQ> <MoveIRQ> <IRQEmotesRighthand>
	NextMove	W_flyready
	Priority	3

	Type		male
	  Anim		male\wep_flydown 180
	TEnd

	Type		huge
	  Anim		huge\air_wep_flydown 180
	TEnd

	Type		fem
	  Anim		fem\air_wep_flydown 180
	TEnd
MEnd

## FLY TO WEAPON COMBAT FLY ###########################################################

#Move air_T_WepCombat
#	Requires	FLY COMBAT WEAPON
#	Interrupts	C_Preflyready <FlyCombat> <FlyReady>
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <BlockIRQ> <FlyWepCombat> <AttackIRQ> <MoveIRQ> <IRQEmotesRighthand>
#	NextMove	W_flyready
#	Priority	3
#
#	Type		male
#	  Anim		male\air_wepready 2 24
#	TEnd
#
#	Type		huge
#	  Anim		huge\air_wepready 2 24
#	TEnd
#
#	Type		fem
#	  Anim		fem\air_wepready 2 24
#	TEnd
#MEnd

## WEAPON FLY READY ###########################################################

Move W_FlyReady_Pre
	Requires	FLY COMBAT WEAPON
	Priority	3
	Interrupts	<WepCombat> <WepCombatMove> <TwoHandCombat> <TwoHandMove> <Jumps> <WepAttack> Preflyready
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <BlockIRQ> <FlyWepCombat> <AttackIRQ> <MoveIRQ> <IRQEmotesRighthand>
	NextMove	W_FlyReady

	Type		male
	  Anim		male\pre_airwep	2
	TEnd

	Type		huge
	  Anim		huge\air_wep_pre 2
	TEnd

	Type		fem
	  Anim		fem\air_wep_pre	2
	TEnd
MEnd

Move W_FlyReady
	Requires	FLY COMBAT WEAPON
	Priority	3
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <BlockIRQ> <FlyWepCombat> <AttackIRQ> <MoveIRQ> <IRQEmotesRighthand>
	Flags		Cycle
	NextMove	W_PreFall

	Type		male
	  Anim		male\air_wepready 25 65
	TEnd
MEnd

## WEAPON FLY CYCLE ###########################################################

Move W_flycycle
	Requires	FLY COMBAT WEAPON
	Priority	3
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <BlockIRQ> <FlyWepCombat> <AttackIRQ> <MoveIRQ> <IRQEmotesRighthand>
	Flags		Cycle
	NextMove	W_PreFall
	Scale		0.50

	Type		male
	  Anim		male\air_wep_cycle 1 31
	TEnd

	Type		huge
	  Anim		huge\air_wep_cycle 1 31
	TEnd

	Type		fem
	  Anim		fem\air_wep_cycle 1 31
	TEnd
MEnd

Move W_flycycle_half
	Requires	FLY COMBAT WEAPON
	Priority	3
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <BlockIRQ> <FlyWepCombat> <AttackIRQ> <MoveIRQ> <IRQEmotesRighthand>
	NextMove	W_flycycle
	Scale		0.50

	Type		male
	  Anim		male\air_wep_cycle 15 31
	TEnd

	Type		huge
	  Anim		huge\air_wep_cycle 15 31
	TEnd

	Type		fem
	  Anim		fem\air_wep_cycle 15 31
	TEnd
MEnd

Move Lazy_W_flycycle
	Requires	FLY COMBAT WEAPON
	Priority	3
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <BlockIRQ> <FlyWepCombat> <AttackIRQ> <MoveIRQ> <IRQEmotesRighthand>
	Flags		Cycle
	NextMove	W_PreFall
	Scale		0.50

	Type		male
	  Anim		male\air_wep_cycle 1 31
	TEnd
MEnd

Move Lazy_W_flycycle_half
	Requires	FLY COMBAT WEAPON
	Priority	3
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <BlockIRQ> <FlyWepCombat> <AttackIRQ> <MoveIRQ> <IRQEmotesRighthand>
	NextMove	Lazy_W_flycycle
	Scale		0.50

	Type		male
	  Anim		male\air_wep_cycle 15 31
	TEnd
MEnd

Move Lazy_W_flycycle_HalfBlend
	Requires	FLY COMBAT WEAPON
	Priority	3
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <BlockIRQ> <FlyWepCombat> <AttackIRQ> <MoveIRQ> <IRQEmotesRighthand>
	NextMove	Lazy_W_flycycle
	Scale		0.50
	Interpolate	15

	Type		male
	  Anim		male\air_wep_cycle 15 31
	TEnd
MEnd

#############################################################
## DUAL MODE
#############################################################

## FALL TO DUAL FLY ###########################################################

Move DualWield_Fall_Fready
	Requires	FLY COMBAT WEAPON DUAL
	Priority	4
	Interrupts	<Fall> <iceslide>
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <BlockIRQ> <FlyDualHandCombat> <AttackIRQ> <MoveIRQ> <IRQEmotesDualhand>
	NextMove	DualWield_FlyReady

	Type		male
	  Anim		male\wep_flydown 180
	TEnd
MEnd

## FLY TO DUAL COMBAT FLY ###########################################################

#Move Air_To_DualWield_Combat
#	Requires	FLY COMBAT WEAPON DUAL
#	Interrupts	<FlyReady>
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <BlockIRQ> <FlyDualHandCombat> <AttackIRQ> <MoveIRQ> <IRQEmotesDualhand>
#	NextMove	DualWield_FlyReady
#	Priority	4
#
#	Type		male
#	  Anim		male\air_dual_ready 2 24
#	TEnd
#MEnd

## DUAL FLY READY ###########################################################

Move DualWield_Preflyready
	Requires	FLY COMBAT WEAPON DUAL
	Priority	4
	Interrupts	<WepCombat> <WepCombatMove> <TwoHandCombat> <TwoHandMove> <Jumps> <WepAttack> Preflyready <iceslide>
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <BlockIRQ> <FlyDualHandCombat> <AttackIRQ> <MoveIRQ> <IRQEmotesDualhand>
	NextMove	DualWield_FlyReady

	Type		male
	  Anim		male\air_dual_pre 2
	TEnd
MEnd

Move DualWield_FlyReady
	Requires	FLY COMBAT WEAPON DUAL
	Priority	4
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <BlockIRQ> <FlyDualHandCombat> <AttackIRQ> <MoveIRQ> <IRQEmotesDualhand>
	Flags		Cycle
	NextMove	DualWield_PreFall

	Type		male
	  Anim		male\air_dual_ready 25 65
	TEnd
MEnd

## DUAL FLY CYCLE ###########################################################

Move DualWield_FlyCycle
	Requires	FLY COMBAT WEAPON DUAL
	Priority	4
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <BlockIRQ> <FlyDualHandCombat> <AttackIRQ> <MoveIRQ> <IRQEmotesDualhand>
	Flags		Cycle
	NextMove	DualWield_PreFall
	Scale		0.50

	Type		male
	  Anim		male\air_dual_cycle 1 31
	TEnd
MEnd

Move DualWield_FlyCycle_half
	Requires	FLY COMBAT WEAPON DUAL
	Priority	4
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <BlockIRQ> <FlyDualHandCombat> <AttackIRQ> <MoveIRQ> <IRQEmotesDualhand>
	NextMove	DualWield_FlyCycle
	Scale		0.50

	Type		male
	  Anim		male\air_dual_cycle 15 31
	TEnd
MEnd

## COMBAT/WEAPON/DUAL TO FLY READY TRANSITION ###########################################################

Move Combat2FlyReady
	Requires	FLY
	Priority	1
	Interrupts	C_PreFall W_PreFall DualWield_PreFall
	Include sequencers\Members\Readies_Air.inc
	NextMove	flyready

	Type		male
	  Anim		male\prefloat_combat 5
	TEnd

	Type		huge
	  Anim		huge\prefloat_combat 5
	TEnd

	Type		fem
	  Anim		fem\prefloat_combat 5
	TEnd

	Type		Rularuu_male
	  Anim		male\mystic_float 1
	TEnd

	Type		Wisp
	  Anim		wisp\Float 1
	TEnd
MEnd