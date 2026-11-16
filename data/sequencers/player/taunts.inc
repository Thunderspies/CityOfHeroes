#########################################################
## Taunts.INC - CBruce Power related sequencers
#########################################################

##########################
## Taunt_A (beat chest) ##
##########################

Move TauntA_pre
	Requires	COMBAT TAUNT VARIATIONA
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        TauntA
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\comb_beatchest 1 46
	  PlayFx        FEET\MALETAUNTA.FX
	TEnd

	Type		huge
	  Anim          huge\comb_beatchest 1 46
	  PlayFx        FEET\HUGETAUNTA.FX
	TEnd

	Type		fem
	  Anim          fem\comb_beatchest 1 46
	  PlayFx        FEET\FEMALETAUNTA.FX
	TEnd
MEnd

Move TauntA
	Requires	COMBAT
	Priority        1
	Member		<DeathIRQ> <ReactIRQ> <AttackIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove        TauntA_cycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\comb_beatchest 47 91
	TEnd

	Type		huge
	  Anim          huge\comb_beatchest 47 91
	TEnd

	Type		fem
	  Anim          fem\comb_beatchest 47 91
	TEnd
MEnd

Move TauntA_cycle
	Requires	COMBAT
	Priority        1
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <MoveIRQ> <IRQEmotesCombat>
	Flags		Cycle
	NextMove        postReady_back

	Type		male
	  Anim          male\comb_beatchest 95 144
	TEnd

	Type		huge
	  Anim          huge\comb_beatchest 95 144
	TEnd

	Type		fem
	  Anim          fem\comb_beatchest 95 144
	TEnd
MEnd


##############################
## Air Taunt_A (beat chest) ##
##############################

Move A_TauntA_pre
	Requires	FLY COMBAT TAUNT VARIATIONA
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_TauntA
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_comb_beatchest 1 46
	  PlayFx        FEET\MALETAUNTA.FX
	TEnd

	Type		huge
	  Anim          huge\air_comb_beatchest 1 46
	  PlayFx        FEET\HUGETAUNTA.FX
	TEnd

	Type		fem
	  Anim          fem\air_comb_beatchest 1 46
	  PlayFx        FEET\FEMALETAUNTA.FX
	TEnd
MEnd

Move A_TauntA
	Requires	FLY COMBAT
	Priority        2
	Member		<DeathIRQ> <AttackIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove        C_flycycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_comb_beatchest 47 106
	TEnd

	Type		huge
	  Anim          huge\air_comb_beatchest 47 106
	TEnd

	Type		fem
	  Anim          fem\air_comb_beatchest 47 106
	TEnd
MEnd


########################
## Taunt_B (come ON!) ##
########################

Move TauntB_pre
	Requires	COMBAT TAUNT VARIATIONB
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        TauntB
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\combat_comeon 9 54
	  PlayFX	feet\MaleTaunt12.fx
	TEnd

	Type		huge
	  Anim          huge\combat_comeon 9 54
	  PlayFX	feet\mALETaunt6.fx
	TEnd

	Type		fem
	  Anim          fem\combat_comeon 9 54
	  PlayFX	feet\fEMTaunt9.fx
	TEnd
MEnd

Move TauntB
	Requires	COMBAT
	Priority        1
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove        TauntB_cycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\combat_comeon 55 118
	  PlayFX	feet\handpunch1.fx	100
	TEnd

	Type		huge
	  Anim          huge\combat_comeon 55 118
	  PlayFX	feet\handpunch1.fx	100
	TEnd

	Type		fem
	  Anim          fem\combat_comeon 55 118
	  PlayFX	feet\handpunch1.fx	100
	TEnd
MEnd

Move TauntB_cycle
	Requires	COMBAT
	Priority        1
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <MoveIRQ> <IRQEmotesCombat>
	Flags		Cycle
	NextMove        postReady_back

	Type		male
	  Anim          male\combat_comeon 119 156
	  PlayFX	feet\handpunch1.fx
	TEnd

	Type		huge
	  Anim          huge\combat_comeon 119 156
	  PlayFX	feet\handpunch1.fx
	TEnd

	Type		fem
	  Anim          fem\combat_comeon 119 156
	  PlayFX	feet\handpunch1.fx
	TEnd
MEnd


############################
## Air Taunt_B (come ON!) ##
############################

Move A_TauntB_pre
	Requires	FLY COMBAT TAUNT VARIATIONB
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_TauntB
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_combat_comeon 9 54
	  PlayFX	feet\MaleTaunt12.fx
	TEnd

	Type		huge
	  Anim          huge\air_combat_comeon 9 54
	  PlayFX	feet\mALETaunt6.fx
	TEnd

	Type		fem
	  Anim          fem\air_combat_comeon 9 54
	  PlayFX	feet\fEMTaunt9.fx
	TEnd
MEnd

Move A_TauntB
	Requires	FLY COMBAT
	Priority        2
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove        A_TauntB_cycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_combat_comeon 55 118
	  PlayFX	feet\handpunch1.fx
	TEnd

	Type		huge
	  Anim          huge\air_combat_comeon 55 118
	  PlayFX	feet\handpunch1.fx
	TEnd

	Type		fem
	  Anim          fem\air_combat_comeon 55 118
	  PlayFX	feet\handpunch1.fx
	TEnd
MEnd

Move A_TauntB_cycle
	Requires	FLY COMBAT
	Priority        2
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <MoveIRQ> <IRQEmotesCombat>
	Flags		Cycle
	NextMove        C_flycycle_half

	Type		male
	  Anim          male\air_combat_comeon 119 156
	  PlayFX	feet\handpunch1.fx
	TEnd

	Type		huge
	  Anim          huge\air_combat_comeon 119 156
	  PlayFX	feet\handpunch1.fx
	TEnd

	Type		fem
	  Anim          fem\air_combat_comeon 119 156
	  PlayFX	feet\handpunch1.fx
	TEnd
MEnd


#############################
## Taunt_C (2H come here!) ##
#############################

Move TauntC_pre
	Requires	COMBAT TAUNT VARIATIONC
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        TauntC
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\combat_comehere 5 50
  	  PlayFX	feet\MaleTaunt13.fx
	TEnd

	Type		huge
	  Anim          huge\combat_comehere 5 50
	  PlayFX	feet\MaleTaunt8.fx
	TEnd

	Type		fem
	  Anim          fem\combat_comehere 5 50
	  PlayFX	feet\FemTaunt10.fx
	TEnd
MEnd

Move TauntC
	Requires	COMBAT
	Priority        1
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove        Combat_half
	Flags		NoSizeScale

	Type		male
	  Anim          male\combat_comehere 51
	TEnd

	Type		huge
	  Anim          huge\combat_comehere 51
	TEnd

	Type		fem
	  Anim          fem\combat_comehere 51
	TEnd
MEnd


#################################
## Air Taunt_C (2H come here!) ##
#################################

Move A_TauntC_Pre
	Requires	FLY COMBAT TAUNT VARIATIONC
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_TauntC
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_combat_comehere 5 50
	  PlayFX	feet\maletaunt4.fx
	TEnd

	Type		huge
	  Anim          huge\air_combat_comehere 5 50
	  PlayFX	FEET\HUGETAUNTA.FX
	TEnd

	Type		fem
	  Anim          fem\air_combat_comehere 5 50
	  PlayFX	FEET\femtaunt10.FX
	TEnd
MEnd

Move A_TauntC
	Requires	FLY COMBAT
	Priority        2
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove        C_flycycle_half
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_combat_comehere 51
	TEnd

	Type		huge
	  Anim          huge\air_combat_comehere 51
	TEnd

	Type		fem
	  Anim          fem\air_combat_comehere 51
	TEnd
MEnd


#################
## Sword Taunt ##
#################

Move SwordTaunt_Draw
	Requires	COMBAT WEAPON AXE TAUNT VARIATIONA SPAWNBACK
	Priority        6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        SwordTaunt
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\draw_axe 1 30
	TEnd

	Type		huge
	  Anim          huge\draw_axe 1 30
	TEnd

	Type		fem
	  Anim          fem\draw_axe 1 30
	TEnd
MEnd

Move SwordTaunt
	Requires	COMBAT WEAPON AXE TAUNT VARIATIONA
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        SwordTaunt_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\knife_comehere 1 46
	  PlayFX	feet\MaleTaunt4.fx
	TEnd

	Type		Huge
	  Anim          male\knife_comehere 1 46
	  PlayFX	feet\MaleTaunt6.fx
	TEnd

	Type		fem
	  Anim          male\knife_comehere 1 46
	  PlayFX	feet\FemTaunt10.fx
	TEnd
MEnd

Move SwordTaunt_Post
	Requires	COMBAT WEAPON
	Priority        2
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <MoveIRQ> <IRQEmotesRighthand>
	NextMove        SwordTaunt_Cycle

	Type		male
	  Anim          male\knife_comehere 47
	TEnd
MEnd

Move SwordTaunt_Cycle
	Requires	COMBAT WEAPON
	Priority        2
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <MoveIRQ> <IRQEmotesRighthand>
	Flags		Cycle
	NextMove        postReady_back

	Type		male
	  Anim          male\knifetoss 1
	TEnd

	Type		fem
	  Anim          fem\knifetoss 1
	  Scale		0.5
	TEnd

	Type		huge
	  Anim          huge\knifetoss 1
	  Scale		0.6
	TEnd
MEnd


#####################
## Air Sword Taunt ##
#####################

Move A_SwordTaunt_Draw
	Requires	FLY COMBAT WEAPON AXE TAUNT VARIATIONA SPAWNBACK
	Priority        7
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_SwordTaunt
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_draw_axe 1 30
	TEnd
MEnd

Move A_SwordTaunt
	Requires	FLY COMBAT WEAPON AXE TAUNT VARIATIONA
	Priority        6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_SwordTaunt_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_knife_taunt 1 46
	  PlayFX	feet\MaleTaunt4.fx
	TEnd

	Type		Huge
	  Anim          male\air_knife_taunt 1 46
	  PlayFX	feet\MaleTaunt6.fx
	TEnd

	Type		fem
	  Anim          male\air_knife_taunt 1 46
	  PlayFX	feet\FemTaunt5.fx
	TEnd
MEnd

Move A_SwordTaunt_Post
	Requires	FLY COMBAT WEAPON
	Priority        3
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <MoveIRQ> <IRQEmotesRighthand>
	NextMove        A_SwordTaunt_Cycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_knife_taunt 47
	TEnd
MEnd

Move A_SwordTaunt_Cycle
	Requires	FLY COMBAT WEAPON
	Priority        3
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <MoveIRQ> <IRQEmotesRighthand>
	Flags		Cycle
	NextMove        C_flycycle_half

	Type		male
	  Anim          male\air_knife_toss 1
	TEnd

	Type		fem
	  Anim          fem\air_knife_toss 1
	  Scale		0.5
	TEnd

	Type		huge
	  Anim          huge\air_knife_toss 1
	  Scale		0.6
	TEnd
MEnd


####################
## Ninja To Taunt ##
####################

Move NinjaTo_Taunt_Draw
	Requires	COMBAT WEAPON DUAL REV KATANA TAUNT VARIATIONA NINJASWORD SPAWNHIP
	Priority        9
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Katana_Taunt
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\ninjasword_draw 1 30
	TEnd
MEnd

Move Katana_Taunt_Draw
	Requires	COMBAT WEAPON DUAL REV KATANA TAUNT VARIATIONA SPAWNHIP
	Priority        8
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Katana_Taunt
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\katana_draw 1 30
	TEnd

	Type		fem
	  Anim          fem\katana_draw 1 30
	TEnd

	Type		huge
	  Anim          huge\katana_draw 1 30
	TEnd

	Type		Tsoo
	  Anim          male\ninjasword_draw 1 30
	TEnd
MEnd

Move Katana_Taunt
	Requires	COMBAT WEAPON DUAL REV KATANA TAUNT VARIATIONA
	Priority	7
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Katana_Taunt_Post
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\katana_taunt 1 46
	TEnd

	Type		fem
	  Anim          fem\katana_taunt 1 46
	TEnd

	Type		huge
	  Anim          huge\katana_taunt 1 46
	TEnd
MEnd

Move Katana_Taunt_Post
	Requires	COMBAT WEAPON DUAL REV
	Priority        4
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <MoveIRQ> <IRQEmotesLefthand>
	NextMove        Katana_half
	Flags		FixArmReg

	Type		male
	  Anim          male\katana_taunt 47
	TEnd

	Type		fem
	  Anim          fem\katana_taunt 47
	TEnd

	Type		huge
	  Anim          huge\katana_taunt 47
	TEnd
MEnd


########################
## Air Ninja To Taunt ##
########################

Move A_NinjaTo_Taunt_Draw
	Requires	FLY COMBAT WEAPON DUAL REV KATANA TAUNT VARIATIONA NINJASWORD SPAWNHIP
	Priority        10
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Katana_Taunt
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\air_ninjasword_draw 1 30
	TEnd
MEnd

######################
## Air Katana Taunt ##
######################

Move A_Katana_Taunt_Draw
	Requires	FLY COMBAT WEAPON DUAL REV KATANA TAUNT VARIATIONA SPAWNHIP
	Priority        9
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Katana_Taunt
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\air_katana_draw 1 30
	TEnd

	Type		fem
	  Anim          fem\air_katana_draw 1 30
	TEnd

	Type		huge
	  Anim          huge\air_katana_draw 1 30
	TEnd
MEnd

Move A_Katana_Taunt
	Requires	FLY COMBAT WEAPON DUAL REV KATANA TAUNT VARIATIONA
	Priority        8
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Katana_Taunt_Post
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\air_katana_taunt 1 46
	TEnd

	Type		fem
	  Anim          fem\air_katana_taunt 1 46
	TEnd

	Type		huge
	  Anim          huge\air_katana_taunt 1 46
	TEnd
MEnd

Move A_Katana_Taunt_Post
	Requires	COMBAT WEAPON DUAL REV Fly
	Priority        5
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <FlyWepAttack> <MoveIRQ> <IRQEmotesLefthand>
	NextMove        A_Katana_Ready
	Flags		FixArmReg

	Type		male
	  Anim          male\air_katana_taunt 47
	TEnd

	Type		fem
	  Anim          fem\air_katana_taunt 47
	TEnd

	Type		huge
	  Anim          huge\air_katana_taunt 47
	TEnd
MEnd


########################
## Martial Arts Taunt ##
########################

Move MA_Taunt
	Requires	COMBAT KUNGFU TAUNT VARIATIONB
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        MA_Taunt_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\MA_taunt 1 46
	TEnd

	Type		huge
	  Anim          huge\MA_taunt 1 46
	TEnd

	Type		fem
	  Anim          fem\MA_taunt 1 46
	TEnd
MEnd

Move MA_Taunt_Post
	Requires	COMBAT
	Priority        1
	Member		<DeathIRQ> <AttackIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove        CombatReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\MA_taunt 47
	TEnd

	Type		huge
	  Anim          huge\MA_taunt 47
	TEnd

	Type		fem
	  Anim          fem\MA_taunt 47
	TEnd
MEnd


############################
## Air Martial Arts Taunt ##
############################

Move A_MA_taunt
	Requires	FLY COMBAT KUNGFU TAUNT VARIATIONB
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_MA_taunt2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_MA_taunt 1 46
	TEnd

	Type		huge
	  Anim          huge\air_MA_taunt 1 46
	TEnd

	Type		fem
	  Anim          fem\air_MA_taunt 1 46
	TEnd
MEnd

Move A_MA_taunt2
	Requires	FLY COMBAT
	Priority        2
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove        C_flycycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_MA_taunt 47
	TEnd

	Type		huge
	  Anim          huge\air_MA_taunt 47
	TEnd

	Type		fem
	  Anim          fem\air_MA_taunt 47
	TEnd
MEnd