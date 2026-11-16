#############################################################
## blocks.inc - Sequencers for Dodges\Blocks
#############################################################

## BLOCK ###########################################################

Move Block_R
	Requires	BLOCK
	Interrupts	<BlockIRQ> Block_L
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <Block>
	NextMove        Block_cycle
	Flags		PickRandomly
	Priority        1

	Type		male
	  Anim          male\combat_block 1 50
	TEnd

	Type		huge
	  Anim          huge\combat_block 1 50
	TEnd

	Type		fem
	  Anim          fem\combat_block 1 50
	TEnd
MEnd

Move Block_L
	Requires	BLOCK
	Priority        1
	Interrupts	<BlockIRQ> Block_R
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <Block>
	NextMove        Fake_CombatCycle

	Type		male
	  Anim          male\combat_block 53
	TEnd

	Type		huge
	  Anim          huge\combat_block 53
	TEnd

	Type		fem
	  Anim          fem\combat_block 53
	TEnd
MEnd

#############################################################

Move A_Block_R
	Requires	FLY BLOCK
	Interrupts	<BlockIRQ>
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <Block> <AttackIRQ>
	NextMove        C_flycycle
	Priority        2

	Type		male
	  Anim          male\air_combat_block 1 69
	TEnd

	Type		huge
	  Anim          huge\air_combat_block 1 69
	TEnd

	Type		fem
	  Anim          fem\air_combat_block 1 69
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_float 1
	TEnd
MEnd

## COMBAT BLOCK ###########################################################

Move CombatBlock_R
	Requires	COMBAT BLOCK
	Interrupts	<BlockIRQ> C_Dodge_L A_CombatBlock_R
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <Block> <AttackIRQ> <BlockIRQ>
	NextMove        CombatReady
	Flags		PickRandomly
	Priority        2

	Type		male
	  Anim          male\combat_block 1 50
	TEnd

	Type		huge
	  Anim          huge\combat_block 1 50
	TEnd

	Type		fem
	  Anim          fem\combat_block 1 50
	TEnd
MEnd

Move A_CombatBlock_R
	Requires	FLY COMBAT BLOCK
	Interrupts	<BlockIRQ> CombatBlock_R
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <Block> <AttackIRQ>
	NextMove        C_flycycle
	Priority        3

	Type		male
	  Anim          male\air_combat_block 1 69
	TEnd

	Type		huge
	  Anim          huge\air_combat_block 1 69
	TEnd

	Type		fem
	  Anim          fem\air_combat_block 1 69
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_float 1
	TEnd
MEnd

Move CombatBlock_L
	Requires	COMBAT BLOCK
	Interrupts	<BlockIRQ> C_Dodge_R
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <Block> <AttackIRQ> <BlockIRQ>
	NextMove        CombatReady
	Priority        2

	Type		male
	  Anim          male\combat_block 53
	TEnd

	Type		huge
	  Anim          huge\combat_block 53
	TEnd

	Type		fem
	  Anim          fem\combat_block 53
	TEnd
MEnd

Move A_CombatBlock_L
	Requires	FLY COMBAT BLOCK
	Interrupts	<BlockIRQ> A_CombatBlock_R
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <Block> <AttackIRQ>
	NextMove        C_flycycle
	Priority        3

	Type		male
	  Anim          male\air_combat_block 72
	TEnd

	Type		huge
	  Anim          huge\air_combat_block 72
	TEnd

	Type		fem
	  Anim          fem\air_combat_block 72
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_float 1
	TEnd
MEnd


## COMBAT DODGE ###########################################################

Move C_Dodge_R
	Requires	COMBAT BLOCK
	Interrupts	<BlockIRQ> C_Block_L
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <Block> <AttackIRQ> <BlockIRQ>
	NextMove        CombatReady
	Flags		PickRandomly
	Priority        2

	Type		male
	  Anim          male\combat_dodge 1 49
	TEnd

	Type		huge
	  Anim          huge\combat_dodge 1 49
	TEnd

	Type		fem
	  Anim          fem\combat_dodge 1 49
	TEnd
MEnd

Move C_Dodge_L
	Requires	COMBAT BLOCK
	Interrupts	<BlockIRQ> C_Block_R
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <Block> <AttackIRQ> <BlockIRQ>
	NextMove        CombatReady
	Priority        2

	Type		male
	  Anim          male\combat_dodge 52
	TEnd

	Type		huge
	  Anim          huge\combat_dodge 52
	TEnd

	Type		fem
	  Anim          fem\combat_dodge 52
	TEnd
MEnd

## BRAWL BLOCK ###########################################################

Move B_Block_R
	Requires	COMBAT BLOCK
	Interrupts	<Right>
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <Block> <BlockIRQ>
	NextMove        Brawlcycle
	Priority        2

	Type		male
	  Anim          male\brawl_block 55
	TEnd

	Type		huge
	  Anim          male\brawl_block 55
	TEnd

	Type		fem
	  Anim          male\brawl_block 55
	TEnd
MEnd

## WEAPON BLOCK ###########################################################

Move WeaponBlock_R
	Requires	COMBAT WEAPON BLOCK
	Interrupts	<BlockIRQ> W_Block_L
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <Block> <AttackIRQ>
	NextMove        WepReady
	Flags		PickRandomly
	Priority        3

	Type		male
	  Anim          male\wep_dodge 1 49
	TEnd

	Type		huge
	  Anim          huge\wep_dodge 1 49
	TEnd

	Type		fem
	  Anim          fem\wep_dodge 1 49
	TEnd
MEnd


Move A_WeaponBlock_R
	Requires	FLY COMBAT WEAPON BLOCK GUN
	Interrupts	<BlockIRQ> A_W_Block_L
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <Block> <AttackIRQ>
	NextMove        W_flycycle
	Priority        5

	Type		male
	  Anim          male\air_wep_dodge 1 49
	TEnd

	Type		huge
	  Anim          huge\air_wep_dodge 1 49
	TEnd

	Type		fem
	  Anim          fem\air_wep_dodge 1 49
	TEnd
MEnd

Move WeaponBlock_L
	Requires	COMBAT WEAPON BLOCK
	Interrupts	<BlockIRQ> W_Block_R
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <Block> <AttackIRQ>
	NextMove        wepReady
	Priority        3

	Type		male
	  Anim          male\wep_dodge 52 104
	TEnd

	Type		huge
	  Anim          huge\wep_dodge 52 104
	TEnd

	Type		fem
	  Anim          fem\wep_dodge 52 104
	TEnd
MEnd

Move A_WeaponBlock_L
	Requires	FLY COMBAT WEAPON BLOCK
	Interrupts	<BlockIRQ> A_W_Block_R
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <Block> <AttackIRQ>
	NextMove        W_flycycle
	Priority        4

	Type		male
	  Anim          male\air_wep_dodge 52 104
	TEnd

	Type		huge
	  Anim          huge\air_wep_dodge 52 104
	TEnd

	Type		fem
	  Anim          fem\air_wep_dodge 52 104
	TEnd
MEnd

## SHOTGUN BLOCK ###########################################################

Move Shotgun_Block_R
	Requires	COMBAT WEAPON GUN BLOCK
	Interrupts	<BlockIRQ> SG_Block_L
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <Block> <AttackIRQ>
	NextMove        ShotgunReady
	Priority        4
	Flags		FixArmReg PickRandomly

	Type		male
	  Anim          male\shotgun_block 1 48
	TEnd

	Type		huge
	  Anim          huge\wep_dodge 1 49
	TEnd

	Type		fem
	  Anim          fem\rocket_dodge 1 68
	TEnd
MEnd

Move Shotgun_Block_L
	Requires	COMBAT WEAPON GUN BLOCK
	Interrupts	<BlockIRQ> SG_Block_R
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <Block> <AttackIRQ>
	NextMove        ShotgunReady
	Priority        4
	Flags		FixArmReg

	Type		male
	  Anim          male\shotgun_block 50 92
	TEnd

	Type		huge
	  Anim          huge\wep_dodge 52 104
	TEnd

	Type		fem
	  Anim          fem\rocket_dodge 71 139
	TEnd
MEnd

Move Shotgun_BlockCycle
	Requires	COMBAT WEAPON GUN
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <TwoHandCombat> <AttackIRQ> <BlockIRQ>
	Flags		Cycle FixArmReg
	NextMove        PostReady
	Priority        3
	Scale           0.50

	Type		male
	  Anim          male\wep_cycle 1 31
	TEnd

	Type		huge
	  Anim          huge\wep_cycle 1 31
	TEnd

	Type		fem
	  Anim          fem\wep_cycle 1 31
	TEnd
MEnd

## TWOHANDED BLOCK ###########################################################

Move Club_Block_L
	Requires	COMBAT WEAPON CLUB BLOCK
	Interrupts	2hCLUBcycle 2hCLUB_half
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <Block> <AttackIRQ>
	NextMove        HammerReady_15
	Priority        4

	Type		male
	  Anim          male\2hclub_block 1 71
	TEnd

	Type		huge
	  Anim          huge\2hclub_block 1 69
	TEnd

	Type		fem
	  Anim          fem\2hclub_block 3 77
	TEnd
MEnd

Move Club_Block_R
	Requires	COMBAT WEAPON CLUB BLOCK
	Interrupts	2hCLUBcycle 2hCLUB_half
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <Block> <AttackIRQ>
	NextMove        HammerReady
	Priority        4

	Type		male
	  Anim          male\2hclub_block 74
	TEnd

	Type		huge
	  Anim          huge\2hclub_block 72
	TEnd

	Type		fem
	  Anim          fem\2hclub_block 80 156
	TEnd
MEnd