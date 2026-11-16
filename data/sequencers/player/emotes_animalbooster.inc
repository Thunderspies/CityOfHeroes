#############################################################
## emotes_AnimalBooster.inc
#############################################################



## Sniff Emote ###########################################################

Move Emote_Sniff
	Requires	EMOTE ANIMAL YELL ONE
	Priority        4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat>
	Include sequencers\Members\Emotes_Idle.inc
	Flags		nosizescale
	NextMove	Emote_Sniff_Post

	Type		male
	  Anim          male\emote_sniff 1 17
	  PlayFX		Emotes\AnimalPack\SniffEmote.fx
	TEnd

	Type		fem
	  Anim          fem\emote_sniff 1 17
	  PlayFX		Emotes\AnimalPack\SniffEmote.fx
	TEnd

MEnd

Move Emote_Sniff_Post
	Include sequencers\Members\Emotes_Idle.inc
	Flags		nosizescale
	NextMove	Ready

	Type		male
	  Anim          male\emote_sniff 17
	TEnd

	Type		fem
	  Anim          fem\emote_sniff 17
	TEnd

MEnd


## Air Sniff Emote ###########################################################

Move A_Emote_Sniff
	Requires	FLY EMOTE ANIMAL YELL ONE
	Priority        5
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat>
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		nosizescale
	NextMove	A_Emote_Sniff_Post

	Type		male
	  Anim          male\air_emote_sniff 1 17
	  PlayFX		Emotes\AnimalPack\SniffEmote.fx
	TEnd

	Type		fem
	  Anim          fem\air_emote_sniff 1 17
	  PlayFX		Emotes\AnimalPack\SniffEmote.fx
	TEnd

MEnd

Move A_Emote_Sniff_Post
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		nosizescale
	NextMove	FlyReady

	Type		male
	  Anim          male\air_emote_sniff 17
	TEnd

	Type		fem
	  Anim          fem\air_emote_sniff 17
	TEnd

MEnd

## Hiss Emote ###########################################################

Move Emote_Hiss
	Requires	EMOTE ANIMAL ALTERNATE ONE
	Priority        5
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat>
	Include sequencers\Members\Emotes_Idle.inc
	Flags		nosizescale
	NextMove	Emote_Hiss_Post

	Type		male
	  Anim          male\emote_hiss 1 27
	  PlayFX		Emotes\AnimalPack\HissEmote.fx
	TEnd

	Type		fem
	  Anim          fem\emote_hiss 1 27
	  PlayFX		Emotes\AnimalPack\HissEmote.fx
	TEnd

MEnd

Move Emote_Hiss_Post
	Include sequencers\Members\Emotes_Idle.inc
	Flags		nosizescale
	NextMove	Ready

	Type		male
	  Anim          male\emote_hiss 27
	TEnd

	Type		fem
	  Anim          fem\emote_hiss 27
	TEnd

MEnd

## Air Hiss Emote ###########################################################

Move A_Emote_Hiss
	Requires	FLY EMOTE ANIMAL ALTERNATE ONE
	Priority        6
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat>
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		NoSizeScale
	NextMove	A_Emote_Hiss_Post


	Type		male
	  Anim          male\air_emote_hiss 1 27
	  PlayFX		Emotes\AnimalPack\HissEmote.fx
	TEnd

MEnd

Move A_Emote_Hiss_Post
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		NoSizeScale
	NextMove	FlyReady


	Type		male
	  Anim          male\air_emote_hiss 27
	  PlayFX		Emotes\AnimalPack\HissEmote.fx
	TEnd

MEnd

## Monkey Emote ###########################################################

Move Emote_Monkey_Dance
	Requires	EMOTE ANIMAL DANCE ALTERNATE
	Priority        4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat>
	Include sequencers\Members\Emotes_Idle.inc
	Flags		nosizescale
	NextMove	Emote_Monkey_Dance_Post

	Type		male
	  Anim          male\emote_monkey 1 34
	  PlayFX		Emotes\AnimalPack\MonkeyEmote.fx
	TEnd

	Type		huge
	  Anim          huge\emote_monkey 1 34
	  PlayFX		Emotes\AnimalPack\MonkeyEmote.fx
	TEnd

	Type		fem
	  Anim          fem\emote_monkey 1 34
	  PlayFX		Emotes\AnimalPack\MonkeyEmote.fx
	TEnd

MEnd

Move Emote_Monkey_Dance_Post
	Include sequencers\Members\Emotes_Idle.inc
	Flags		nosizescale
	NextMove	Emote_Monkey_Dance_Cycle

	Type		male
	  Anim          male\emote_monkey 34 160
	TEnd

	Type		huge
	  Anim          huge\emote_monkey 34 160
	TEnd

	Type		fem
	  Anim          fem\emote_monkey 34 160
	TEnd

MEnd

Move Emote_Monkey_Dance_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		nosizescale Cycle
	CycleMove	Emote_Monkey_Dance_Cycle
	NextMove	Emote_Monkey_Dance_Cycle

	Type		male
	  Anim          male\emote_monkey 160 220
	TEnd

	Type		huge
	  Anim          huge\emote_monkey 160 220
	TEnd

	Type		fem
	  Anim          fem\emote_monkey 160 220
	TEnd

MEnd

###########################################################
##		Costume Change Emotes
###########################################################

## Feather Explode ###########################################################

Move Emote_CostumeChange_Feather_Explode_Pre
	Requires	EMOTE CCCHANGE ANIMAL MIX ONE
	Priority        5
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Flags		NoSizeScale
	NextMove	Emote_CostumeChange_Feather_Explode
	Sets            CANTMOVE

	Type		male
	  Anim          male\alt_beatchest 1 35
	  PlayFX	Emotes\CostumeChanges\FeatherBurst\FeatherBurst.fx 30
	TEnd

	Type		huge
	  Anim          huge\alt_beatchest 1 35
	  PlayFX	Emotes\CostumeChanges\FeatherBurst\FeatherBurst.fx 30
	TEnd

	Type		fem
	  Anim          fem\alt_beatchest 1 35
	  PlayFX	Emotes\CostumeChanges\FeatherBurst\FeatherBurst.fx 30
	TEnd

MEnd

Move Emote_CostumeChange_Feather_Explode
	Include sequencers\Members\Emotes_Idle.inc
	Flags		nosizescale
	NextMove        Emote_CostumeChange_Feather_Explode_Post
	Sets            COSTUMECHANGE

	Type		male
	  Anim          male\alt_beatchest 36 91
	TEnd

	Type		huge
	  Anim          huge\alt_beatchest 36 91
	TEnd

	Type		fem
	  Anim          fem\alt_beatchest 36 91
	TEnd
MEnd

Move Emote_CostumeChange_Feather_Explode_Post
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle NosizeScale
	NextMove        postReady_back
	Interpolate	8

	Type		male
	  Anim          male\alt_beatchest_post 92 144
	TEnd

	Type		huge
	  Anim          huge\comb_beatchest 95 144
	TEnd

	Type		fem
	  Anim          fem\comb_beatchest 95 144
	TEnd
MEnd

## AIR Feather Explode ###########################################################

Move A_Emote_CostumeChange_Feather_Explode
	Requires	FLY EMOTE CCCHANGE ANIMAL MIX ONE
	Priority        16
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove        A_Emote_CostumeChange_Feather_Exploder
	Flags		NoSizeScale
	Sets            CANTMOVE

	Type		male
	  Anim          male\air_alt_beatchest 1 35
	  PlayFX	Emotes\CostumeChanges\FeatherBurst\FeatherBurst.fx 30
	TEnd

	Type		huge
	  Anim          huge\air_alt_beatchest 1 35
	  PlayFX	Emotes\CostumeChanges\FeatherBurst\FeatherBurst.fx 30
	TEnd

MEnd


Move A_Emote_CostumeChange_Feather_Exploder
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		nosizescale
	NextMove        A_Emote_CostumeChange_Feather_Explode_Cycle
	Sets            COSTUMECHANGE

	Type		male
	  Anim          male\air_alt_beatchest 36 91
	TEnd

	Type		huge
	  Anim          huge\air_alt_beatchest 36 91
	TEnd

MEnd

Move A_Emote_CostumeChange_Feather_Explode_Cycle
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		Cycle nosizescale
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

MEnd


## Fur Explode ###########################################################

Move Emote_CostumeChange_Fur_Explode_Pre
	Requires	EMOTE CCCHANGE ANIMAL MIX TWO
	Priority        5
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Flags		NoSizeScale
	NextMove	Emote_CostumeChange_Fur_Explode
	Sets            CANTMOVE

	Type		male
	  Anim          male\emote_Rapidboil 1 65
	TEnd

	Type		fem
	  Anim          fem\emote_Rapidboil 1 65
	TEnd

	Type		huge
	  Anim          fem\emote_Rapidboil 1 65
	TEnd

MEnd

Move Emote_CostumeChange_Fur_Explode
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_CostumeChange_Fur_Explode_Post
	Sets            CANTMOVE COSTUMECHANGE
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_Rapidboil 66 70
	  PlayFX	Emotes\CostumeChanges\FurBurst\FurBurst.fx 0
	TEnd

	Type		fem
	  Anim          fem\emote_Rapidboil 66 70
	  PlayFX	Emotes\CostumeChanges\FurBurst\FurBurst.fx 0
	TEnd

	Type		huge
	  Anim          fem\emote_Rapidboil 66 70
	  PlayFX	Emotes\CostumeChanges\FurBurst\FurBurst.fx 0
	TEnd
MEnd

Move Emote_CostumeChange_Fur_Explode_Post
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Ready
	Flags		NoSizeScale
	Scale		0.5

	Type		male
	  Anim          male\emote_Rapidboil 71
	TEnd

	Type		fem
	  Anim          fem\emote_Rapidboil 71
	TEnd

	Type		huge
	  Anim          fem\emote_Rapidboil 71
	TEnd
MEnd

## AIR Fur Explode ###########################################################

Move A_Emote_CostumeChange_Fur_Explode_Pre
	Requires	FLY EMOTE CCCHANGE ANIMAL MIX TWO
	Priority        16
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove	A_Emote_CostumeChange_Fur_Explode
	Flags		NoSizeScale
	Sets            CANTMOVE

	Type		male
	  Anim          male\air_emote_Rapidboil 1 65
	TEnd

	Type		fem
	  Anim          male\air_emote_Rapidboil 1 65
	TEnd

	Type		huge
	  Anim          male\air_emote_Rapidboil 1 65
	TEnd

MEnd

Move A_Emote_CostumeChange_Fur_Explode
	Requires	FLY
	Priority        1
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove        A_Emote_CostumeChange_Fur_Explode_Post
	Sets		CANTMOVE COSTUMECHANGE
	Flags		NoSizeScale
	Scale		0.5

	Type		male
	  Anim          male\air_emote_Rapidboil 66 70
	  PlayFX	Emotes\CostumeChanges\FurBurst\FurBurst.fx 0
	TEnd

	Type		fem
	  Anim          male\air_emote_Rapidboil 66 70
	  PlayFX	Emotes\CostumeChanges\FurBurst\FurBurst.fx 0
	TEnd

	Type		huge
	  Anim          male\air_emote_Rapidboil 66 70
	  PlayFX	Emotes\CostumeChanges\FurBurst\FurBurst.fx 0
	TEnd
MEnd

Move A_Emote_CostumeChange_Fur_Explode_Post
	Include sequencers\Members\Emotes_Idle_Air.inc
	Requires	FLY
	Flags		NoSizeScale
	NextMove        Prefall

	Type		male
	  Anim          male\air_emote_Rapidboil 71
	TEnd
MEnd

## Feather Spin ###########################################################

Move Emote_CostumeChange_Feather_Spin_Pre
	Requires	EMOTE CCCHANGE ANIMAL STAND ONE
	Priority        5
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Flags		NoSizeScale
	NextMove	Emote_CostumeChange_Feather_Spinner
	Sets            CANTMOVE

	Type		male
	  Anim          male\spin 1 20
	  PlayFX	Emotes\CostumeChanges\FeatherSpin\FeatherSpin.fx 10
	TEnd

MEnd

Move Emote_CostumeChange_Feather_Spinner
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_CostumeChange_Feather_Spin
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\spin 20 45
	TEnd
MEnd


Move Emote_CostumeChange_Feather_Spin
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	JumpPost
	Sets            CANTMOVE COSTUMECHANGE
	Flags		NoSizeScale

	Type		male
	  Anim          male\spin 45
	TEnd
MEnd

## AIR Feather Spin ###########################################################

Move A_Emote_CostumeChange_Feather_Spin_Pre
	Requires	FLY EMOTE CCCHANGE ANIMAL STAND ONE
	Priority        16
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove	A_Emote_CostumeChange_Feather_Spinner
	Flags		NoSizeScale
	Sets            CANTMOVE

	Type		male
	  Anim          male\spin 1 20
	  PlayFX	Emotes\CostumeChanges\FeatherSpin\FeatherSpin.fx 10
	TEnd

MEnd

Move A_Emote_CostumeChange_Feather_Spinner
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_CostumeChange_Feather_Spin
	Flags		NoSizeScale
	Sets            CANTMOVE

	Type		male
	  Anim          male\spin 20 45
	TEnd
MEnd


Move A_Emote_CostumeChange_Feather_Spin
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	PreFall
	Flags		NoSizeScale
	Sets            CANTMOVE COSTUMECHANGE

	Type		male
	  Anim          male\spin 45
	TEnd
MEnd

## Fur Spin ###########################################################

Move Emote_CostumeChange_Fur_Spin_Pre
	Requires	EMOTE CCCHANGE ANIMAL STAND TWO
	Priority        5
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Flags		NoSizeScale
	NextMove	Emote_CostumeChange_Fur_Spinner
	Sets            CANTMOVE

	Type		male
	  Anim          male\spin 1 20
	  PlayFX	Emotes\CostumeChanges\FurSpin\FurSpin.fx 10 CONSTANT
	TEnd

MEnd

Move Emote_CostumeChange_Fur_Spinner
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Emote_CostumeChange_Fur_Spin
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\spin 20 45
	TEnd
MEnd


Move Emote_CostumeChange_Fur_Spin
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        JumpPost
	Sets            CANTMOVE COSTUMECHANGE
	Flags		NoSizeScale

	Type		male
	  Anim          male\spin 45
	TEnd
MEnd

## AIR Fur Spin ###########################################################

Move A_Emote_CostumeChange_Fur_Spin_Pre
	Requires	FLY EMOTE CCCHANGE ANIMAL STAND TWO
	Priority        16
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove	A_Emote_CostumeChange_Fur_Spinner
	Flags		NoSizeScale
	Sets            CANTMOVE

	Type		male
	  Anim          male\spin 1 20
	  PlayFX	Emotes\CostumeChanges\FurSpin\FurSpin.fx 10 CONSTANT
	TEnd

MEnd

Move A_Emote_CostumeChange_Fur_Spinner
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_CostumeChange_Fur_Spin
	Flags		NoSizeScale
	Sets		CANTMOVE

	Type		male
	  Anim          male\spin 20 45
	TEnd
MEnd

Move A_Emote_CostumeChange_Fur_Spin
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	PreFall
	Flags		NoSizeScale
	Sets		CANTMOVE COSTUMECHANGE

	Type		male
	  Anim          male\spin 45
	TEnd
MEnd

###########################################################