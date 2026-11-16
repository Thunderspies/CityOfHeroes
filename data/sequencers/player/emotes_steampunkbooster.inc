#############################################################
## emotes_AnimalBooster.inc
#############################################################

## Inspiration Emote ###########################################################

Move Emote_Inspiration
	Requires	EMOTE STEAMPUNK ONE
	Priority        3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat>
	Include sequencers\Members\Emotes_Idle.inc
	Flags		nosizescale
	NextMove	Emote_Inspiration_Thinking

	Type		male
	  Anim          male\emote_chalkboard_ready 1 46
	TEnd

	Type		huge
	  Anim          huge\emote_chalkboard_ready 1 46
	TEnd

MEnd

Move Emote_Inspiration_Thinking
	Include sequencers\Members\Emotes_Idle.inc
	Flags		nosizescale
	NextMove	Emote_Inspiration_Thinking_More

	Type		male
	  Anim          male\emote_chalkboard_cycle 1 40
	TEnd

	Type		huge
	  Anim          huge\emote_chalkboard_cycle 1 40
	TEnd

MEnd


Move Emote_Inspiration_Thinking_More
	Include sequencers\Members\Emotes_Idle.inc
	Flags		nosizescale
	NextMove	Emote_Inspiration_Inspired

	Type		male
	  Anim          male\emote_chalkboard_cycle 1 40
	TEnd

	Type		huge
	  Anim          huge\emote_chalkboard_cycle 1 40
	TEnd

MEnd

Move Emote_Inspiration_Inspired
	Include sequencers\Members\Emotes_Idle.inc
	Flags		nosizescale
	NextMove	Emote_Inspiration_Post

	Type		male
	  Anim          male\emote_inspiration 1 40
	PlayFX 		fx\Emotes\Steampunk\Inspiration.fx 0 constant
	TEnd

	Type		fem
	  Anim          fem\emote_inspiration 1 40
	PlayFX 		fx\Emotes\Steampunk\Inspiration.fx 0 constant
	TEnd

MEnd

Move Emote_Inspiration_Post
	Include sequencers\Members\Emotes_Idle.inc
	Flags		nosizescale
	NextMove	Ready

	Type		male
	  Anim          male\emote_inspiration 40
	TEnd

	Type		fem
	  Anim          fem\emote_inspiration 40
	TEnd

MEnd


## Air Inspiration Emote ###########################################################

Move A_Emote_Inspiration
	Requires	FLY EMOTE STEAMPUNK ONE
	Priority        5
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat>
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		nosizescale
	NextMove	A_Emote_Inspiration_Thinking

	Type		male
	  Anim          male\emote_chalkboard_ready_air 1 46
	TEnd

	Type		huge
	  Anim          huge\emote_chalkboard_ready_air 1 46
	TEnd

MEnd

Move A_Emote_Inspiration_Thinking
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		nosizescale
	NextMove	A_Emote_Inspiration_Thinking_More

	Type		male
	  Anim          male\emote_chalkboard_cycle_air 1 40
	TEnd

	Type		huge
	  Anim          huge\emote_chalkboard_cycle_air 1 40
	TEnd

MEnd

Move A_Emote_Inspiration_Thinking_More
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		nosizescale
	NextMove	A_Emote_Inspiration_Inspired

	Type		male
	  Anim          male\emote_chalkboard_cycle_air 1 40
	TEnd

	Type		huge
	  Anim          huge\emote_chalkboard_cycle_air 1 40
	TEnd

MEnd

Move A_Emote_Inspiration_Inspired
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		nosizescale
	NextMove	A_Emote_Inspiration_Post

	Type		male
	  Anim          male\air_emote_inspiration 1 40
	PlayFX 		fx\Emotes\Steampunk\Inspiration.fx 0 constant
	TEnd

	Type		huge
	  Anim          male\air_emote_inspiration 1 40
	PlayFX 		fx\Emotes\Steampunk\Inspiration.fx 0 constant
	TEnd

MEnd

Move A_Emote_Inspiration_Post
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		nosizescale
	NextMove	FlyReady

	Type		male
	  Anim          male\air_emote_inspiration 40
	TEnd

	Type		huge
	  Anim          male\air_emote_inspiration 40
	TEnd

MEnd

##  Train whistle  Emote ###########################################################

Move Emote_Train_Whistle
	Requires	EMOTE STEAMPUNK TWO
	Priority        3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat>
	Include sequencers\Members\Emotes_Idle.inc
	Flags		nosizescale cantmove
	NextMove	Emote_Train_Whistle_Post

	Type		male
	  Anim          male\emote_trainwhistle 1 30
	  PlayFX		fx\Emotes\Steampunk\SteamWhistle_Male.fx 0 constant
	TEnd

	Type		fem
	  Anim          fem\emote_trainwhistle 1 30
	  PlayFX		fx\Emotes\Steampunk\SteamWhistle_Fem.fx 0 constant
	TEnd


	Type		huge
	  Anim          male\emote_trainwhistle 1 30
	  PlayFX		fx\Emotes\Steampunk\SteamWhistle_Huge.fx 0 constant
	TEnd



MEnd

Move Emote_Train_Whistle_Post
	Include sequencers\Members\Emotes_Idle.inc
	Flags		nosizescale
	NextMove	Ready

	Type		male
	  Anim          male\emote_trainwhistle 30
	  PlayFX		fx\Emotes\Steampunk\SteamWhistle_Male.fx 0 constant
	TEnd

	Type		fem
	  Anim          fem\emote_trainwhistle 30
	  PlayFX		fx\Emotes\Steampunk\SteamWhistle_Fem.fx 0 constant
	TEnd


	Type		huge
	  Anim          male\emote_trainwhistle 30
	  PlayFX		fx\Emotes\Steampunk\SteamWhistle_Huge.fx 0 constant
	TEnd


MEnd

## Air Train whistle Emote ###########################################################

Move A_Emote_Train_Whistle
	Requires	FLY EMOTE STEAMPUNK TWO
	Priority        5
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat>
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		NoSizeScale cantmove
	NextMove	A_Emote_Train_Whistle_Post


	Type		male
	  Anim          male\air_emote_trainwhistle 1 30
	  PlayFX		fx\Emotes\Steampunk\SteamWhistle_Male_Air.fx 0 constant
	TEnd

	Type		fem
	  Anim          male\air_emote_trainwhistle 1 30
	  PlayFX		fx\Emotes\Steampunk\SteamWhistle_Fem_Air.fx 0 constant
	TEnd

	Type		huge
	  Anim          male\air_emote_trainwhistle 1 30
	  PlayFX		fx\Emotes\Steampunk\SteamWhistle_Huge_Air.fx 0 constant
	TEnd

MEnd

Move A_Emote_Train_Whistle_Post
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		NoSizeScale
	NextMove	FlyReady


	Type		male
	  Anim          male\air_emote_trainwhistle 30
	  PlayFX		fx\Emotes\Steampunk\SteamWhistle_Male_Air.fx 0 constant
	TEnd

	Type		fem
	  Anim          male\air_emote_trainwhistle 30
	  PlayFX		fx\Emotes\Steampunk\SteamWhistle_Fem_Air.fx 0 constant
	TEnd

	Type		huge
	  Anim          male\air_emote_trainwhistle 30
	  PlayFX		fx\Emotes\Steampunk\SteamWhistle_Huge_Air.fx 0 constant
	TEnd

MEnd

## Hot Temper Emote ###########################################################

Move Emote_Hot_Temper_Pre
	Requires	EMOTE STEAMPUNK THREE
	Priority        3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat>
	Include sequencers\Members\Emotes_Idle.inc
	Flags		nosizescale cantmove
	NextMove	Emote_Hot_Temper_Stomp

	Type		male
	  Anim          male\emote_hottemper 1 30
	TEnd

	Type		fem
	  Anim          fem\emote_hottemper 1 30
	TEnd

MEnd

Move Emote_Hot_Temper_Stomp
	Include sequencers\Members\Emotes_Idle.inc
	Flags		nosizescale
	NextMove	Emote_Hot_Temper_Stomp_Two

	Type		male
	  Anim          male\emote_hottemper 31 52
	  PlayFX	Feet\MaleRight.fx 1
	  PlayFX	Feet\MaleLeft.fx 20
	TEnd

	Type		fem
	  Anim          fem\emote_hottemper 31 52
	  PlayFX	Feet\FemRight.fx 1
	  PlayFX	Feet\FemLeft.fx 20
	TEnd

MEnd

Move Emote_Hot_Temper_Stomp_Two
	Include sequencers\Members\Emotes_Idle.inc
	Flags		nosizescale
	NextMove	Emote_Hot_Temper_Post

	Type		male
	  Anim          male\emote_hottemper 53 74
	  PlayFX		fx\Emotes\Steampunk\HotTemper.fx 0 constant
	TEnd

	Type		fem
	  Anim          fem\emote_hottemper 53 74
	  PlayFX		fx\Emotes\Steampunk\HotTemper.fx 0 constant
	TEnd

MEnd


Move Emote_Hot_Temper_Post
	Include sequencers\Members\Emotes_Idle.inc
	Flags		nosizescale
	NextMove	Ready

	Type		male
	  Anim          male\emote_hottemper 75
	TEnd

	Type		fem
	  Anim          fem\emote_hottemper 75
	TEnd

MEnd

## Air Hot Temper Emote ###########################################################

Move A_Emote_Hot_Temper
	Requires	FLY EMOTE STEAMPUNK THREE
	Priority        5
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat>
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		NoSizeScale cantmove
	NextMove	A_Emote_Hot_Temper_Stomp


	Type		male
	  Anim          male\emote_hottemper 1 30
	TEnd

	Type		fem
	  Anim          fem\emote_hottemper 1 30
	TEnd

MEnd

Move A_Emote_Hot_Temper_Stomp
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		NoSizeScale
	NextMove	A_Emote_Hot_Temper_Stomp_Two

	Type		male
	  Anim          male\emote_hottemper 31 52
	TEnd

	Type		fem
	  Anim          fem\emote_hottemper 31 52
	TEnd

MEnd

Move A_Emote_Hot_Temper_Stomp_Two
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		NoSizeScale
	NextMove	A_Emote_Hot_Temper_Post

	Type		male
	  Anim          male\emote_hottemper 53 74
	  PlayFX		fx\Emotes\Steampunk\HotTemper.fx 0 constant
	TEnd

	Type		fem
	  Anim          fem\emote_hottemper 53 74
	  PlayFX		fx\Emotes\Steampunk\HotTemper.fx 0 constant
	TEnd

MEnd

Move A_Emote_Hot_Temper_Post
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		NoSizeScale
	NextMove	FlyReady

	Type		male
	  Anim          male\emote_hottemper 75
	TEnd

	Type		fem
	  Anim          fem\emote_hottemper 75
	TEnd

MEnd

###########################################################
##		Costume Change Emotes
###########################################################

##  Oil Strike Explode ###########################################################

Move Emote_CostumeChange_Oil_Strike_Pre
	Requires	EMOTE CCCHANGE STEAMPUNK FIVE
	Priority        15
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Flags		NoSizeScale
	NextMove	Emote_CostumeChange_Oil_Strike
	Sets            CANTMOVE

	Type		male
	  Anim          male\emote_oilstrike_summon 1 28
	  PlayFX	Emotes\CostumeChanges\OilStrike\OilStrikeEmote.fx
	TEnd

MEnd

Move Emote_CostumeChange_Oil_Strike
	Include sequencers\Members\Emotes_Idle.inc
	Flags		nosizescale
	NextMove        Emote_CostumeChange_Oil_Strike_Post
	Sets            CANTMOVE COSTUMECHANGE

	Type		male
	  Anim          male\emote_oilstrike_summon 29 60
	TEnd

MEnd

Move Emote_CostumeChange_Oil_Strike_Post
	Include sequencers\Members\Emotes_Idle.inc
	Flags		NosizeScale
	NextMove        Ready

	Type		male
	  Anim          male\emote_oilstrike_summon 61
	TEnd

MEnd

## AIR Oil Strike Explode ###########################################################

Move A_Emote_CostumeChange_Oil_Strike_Pre
	Requires	FLY EMOTE CCCHANGE STEAMPUNK FIVE
	Priority        26
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove        A_Emote_CostumeChange_Oil_Strike
	Flags		NoSizeScale
	Sets            CANTMOVE

	Type		male
	  Anim          male\air_emote_oilstrike_summon 1 28
	  PlayFX	Emotes\CostumeChanges\OilStrike\OilStrikeEmote_FLY.fx
	TEnd

MEnd

Move A_Emote_CostumeChange_Oil_Strike
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		nosizescale
	NextMove        A_Emote_CostumeChange_Oil_Strike_Post
	Sets            COSTUMECHANGE CANTMOVE

	Type		male
	  Anim          male\air_emote_oilstrike_summon 29 60
	TEnd

MEnd


Move A_Emote_CostumeChange_Oil_Strike_Post
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		nosizescale
	NextMove        flyready

	Type		male
	  Anim          male\air_emote_oilstrike_summon 61
	TEnd

MEnd

## Presure Release Explode ###########################################################

Move Emote_CostumeChange_Pressure_Release_Pre
	Requires	EMOTE CCCHANGE STEAMPUNK FOUR
	Priority        14
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Flags		NoSizeScale
	NextMove	Emote_CostumeChange_Pressure_Release
	Sets            CANTMOVE

	Type		male
	  Anim          male\emote_pressure_release 1 41
	  PlayFX	Emotes\CostumeChanges\PressureRelease\PressureReleaseEmote.fx 38
	TEnd

MEnd

Move Emote_CostumeChange_Pressure_Release
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_CostumeChange_Pressure_Release_Post
	Sets            CANTMOVE COSTUMECHANGE
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_pressure_release 41 61
	TEnd

MEnd

Move Emote_CostumeChange_Pressure_Release_Post
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Ready
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_pressure_release 61
	TEnd

MEnd

## AIR Presure Release Explode ###########################################################

Move A_Emote_CostumeChange_Pressure_Release_Pre
	Requires	FLY EMOTE CCCHANGE STEAMPUNK FOUR
	Priority        25
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove	A_Emote_CostumeChange_Pressure_Release
	Flags		NoSizeScale
	Sets            CANTMOVE

	Type		male
	  Anim          male\air_emote_oilstrike_summon 1 28
	  PlayFX	Emotes\CostumeChanges\PressureRelease\PressureReleaseEmote_FLY.fx 25
	TEnd

MEnd

Move A_Emote_CostumeChange_Pressure_Release
	Requires	FLY
	Priority        1
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove        A_Emote_CostumeChange_Pressure_Release_Post
	Sets		CANTMOVE COSTUMECHANGE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_emote_oilstrike_summon 29 60
	TEnd
MEnd

Move A_Emote_CostumeChange_Pressure_Release_Post
	Include sequencers\Members\Emotes_Idle_Air.inc
	Requires	FLY
	Flags		NoSizeScale
	NextMove        flyready

	Type		male
	  Anim          male\air_emote_oilstrike_summon 61
	TEnd

MEnd

###########################################################