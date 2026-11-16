#############################################################
## emotes_DanceBooster.inc
#############################################################

 ###########################################################
## Disco Dance Emote ###########################################################
 ###########################################################

Move Emote_Disco_Dance_Pre
	Requires	EMOTE DANCE ELEVEN
	Priority        3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	Flags		nosizescale
	NextMove	Emote_Disco_Dance

	Type		male
	  Anim          male\emote_disco_dance 1 7
	TEnd

	Type		fem
	  Anim          fem\emote_disco_dance 1 7
	TEnd

MEnd

Move Emote_Disco_Dance
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Disco_Dance_Cycle
	Flags		nosizescale

	Type		male
	  Anim          male\emote_disco_dance 8 192
	TEnd

	Type		fem
	  Anim          fem\emote_disco_dance 8 192
	TEnd

MEnd

Move Emote_Disco_Dance_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		nosizescale Cycle
	Interpolate	0

	Type		male
	  Anim          male\emote_disco_dance 7 192
	TEnd

	Type		fem
	  Anim          fem\emote_disco_dance 7 192
	TEnd

MEnd

## AirGuitar Emote ###########################################################

Move Emote_Air_Guitar_Pre
	Requires	EMOTE DANCE TEN
	Priority        3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	Flags		nosizescale
	NextMove	Emote_Air_Guitar_Cycle

	Type		male
	  Anim          male\emote_air_guitar_dance 1 22
	TEnd

MEnd

Move Emote_Air_Guitar_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		nosizescale Cycle
	CycleMove	Emote_Air_Guitar_Cycle
	CycleMove	Emote_Air_Guitar_Cycle
	CycleMove	Emote_Air_Guitar_Cycle
	CycleMove	Emote_Air_Guitar_Cycle_HeadSwing

	Type		male
	  Anim          male\emote_air_guitar_dance 22 88
	TEnd

MEnd

Move Emote_Air_Guitar_Cycle_HeadSwing
	Include sequencers\Members\Emotes_Idle.inc
	Flags		nosizescale Cycle
	Scale		1.1
	CycleMove	Emote_Air_Guitar_Cycle
	CycleMove	Emote_Air_Guitar_Cycle
	CycleMove	Emote_Air_Guitar_Cycle
	CycleMove	Emote_Air_Guitar_Cycle_HeadSwing



	Type		male
	  Anim          male\emote_air_guitar_dance 88 124
	TEnd

MEnd

 ###########################################################
## AIR Disco Dance Emote ###########################################################
 ###########################################################

Move A_Emote_Disco_Dance_Pre
	Requires	FLY EMOTE DANCE ELEVEN
	Priority        4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		nosizescale
	NextMove	A_Emote_Disco_Dance

	Type		male
	  Anim          male\emote_disco_dance 1 7
	TEnd

	Type		fem
	  Anim          fem\emote_disco_dance 1 7
	TEnd

MEnd

Move A_Emote_Disco_Dance
	Requires	FLY
	Priority        1
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Disco_Dance_Cycle
	Flags		nosizescale

	Type		male
	  Anim          male\emote_disco_dance 8 192
	TEnd

	Type		fem
	  Anim          fem\emote_disco_dance 8 192
	TEnd

MEnd

Move A_Emote_Disco_Dance_Cycle
	Requires	FLY
	Priority        1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		nosizescale Cycle
	Interpolate	0

	Type		male
	  Anim          male\emote_disco_dance 7 192
	TEnd

	Type		fem
	  Anim          fem\emote_disco_dance 7 192
	TEnd

MEnd

## Air AirGuitar Emote ###########################################################

Move A_Emote_Air_Guitar_Pre
	Requires	FLY EMOTE DANCE TEN
	Priority        4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		nosizescale
	NextMove	A_Emote_Air_Guitar_Cycle

	Type		male
	  Anim          male\emote_air_guitar_dance 1 22
	TEnd

MEnd

Move A_Emote_Air_Guitar_Cycle
	Requires	FLY
	Priority        1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		nosizescale Cycle
	CycleMove	A_Emote_Air_Guitar_Cycle
	CycleMove	A_Emote_Air_Guitar_Cycle
	CycleMove	A_Emote_Air_Guitar_Cycle
	CycleMove	A_Emote_Air_Guitar_Cycle_HeadSwing

	Type		male
	  Anim          male\emote_air_guitar_dance 22 88
	TEnd

MEnd

Move A_Emote_Air_Guitar_Cycle_HeadSwing
	Requires	FLY
	Priority        1
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		nosizescale Cycle
	Scale		1.1
	CycleMove	A_Emote_Air_Guitar_Cycle
	CycleMove	A_Emote_Air_Guitar_Cycle
	CycleMove	A_Emote_Air_Guitar_Cycle
	CycleMove	A_Emote_Air_Guitar_Cycle_HeadSwing

	Type		male
	  Anim          male\emote_air_guitar_dance 88 124
	TEnd

MEnd

## Karate Dance ###########################################################

Move Emote_Karate_Dance_Pre
	Requires	EMOTE DANCE EIGHT
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	Nextmove	Emote_Karate_Dance_one
	Flags		NoSizeScale pickrandomly
	Sets            CANTMOVE

	Type		male
	  Anim          male\emote_karate_dance 1 11
	TEnd
MEnd

Move Emote_Karate_Dance_one
	Include sequencers\Members\Emotes_Idle.inc
	CycleMove	Emote_Karate_Dance_one
	CycleMove	Emote_Karate_Dance_one
	CycleMove	Emote_Karate_Dance_cycle_post
	NextMove	Emote_Karate_Dance_cycle_post
	Flags		NoSizeScale Cycle


	Type		male
	  Anim          male\emote_karate_dance 11 50
	  # PlayFX		NPCFX\Dance_ClothShort_SFX.fx 11
	  # PlayFX		NPCFX\Dance_ClothLong_SFX.fx 22
	  # PlayFX		NPCFX\Dance_ClothShort_SFX.fx 47
	  # PlayFX		NPCFX\Dance_ClothLong_SFX.fx 61
	  # PlayFX		NPCFX\Dance_ClothShort_SFX.fx 82
	  # PlayFX		NPCFX\Dance_ClothLong_SFX.fx 96
	  # PlayFX		NPCFX\Dance_ClothShort_SFX.fx 117
	TEnd
MEnd

Move Emote_Karate_Dance_cycle_post
	Include sequencers\Members\Emotes_Idle.inc
	Cyclemove	Emote_Karate_Dance_cycle_two
	Nextmove	Emote_Karate_Dance_cycle_two
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_karate_dance 50 156
	TEnd
MEnd

Move Emote_Karate_Dance_cycle_two
	Include sequencers\Members\Emotes_Idle.inc
	CycleMove	Emote_Karate_Dance_cycle_two
	CycleMove	Emote_Karate_Dance_post
	NextMove	Emote_Karate_Dance_post
	Flags		NoSizeScale Cycle


	Type		male
	  Anim          male\emote_karate_dance 156 215
	TEnd
MEnd

Move Emote_Karate_Dance_post
	Include sequencers\Members\Emotes_Idle.inc
	CycleMove	Emote_Karate_Dance_one
	CycleMove	Emote_Karate_Dance_Post_kick
	NextMove	Emote_Karate_Dance_Post_kick
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_karate_dance 215 225
	TEnd
MEnd

Move Emote_Karate_Dance_Post_kick
	Include sequencers\Members\Emotes_Idle.inc
	CycleMove	Emote_Karate_Dance_one
	CycleMove	Emote_Karate_Dance_Post_kick
	NextMove	Emote_Karate_Dance_one
	Flags		NoSizeScale
	scale		1.2

	Type		male
	  Anim          male\emote_karate_dance 225 294
	TEnd
MEnd

## Karate Dance ###########################################################

Move A_Emote_Karate_Dance_Pre
	Requires	FLY EMOTE DANCE EIGHT
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	Nextmove	A_Emote_Karate_Dance_one
	Flags		NoSizeScale pickrandomly
	Sets            CANTMOVE

	Type		male
	  Anim          male\emote_karate_dance 1 11
	TEnd
MEnd

Move A_Emote_Karate_Dance_one
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Priority	1
	CycleMove	A_Emote_Karate_Dance_one
	CycleMove	A_Emote_Karate_Dance_one
	CycleMove	A_Emote_Karate_Dance_cycle_post
	NextMove	A_Emote_Karate_Dance_cycle_post
	Flags		NoSizeScale Cycle


	Type		male
	  Anim          male\emote_karate_dance 11 50
	  # PlayFX		NPCFX\Dance_ClothShort_SFX.fx 11
	  # PlayFX		NPCFX\Dance_ClothLong_SFX.fx 22
	  # PlayFX		NPCFX\Dance_ClothShort_SFX.fx 47
	  # PlayFX		NPCFX\Dance_ClothLong_SFX.fx 61
	  # PlayFX		NPCFX\Dance_ClothShort_SFX.fx 82
	  # PlayFX		NPCFX\Dance_ClothLong_SFX.fx 96
	  # PlayFX		NPCFX\Dance_ClothShort_SFX.fx 117
	TEnd
MEnd

Move A_Emote_Karate_Dance_cycle_post
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Cyclemove	A_Emote_Karate_Dance_cycle_two
	Nextmove	A_Emote_Karate_Dance_cycle_two
	Flags		NoSizeScale cycle

	Type		male
	  Anim          male\emote_karate_dance 50 156
	TEnd
MEnd

Move A_Emote_Karate_Dance_cycle_two
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	CycleMove	A_Emote_Karate_Dance_cycle_two
	CycleMove	A_Emote_Karate_Dance_post
	NextMove	A_Emote_Karate_Dance_post
	Flags		NoSizeScale Cycle


	Type		male
	  Anim          male\emote_karate_dance 156 215
	TEnd
MEnd

Move A_Emote_Karate_Dance_post
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	CycleMove	A_Emote_Karate_Dance_one
	CycleMove	A_Emote_Karate_Dance_Post_kick
	NextMove	A_Emote_Karate_Dance_Post_kick
	Flags		NoSizeScale cycle

	Type		male
	  Anim          male\emote_karate_dance 215 225
	TEnd
MEnd

Move A_Emote_Karate_Dance_Post_kick
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	CycleMove	A_Emote_Karate_Dance_one
	CycleMove	A_Emote_Karate_Dance_Post_kick
	NextMove	A_Emote_Karate_Dance_one
	Flags		NoSizeScale cycle
	scale		1.2

	Type		male
	  Anim          male\emote_karate_dance 225 294
	TEnd
MEnd

#############################################################
## Billy Jeans Dance ###########################################################

Move Emote_Dance_BillyJeans_Pre
	Requires	EMOTE DANCE NINE
	Priority	3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesShield> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	Nextmove	Emote_Dance_BillyJeans
	Flags		NoSizeScale pickrandomly
	Sets            CANTMOVE

	Type		male
	  Anim          male\emote_dance_billyjeans 1 2
	TEnd

	Type		fem
	  Anim          fem\emote_dance_billyjeans 1 2
	TEnd

	Type		Huge
	  Anim          huge\emote_dance_billyjeans 1 2
	TEnd
MEnd

Move Emote_Dance_BillyJeans
	Include sequencers\Members\Emotes_Idle.inc
	CycleMove	Emote_Dance_BillyJeans
	Flags		NoSizeScale Cycle

	Type		male
	  Anim          male\emote_dance_billyjeans 2
	  PlayFX		NPCFX\Dance_Cloth.fx 3
	  PlayFX		NPCFX\Dance_Feet.fx 14
	  PlayFX		NPCFX\Dance_Feet.fx 19
	  PlayFX		NPCFX\Dance_ClothLong_SFX.fx 41
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 52
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 74
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 100
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 124
	  PlayFX		NPCFX\Dance_Scuff.fx 145
	  PlayFX		NPCFX\Dance_Cloth.fx 161
	  PlayFX		NPCFX\Dance_Scuff.fx 177
	  PlayFX		NPCFX\Dance_Cloth.fx 219
	  PlayFX		NPCFX\Dance_Feet.fx 244
	  PlayFX		NPCFX\Dance_Feet.fx 249
	  PlayFX		NPCFX\Dance_Cloth.fx 268
	  PlayFX		NPCFX\Dance_Cloth.fx 271
	  PlayFX		NPCFX\Dance_Feet.fx 279
	  PlayFX		NPCFX\Dance_Feet.fx 281
	  PlayFX		NPCFX\Dance_Feet.fx 293
	  PlayFX		NPCFX\Dance_Feet.fx 295
	  PlayFX		NPCFX\Dance_Scuff.fx 328
	  PlayFX		NPCFX\Dance_Scuff.fx 351
	  PlayFX		NPCFX\Dance_Scuff.fx 367
	  PlayFX		NPCFX\Dance_Scuff.fx 387
	  PlayFX		NPCFX\Dance_Scuff.fx 406
	  PlayFX		NPCFX\Dance_Cloth.fx 407
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 423
	  PlayFX		NPCFX\Dance_Feet.fx 430
	  PlayFX		NPCFX\Dance_Feet.fx 432
	TEnd

	Type		fem
	  Anim          fem\emote_dance_billyjeans 2
	  PlayFX		NPCFX\Dance_Cloth.fx 3
	  PlayFX		NPCFX\Dance_Feet.fx 14
	  PlayFX		NPCFX\Dance_Feet.fx 19
	  PlayFX		NPCFX\Dance_ClothLong_SFX.fx 41
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 52
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 74
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 100
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 124
	  PlayFX		NPCFX\Dance_Scuff.fx 145
	  PlayFX		NPCFX\Dance_Cloth.fx 161
	  PlayFX		NPCFX\Dance_Scuff.fx 177
	  PlayFX		NPCFX\Dance_Cloth.fx 219
	  PlayFX		NPCFX\Dance_Feet.fx 244
	  PlayFX		NPCFX\Dance_Feet.fx 249
	  PlayFX		NPCFX\Dance_Cloth.fx 268
	  PlayFX		NPCFX\Dance_Cloth.fx 271
	  PlayFX		NPCFX\Dance_Feet.fx 279
	  PlayFX		NPCFX\Dance_Feet.fx 281
	  PlayFX		NPCFX\Dance_Feet.fx 293
	  PlayFX		NPCFX\Dance_Feet.fx 295
	  PlayFX		NPCFX\Dance_Scuff.fx 328
	  PlayFX		NPCFX\Dance_Scuff.fx 351
	  PlayFX		NPCFX\Dance_Scuff.fx 367
	  PlayFX		NPCFX\Dance_Scuff.fx 387
	  PlayFX		NPCFX\Dance_Scuff.fx 406
	  PlayFX		NPCFX\Dance_Cloth.fx 407
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 423
	  PlayFX		NPCFX\Dance_Feet.fx 430
	  PlayFX		NPCFX\Dance_Feet.fx 432
	TEnd

	Type		Huge
	  Anim          huge\emote_dance_billyjeans 2
	  PlayFX		NPCFX\Dance_Cloth.fx 3
	  PlayFX		NPCFX\Dance_Feet.fx 14
	  PlayFX		NPCFX\Dance_Feet.fx 19
	  PlayFX		NPCFX\Dance_ClothLong_SFX.fx 41
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 52
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 74
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 100
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 124
	  PlayFX		NPCFX\Dance_Scuff.fx 145
	  PlayFX		NPCFX\Dance_Cloth.fx 161
	  PlayFX		NPCFX\Dance_Scuff.fx 177
	  PlayFX		NPCFX\Dance_Cloth.fx 219
	  PlayFX		NPCFX\Dance_Feet.fx 244
	  PlayFX		NPCFX\Dance_Feet.fx 249
	  PlayFX		NPCFX\Dance_Cloth.fx 268
	  PlayFX		NPCFX\Dance_Cloth.fx 271
	  PlayFX		NPCFX\Dance_Feet.fx 279
	  PlayFX		NPCFX\Dance_Feet.fx 281
	  PlayFX		NPCFX\Dance_Feet.fx 293
	  PlayFX		NPCFX\Dance_Feet.fx 295
	  PlayFX		NPCFX\Dance_Scuff.fx 328
	  PlayFX		NPCFX\Dance_Scuff.fx 351
	  PlayFX		NPCFX\Dance_Scuff.fx 367
	  PlayFX		NPCFX\Dance_Scuff.fx 387
	  PlayFX		NPCFX\Dance_Scuff.fx 406
	  PlayFX		NPCFX\Dance_Cloth.fx 407
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 423
	  PlayFX		NPCFX\Dance_Feet.fx 430
	  PlayFX		NPCFX\Dance_Feet.fx 432
	TEnd
MEnd

#############################################################
## Billy Jeans Dance ###########################################################

Move A_Emote_Dance_BillyJeans_Pre
	Requires	FLY EMOTE DANCE NINE
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	Nextmove	A_Emote_Dance_BillyJeans
	Flags		NoSizeScale pickrandomly
	Sets            CANTMOVE

	Type		male
	  Anim          male\emote_dance_billyjeans 1 2
	TEnd

	Type		fem
	  Anim          fem\emote_dance_billyjeans 1 2
	TEnd

	Type		Huge
	  Anim          huge\emote_dance_billyjeans 1 2
	TEnd
MEnd

Move A_Emote_Dance_BillyJeans
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	CycleMove	A_Emote_Dance_BillyJeans
	Flags		NoSizeScale Cycle

	Type		male
	  Anim          male\emote_dance_billyjeans 2
	  PlayFX		NPCFX\Dance_Cloth.fx 3
	  PlayFX		NPCFX\Dance_Feet.fx 14
	  PlayFX		NPCFX\Dance_Feet.fx 19
	  PlayFX		NPCFX\Dance_ClothLong_SFX.fx 41
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 52
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 74
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 100
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 124
	  PlayFX		NPCFX\Dance_Scuff.fx 145
	  PlayFX		NPCFX\Dance_Cloth.fx 161
	  PlayFX		NPCFX\Dance_Scuff.fx 177
	  PlayFX		NPCFX\Dance_Cloth.fx 219
	  PlayFX		NPCFX\Dance_Feet.fx 244
	  PlayFX		NPCFX\Dance_Feet.fx 249
	  PlayFX		NPCFX\Dance_Cloth.fx 268
	  PlayFX		NPCFX\Dance_Cloth.fx 271
	  PlayFX		NPCFX\Dance_Feet.fx 279
	  PlayFX		NPCFX\Dance_Feet.fx 281
	  PlayFX		NPCFX\Dance_Feet.fx 293
	  PlayFX		NPCFX\Dance_Feet.fx 295
	  PlayFX		NPCFX\Dance_Scuff.fx 328
	  PlayFX		NPCFX\Dance_Scuff.fx 351
	  PlayFX		NPCFX\Dance_Scuff.fx 367
	  PlayFX		NPCFX\Dance_Scuff.fx 387
	  PlayFX		NPCFX\Dance_Scuff.fx 406
	  PlayFX		NPCFX\Dance_Cloth.fx 407
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 423
	  PlayFX		NPCFX\Dance_Feet.fx 430
	  PlayFX		NPCFX\Dance_Feet.fx 432
	TEnd

	Type		fem
	  Anim          fem\emote_dance_billyjeans 2
	  PlayFX		NPCFX\Dance_Cloth.fx 3
	  PlayFX		NPCFX\Dance_Feet.fx 14
	  PlayFX		NPCFX\Dance_Feet.fx 19
	  PlayFX		NPCFX\Dance_ClothLong_SFX.fx 41
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 52
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 74
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 100
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 124
	  PlayFX		NPCFX\Dance_Scuff.fx 145
	  PlayFX		NPCFX\Dance_Cloth.fx 161
	  PlayFX		NPCFX\Dance_Scuff.fx 177
	  PlayFX		NPCFX\Dance_Cloth.fx 219
	  PlayFX		NPCFX\Dance_Feet.fx 244
	  PlayFX		NPCFX\Dance_Feet.fx 249
	  PlayFX		NPCFX\Dance_Cloth.fx 268
	  PlayFX		NPCFX\Dance_Cloth.fx 271
	  PlayFX		NPCFX\Dance_Feet.fx 279
	  PlayFX		NPCFX\Dance_Feet.fx 281
	  PlayFX		NPCFX\Dance_Feet.fx 293
	  PlayFX		NPCFX\Dance_Feet.fx 295
	  PlayFX		NPCFX\Dance_Scuff.fx 328
	  PlayFX		NPCFX\Dance_Scuff.fx 351
	  PlayFX		NPCFX\Dance_Scuff.fx 367
	  PlayFX		NPCFX\Dance_Scuff.fx 387
	  PlayFX		NPCFX\Dance_Scuff.fx 406
	  PlayFX		NPCFX\Dance_Cloth.fx 407
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 423
	  PlayFX		NPCFX\Dance_Feet.fx 430
	  PlayFX		NPCFX\Dance_Feet.fx 432
	TEnd

	Type		Huge
	  Anim          huge\emote_dance_billyjeans 2
	  PlayFX		NPCFX\Dance_Cloth.fx 3
	  PlayFX		NPCFX\Dance_Feet.fx 14
	  PlayFX		NPCFX\Dance_Feet.fx 19
	  PlayFX		NPCFX\Dance_ClothLong_SFX.fx 41
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 52
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 74
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 100
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 124
	  PlayFX		NPCFX\Dance_Scuff.fx 145
	  PlayFX		NPCFX\Dance_Cloth.fx 161
	  PlayFX		NPCFX\Dance_Scuff.fx 177
	  PlayFX		NPCFX\Dance_Cloth.fx 219
	  PlayFX		NPCFX\Dance_Feet.fx 244
	  PlayFX		NPCFX\Dance_Feet.fx 249
	  PlayFX		NPCFX\Dance_Cloth.fx 268
	  PlayFX		NPCFX\Dance_Cloth.fx 271
	  PlayFX		NPCFX\Dance_Feet.fx 279
	  PlayFX		NPCFX\Dance_Feet.fx 281
	  PlayFX		NPCFX\Dance_Feet.fx 293
	  PlayFX		NPCFX\Dance_Feet.fx 295
	  PlayFX		NPCFX\Dance_Scuff.fx 328
	  PlayFX		NPCFX\Dance_Scuff.fx 351
	  PlayFX		NPCFX\Dance_Scuff.fx 367
	  PlayFX		NPCFX\Dance_Scuff.fx 387
	  PlayFX		NPCFX\Dance_Scuff.fx 406
	  PlayFX		NPCFX\Dance_Cloth.fx 407
	  PlayFX		NPCFX\Dance_ClothShort_SFX.fx 423
	  PlayFX		NPCFX\Dance_Feet.fx 430
	  PlayFX		NPCFX\Dance_Feet.fx 432
	TEnd
MEnd
###########################################################