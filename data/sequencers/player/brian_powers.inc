#############################################################
## Brian_powers.inc
#############################################################

## PARAMILITARY TOSS SERUM ###########################################################

Move PM_TossSerum
	Requires	COMBAT BOOSTUP SERUM LAUNCH
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        PM_TossSerum02
	Flags		FixArmReg CANTMOVE

	Type		male
	  Anim          male\Paramilitary_TossBoost 1 45
	TEnd

	Type		huge
	  Anim          huge\Paramilitary_TossBoost 1 45
	TEnd
MEnd

Move PM_TossSerum02
	Requires	COMBAT
	Priority        1
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <BlockIRQ>
	NextMove        Lazy_CombatReady

	Type		male
	  Anim          male\Paramilitary_TossBoost 46
	TEnd

	Type		huge
	  Anim          huge\Paramilitary_TossBoost 46
	TEnd
MEnd


#############################################################

Move Air_PM_TossSerum
	Requires	FLY COMBAT BOOSTUP SERUM LAUNCH
	Priority        15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Air_PM_TossSerum02
	Flags		CANTMOVE FixArmReg

	Type		male
	  Anim          male\Air_Paramilitary_Toss 1 45
	TEnd

	Type		huge
	  Anim          huge\Air_Paramilitary_Toss 1 45
	TEnd
MEnd

Move Air_PM_TossSerum02
	Requires	FLY COMBAT
	Priority        12
	Include sequencers\Members\FlyAttacks_PostMoves.inc
	NextMove        Lazy_C_flycycle

	Type		male
	  Anim          male\Air_Paramilitary_Toss 46
	TEnd

	Type		huge
	  Anim          huge\Air_Paramilitary_Toss 46
	TEnd
MEnd

## PARAMILITARY SUMMON SOLDIER ###########################################################

Move PM_SummonSoldier1
	Requires	COMBAT WALKIETALKIE
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        PM_SummonSoldier2
	Flags		CANTMOVE FixArmReg NoSizeScale

	Type		male
	  Anim          male\Paramilitary_SummonSoldiers 1 9
	TEnd

	Type		fem
	  Anim          male\Paramilitary_SummonSoldiers 1 9
	TEnd

	Type		huge
	  Anim          huge\Paramilitary_SummonSoldiers 1 9
	TEnd
MEnd

Move PM_SummonSoldier2
	Requires	COMBAT
	Priority        1
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <BlockIRQ>
	NextMove        PM_SummonSoldier3
	Flags		CANTMOVE FixArmReg NoSizeScale

	Type		male
	  Anim          male\Paramilitary_SummonSoldiers 9 120
	  PlayFX	V_COV\Powers\Paramilitary\Paramilitary_SummonRadio.fx 0 Constant
	TEnd

	Type		fem
	  Anim          male\Paramilitary_SummonSoldiers 9 120
	  PlayFX	V_COV\Powers\Paramilitary\Paramilitary_SummonRadioFEM.fx 0 Constant
	TEnd

	Type		huge
	  Anim          huge\Paramilitary_SummonSoldiers 9 120
	  PlayFX	V_COV\Powers\Paramilitary\Paramilitary_SummonRadioHUGE.fx 0 Constant
	TEnd
MEnd

Move PM_SummonSoldier3
	Requires	COMBAT
	Priority        1
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <BlockIRQ>
	NextMove        PM_SummonSoldier4
	Flags		FixArmReg NoSizeScale

	Type		male
	  Anim          male\Paramilitary_SummonSoldiers 120 180
	  PlayFX	V_COV\Powers\Paramilitary\Paramilitary_SummonRadio.fx 0 Constant
	TEnd

	Type		fem
	  Anim          male\Paramilitary_SummonSoldiers 120 180
	  PlayFX	V_COV\Powers\Paramilitary\Paramilitary_SummonRadioFEM.fx 0 Constant
	TEnd

	Type		huge
	  Anim          huge\Paramilitary_SummonSoldiers 120 180
	  PlayFX	V_COV\Powers\Paramilitary\Paramilitary_SummonRadioHUGE.fx 0 Constant
	TEnd
MEnd

Move PM_SummonSoldier4
	Requires	COMBAT
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <BlockIRQ>
	NextMove        Lazy_CombatReady
	Priority        1

	Type		male
	  Anim          male\Paramilitary_SummonSoldiers 180
	TEnd

	Type		fem
	  Anim          male\Paramilitary_SummonSoldiers 180
	TEnd

	Type		huge
	  Anim          huge\Paramilitary_SummonSoldiers 180
	TEnd
MEnd


#############################################################

Move Air_PM_SummonSoldier1
	Requires	Fly COMBAT WALKIETALKIE
	Priority        13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Air_PM_SummonSoldier2
	Flags		CANTMOVE FixArmReg NoSizeScale

	Type		male
	  Anim          male\Air_Paramilitary_Summon 1 9
	TEnd

	Type		fem
	  Anim          male\Air_Paramilitary_Summon 1 9
	TEnd

	Type		huge
	  Anim          huge\Air_Paramilitary_Summon 1 9
	TEnd
MEnd

Move Air_PM_SummonSoldier2
	Requires	FLY COMBAT
	Priority        12
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Air_PM_SummonSoldier3
	Flags		CANTMOVE FixArmReg NoSizeScale

	Type		male
	  Anim          male\Air_Paramilitary_Summon 9 120
	  PlayFX	V_COV\Powers\Paramilitary\Paramilitary_SummonRadio.fx 0 Constant
	TEnd

	Type		fem
	  Anim          male\Air_Paramilitary_Summon 9 120
	  PlayFX	V_COV\Powers\Paramilitary\Paramilitary_SummonRadioFEM.fx 0 Constant
	TEnd

	Type		huge
	  Anim          huge\Air_Paramilitary_Summon 9 120
	  PlayFX	V_COV\Powers\Paramilitary\Paramilitary_SummonRadioHUGE.fx 0 Constant
	TEnd
MEnd

Move Air_PM_SummonSoldier3
	Requires	FLY COMBAT
	Priority        12
	Include sequencers\Members\FlyAttacks_PostMoves.inc
	NextMove        Air_PM_SummonSoldier4
	Flags		FixArmReg NoSizeScale

	Type		male
	  Anim          male\Air_Paramilitary_Summon 120 180
	  PlayFX	V_COV\Powers\Paramilitary\Paramilitary_SummonRadio.fx 0 Constant
	TEnd

	Type		fem
	  Anim          male\Air_Paramilitary_Summon 120 180
	  PlayFX	V_COV\Powers\Paramilitary\Paramilitary_SummonRadioFEM.fx 0 Constant
	TEnd

	Type		huge
	  Anim          huge\Air_Paramilitary_Summon 120 180
	  PlayFX	V_COV\Powers\Paramilitary\Paramilitary_SummonRadioHUGE.fx 0 Constant
	TEnd
MEnd

Move Air_PM_SummonSoldier4
	Requires	FLY COMBAT
	Priority        12
	Include sequencers\Members\FlyAttacks_PostMoves.inc
	NextMove        C_flycycle

	Type		male
	  Anim          male\Air_Paramilitary_Summon 180
	TEnd

	Type		fem
	  Anim          male\Air_Paramilitary_Summon 180
	TEnd

	Type		huge
	  Anim          huge\Air_Paramilitary_Summon 180
	TEnd
MEnd


## IR GOGGLES ACTIVATE #################################################################################################

Move IRGoggl_Activate
	Requires	IRGOGGLE
	Priority        1
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        IRGoggl_Activate_post
	Flags		CANTMOVE FixArmReg
	Scale		1.2

	Type		male
	  Anim          male\GoggleActivate 1 90
	TEnd

	Type		huge
	  Anim          huge\GoggleActivate 1 90
	TEnd

	Type		fem
	  Anim          fem\GoggleActivate 1 90
	TEnd
MEnd

Move IRGoggl_Activate_post
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <BlockIRQ>
	NextMove        IRGoggl_Activate_cycle

	Type		male
	  Anim          male\GoggleActivate 91
	TEnd

	Type		huge
	  Anim          huge\GoggleActivate 91
	TEnd
	Type		fem
	  Anim          fem\GoggleActivate 91
	TEnd
MEnd

Move IRGoggl_Activate_cycle
	Member		<Ready> <ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ>
	Flags		Cycle

	Type		male
	  Anim          male\Skel_ready2 1
	TEnd

	Type		huge
	  Anim          huge\Skel_ready 1
	TEnd

	Type		fem
	  Anim          fem\ready2 1
	TEnd
MEnd


#############################################################

Move A_IRGoggl_Activate
	Requires	FLY IRGOGGLE
	Priority        12
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_IRGoggl_Activate_Post
	Flags		CANTMOVE FixArmReg
	Scale		1.2

	Type		male
	  Anim          male\Air_GoggleActivate 1 90
	TEnd

	Type		huge
	  Anim          huge\Air_GoggleActivate 1 90
	TEnd

	Type		fem
	  Anim          fem\Air_GoggleActivate 1 90
	TEnd
MEnd

Move A_IRGoggl_Activate_Post
	Requires	FLY
	Priority        11
	Include sequencers\Members\FlyAttacks_PostMoves.inc
	NextMove        flyready

	Type		male
	  Anim          male\Air_GoggleActivate 91
	TEnd

	Type		huge
	  Anim          huge\Air_GoggleActivate 91
	TEnd
	Type		fem
	  Anim          fem\Air_GoggleActivate 91
	TEnd
MEnd