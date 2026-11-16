#########################################################
## CRABPACK.INC - Sequencers designed to work in conjunction with Crabpack Backpack
#########################################################

## CRABPACK - GRENADE ###########################################################

Move CrabPack_Grenade
	Requires	COMBAT CRABPACKGRENADE
	Priority	2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	CrabPack_Grenade_Post
	Sets		CANTMOVE
	SticksOnChild	COMBAT CRABPACKGRENADE

	Type		male
	  Anim          male\CrabPack_WebGrenade 1 50
	TEnd
MEnd

Move CrabPack_Grenade_Post
	Requires	COMBAT
	Priority	1
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <BlockIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Lazy_CombatReady

	Type		male
	  Anim          male\CrabPack_WebGrenade 51
	TEnd
MEnd

Move A_CrabPack_Grenade
	Requires	FLY COMBAT CRABPACKGRENADE
	Priority	3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_CrabPack_Grenade_Post
	Sets		CANTMOVE
	SticksOnChild	COMBAT CRABPACKGRENADE

	Type		male
	  Anim          male\air_CrabPack_WebGrenade 1 50
	TEnd
MEnd

Move A_CrabPack_Grenade_Post
	Requires	FLY COMBAT
	Priority	2
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <BlockIRQ> <AttackIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Lazy_C_flycycle

	Type		male
	  Anim          male\air_CrabPack_WebGrenade 51
	TEnd
MEnd


## CRAB MELEE - RIGHT ###########################################################

Move CrabPack_Melee_Right
	Requires	COMBAT CRABPACKMELE
	Priority	2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	CrabPack_Melee_Right_Post
	Sets		CANTMOVE
	SticksOnChild   COMBAT CRABPACKMELE RIGHT

	Type		male
	  Anim          male\CrabPack_MeleRight 1 25
	TEnd

	Type		LordRecluse
	  Anim          male\Recluse_GutPunch 1 25
	TEnd
MEnd

Move CrabPack_Melee_Right_Post
	Requires	COMBAT
	Priority	1
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <BlockIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Lazy_CombatReady

	Type		male
	  Anim          male\CrabPack_MeleRight 26
	TEnd

	Type		LordRecluse
	  Anim          male\Recluse_GutPunch 26
	TEnd
MEnd

Move A_CrabPack_Melee_Right
	Requires	FLY COMBAT CRABPACKMELE
	Priority	3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_CrabPack_Melee_Right_Post
	Sets		CANTMOVE
	SticksOnChild   COMBAT CRABPACKMELE RIGHT

	Type		male
	  Anim          male\air_CrabPack_MeleRight 1 25
	TEnd

	Type		LordRecluse
	  Anim          male\Recluse_GutPunch 1 25
	TEnd
MEnd

Move A_CrabPack_Melee_Right_Post
	Requires	FLY COMBAT
	Priority	2
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <BlockIRQ> <AttackIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Lazy_C_flycycle

	Type		male
	  Anim          male\air_CrabPack_MeleRight 26
	TEnd

	Type		LordRecluse
	  Anim          male\Recluse_GutPunch 26
	TEnd
MEnd


## CRAB MELEE - LEFT ###########################################################

Move CrabPack_Melee_Left
	Requires	COMBAT CRABPACKMELE
	Priority	2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	CrabPack_Melee_Left_Post
	Sets		CANTMOVE
	SticksOnChild   COMBAT CRABPACKMELE LEFT

	Type		male
	  Anim          male\CrabPack_MeleLeft 1 23
	TEnd

	Type		LordRecluse
	  Anim          male\Recluse_GutPunch 1 25
	TEnd
MEnd

Move CrabPack_Melee_Left_Post
	Requires	COMBAT
	Priority	1
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <BlockIRQ>
	NextMove	Lazy_CombatReady

	Type		male
	  Anim          male\CrabPack_MeleLeft 24
	TEnd

	Type		LordRecluse
	  Anim          male\Recluse_GutPunch 26
	TEnd
MEnd

Move A_CrabPack_Melee_Left
	Requires	FLY COMBAT CRABPACKMELE
	Priority	3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_CrabPack_Melee_Left_Post
	Sets		CANTMOVE
	SticksOnChild   COMBAT CRABPACKMELE LEFT

	Type		male
	  Anim          male\air_CrabPack_MeleLeft 1 23
	TEnd

	Type		LordRecluse
	  Anim          male\Recluse_GutPunch 1 25
	TEnd
MEnd

Move A_CrabPack_Melee_Left_Post
	Requires	FLY COMBAT
	Priority	2
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <BlockIRQ> <AttackIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Lazy_C_flycycle

	Type		male
	  Anim          male\air_CrabPack_MeleLeft 24
	TEnd

	Type		LordRecluse
	  Anim          male\Recluse_GutPunch 26
	TEnd
MEnd


## MELEE BURST ###########################################################

Move CrabPack_MeleeBurst
	Requires	COMBAT CRABPACKMELEBURST
	Priority	2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	CrabPack_MeleeBurst_Post
	Sets		CANTMOVE

	Type		male
	  Anim          male\CrabPack_MeleBurst 1 50
	TEnd
MEnd

Move CrabPack_MeleeBurst_Post
	Requires	COMBAT
	Priority	1
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <BlockIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Lazy_CombatReady

	Type		male
	  Anim          male\CrabPack_MeleBurst 51
	TEnd
MEnd

Move A_CrabPack_MeleeBurst
	Requires	FLY COMBAT CRABPACKMELEBURST
	Priority	3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_CrabPack_MeleeBurst_Post
	Sets		CANTMOVE

	Type		male
	  Anim          male\air_CrabPack_MeleBurst 1 50
	TEnd
MEnd

Move A_CrabPack_MeleeBurst_Post
	Requires	FLY COMBAT
	Priority	2
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <BlockIRQ> <AttackIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Lazy_C_flycycle

	Type		male
	  Anim          male\air_CrabPack_MeleBurst 51
	TEnd
MEnd

## RECLUSE SLICE ###########################################################

Move Recluse_Slice
	Requires	COMBAT RECLUSESLICE
	Priority	2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Recluse_Slice_Post
	Sets		CANTMOVE
	SticksOnChild   COMBAT RECLUSESLICE LEFT

	Type		male
	  Anim          male\CrabPack_MeleLeft 1 23
	TEnd
MEnd

Move Recluse_Slice_Post
	Requires	COMBAT
	Priority	1
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <BlockIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Lazy_CombatReady

	Type		male
	  Anim          male\CrabPack_MeleLeft 24
	TEnd
MEnd

Move A_Recluse_Slice
	Requires	FLY COMBAT RECLUSESLICE
	Priority	3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Recluse_Slice_Post
	Sets		CANTMOVE
	SticksOnChild   COMBAT RECLUSESLICE LEFT

	Type		male
	  Anim          male\air_CrabPack_MeleLeft 1 23
	TEnd
MEnd

Move A_Recluse_Slice_Post
	Requires	FLY COMBAT
	Priority	2
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <BlockIRQ> <AttackIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Lazy_C_flycycle

	Type		male
	  Anim          male\air_CrabPack_MeleLeft 24
	TEnd
MEnd


## RECLUSE KNOCKUP ###########################################################

Move Recluse_KnockUp
	Requires	COMBAT RECLUSEKNOCKUP
	Priority	2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Recluse_KnockUp_Post
	Sets		CANTMOVE
	SticksOnChild   COMBAT RECLUSEKNOCKUP RIGHT

	Type		male
	  Anim          male\Recluse_KnockUp 1 40
	TEnd
MEnd

Move Recluse_KnockUp_Post
	Requires	COMBAT
	Priority	1
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <BlockIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Lazy_CombatReady

	Type		male
	  Anim          male\Recluse_KnockUp 41
	TEnd
MEnd

Move A_Recluse_KnockUp
	Requires	FLY COMBAT RECLUSEKNOCKUP
	Priority	3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Recluse_KnockUp_Post
	Sets		CANTMOVE
	SticksOnChild   COMBAT RECLUSEKNOCKUP RIGHT

	Type		male
	  Anim          male\Recluse_KnockUp 1 40
	TEnd
MEnd

Move A_Recluse_KnockUp_Post
	Requires	FLY COMBAT
	Priority	2
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <BlockIRQ> <AttackIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Lazy_C_flycycle

	Type		male
	  Anim          male\Recluse_KnockUp 41
	TEnd
MEnd


## PBAOE ###########################################################

Move Recluse_MeleePBAoE
	Requires	COMBAT RECLUSEPBAOE
	Priority	2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Recluse_MeleePBAoE_Post
	Sets		CANTMOVE
	SticksOnChild	COMBAT RECLUSEPBAOE

	Type		male
	  Anim          male\Recluse_MelePBAoE 1 60
	TEnd
MEnd

Move Recluse_MeleePBAoE_Post
	Requires	COMBAT
	Priority	1
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <BlockIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Lazy_CombatReady

	Type		male
	  Anim          male\Recluse_MelePBAoE 61
	TEnd
MEnd

Move A_Recluse_MeleePBAoE
	Requires	FLY COMBAT RECLUSEPBAOE
	Priority	3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Recluse_MeleePBAoE_Post
	Sets		CANTMOVE
	SticksOnChild	COMBAT RECLUSEPBAOE

	Type		male
	  Anim          male\air_Recluse_MelePBAoE 1 60
	TEnd
MEnd

Move A_Recluse_MeleePBAoE_Post
	Requires	FLY COMBAT
	Priority	2
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <BlockIRQ> <AttackIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Lazy_C_flycycle

	Type		male
	  Anim          male\air_Recluse_MelePBAoE 61
	TEnd
MEnd


##############################################
###### CrabPack Pulse Fire Right Single ######
##############################################

Move CrabPack_FireRight
	Requires	COMBAT CRABPACKRANGED
	Priority	2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	CrabPack_FireRight_Post
	Sets		CANTMOVE
	SticksOnChild	COMBAT CRABPACKRANGED

	Type		male
	  Anim          male\CrabPack_FireRight 1 40
	TEnd
MEnd

Move CrabPack_FireRight_Post
	Requires	COMBAT
	Priority	1
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <BlockIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Lazy_COMBATReady

	Type		male
	  Anim          male\CrabPack_FireRight 41
	TEnd
MEnd

Move A_CrabPack_FireRight
	Requires	FLY COMBAT CRABPACKRANGED
	Priority	3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_CrabPack_FireRight_Post
	Sets		CANTMOVE
	SticksOnChild	COMBAT CRABPACKRANGED

	Type		male
	  Anim          male\air_CrabPack_FireRight 1 40
	TEnd
MEnd

Move A_CrabPack_FireRight_Post
	Requires	FLY COMBAT
	Priority	2
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <BlockIRQ> <AttackIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Lazy_C_flycycle

	Type		male
	  Anim          male\air_CrabPack_FireRight 41
	TEnd
MEnd


##############################################
###### CrabPack Spray Fire Right Single ######
##############################################

Move CrabPack_SprayRight
	Requires	COMBAT CRABPACKRANGEDSPRAY
	Priority	2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	CrabPack_SprayRight_Post
	Sets		CANTMOVE
	SticksOnChild	COMBAT CRABPACKRANGEDSPRAY

	Type		male
	  Anim          male\CrabPack_SprayRight 1 60
	TEnd
MEnd

Move CrabPack_SprayRight_Post
	Requires	COMBAT
	Priority	1
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <BlockIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Lazy_COMBATReady

	Type		male
	  Anim          male\CrabPack_SprayRight 61
	TEnd
MEnd

Move A_CrabPack_SprayRight
	Requires	FLY COMBAT CRABPACKRANGEDSPRAY
	Priority	3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_CrabPack_SprayRight_Post
	Sets		CANTMOVE
	SticksOnChild	COMBAT CRABPACKRANGEDSPRAY

	Type		male
	  Anim          male\air_CrabPack_SprayRight 1 60
	TEnd
MEnd

Move A_CrabPack_SprayRight_Post
	Requires	FLY COMBAT
	Priority	2
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <BlockIRQ> <AttackIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Lazy_C_flycycle

	Type		male
	  Anim          male\air_CrabPack_SprayRight 61
	TEnd
MEnd


####################################
###### CrabPack Ranged Burst  ######
####################################

Move CrabPack_RangeBurst
	Requires	COMBAT CRABPACKRANGEDBURST
	Priority	2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	CrabPack_RangeBurst_Post
	Sets		CANTMOVE
	SticksOnChild	COMBAT CRABPACKRANGEDBURST

	Type		male
	  Anim          male\CrabPack_RangedBurst 1 90
	TEnd

	Type		LordRecluse
	  Anim          male\Recluse_RangedCone 1 60
	TEnd
MEnd

Move CrabPack_RangeBurst_Post
	Requires	COMBAT
	Priority	1
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <BlockIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Lazy_COMBATReady

	Type		male
	  Anim          male\CrabPack_RangedBurst 91
	TEnd

	Type		LordRecluse
	  Anim          male\Recluse_RangedCone 61
	TEnd
MEnd

Move A_CrabPack_RangeBurst
	Requires	FLY COMBAT CRABPACKRANGEDBURST
	Priority	3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_CrabPack_RangeBurst_Post
	Sets		CANTMOVE
	SticksOnChild	COMBAT CRABPACKRANGEDBURST

	Type		male
	  Anim          male\air_CrabPack_RangedBurst 1 90
	TEnd

	Type		LordRecluse
	  Anim          male\Recluse_RangedCone 1 60
	TEnd
MEnd

Move A_CrabPack_RangeBurst_Post
	Requires	FLY COMBAT
	Priority	2
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <BlockIRQ> <AttackIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Lazy_C_flycycle

	Type		male
	  Anim          male\air_CrabPack_RangedBurst 91
	TEnd

	Type		LordRecluse
	  Anim          male\Recluse_RangedCone 61
	TEnd
MEnd

#######################################
###### Arachnoid Spin COCOON POWER ####
#######################################

Move SpinCocoon_Power
	Requires	COMBAT SPINCOCOON
	Priority	3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Lazy_COMBAT_Blend
	Flags		CANTMOVE PitchToTarget
	SticksOnChild	COMBAT SPINCOCOON

	Type		male
	  Anim		male\Arachnoid_SpinCocoonPower 1 100
	TEnd
MEnd

Move A_SpinCocoon_Power
	Requires	FLY SPINCOCOON COMBAT
	Priority	4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Lazy_COMBAT_Blend
	Flags		CANTMOVE PitchToTarget
	SticksOnChild	COMBAT SPINCOCOON

	Type		male
	  Anim		male\Arachnoid_SpinCocoonPower 1 100
	TEnd
MEnd