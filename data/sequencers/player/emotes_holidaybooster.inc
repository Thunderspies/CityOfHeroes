#############################################################
## emotes_HolidayBooster.inc
#############################################################

##  Gift Burst ###########################################################

Move Emote_Transform_Gift_Pre
	Requires	EMOTE CCCHANGE GIFT STAND TWO
	Priority        5
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove	Emote_Transform_Gift
	Flags		NoSizeScale cantmove

	Type		male
	  Anim          male\emote_gift_burst 1 23
	  PlayFx       "Emotes\CostumeChanges\GiftBurst\Gift_Prop.fx" 0
	TEnd

	Type		fem
	  Anim          fem\emote_gift_burst 1 23
	  PlayFx       "Emotes\CostumeChanges\GiftBurst\Gift_Prop.fx" 0
	TEnd

MEnd

Move Emote_Transform_Gift
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Emote_Transform_Gift_Post
	Flags		NoSizeScale Cantmove
	Sets		COSTUMECHANGE

	Type		male
	  Anim          male\emote_gift_burst 21 33
	  PlayFx        "GENERIC\ENTITYVANISH.fx" 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_gift_burst 21 33
	  PlayFx        "GENERIC\ENTITYVANISH.fx" 0 CONSTANT
	TEnd

MEnd

Move Emote_Transform_Gift_Post
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Ready
	Flags		NoSizeScale
	Sets		COSTUMECHANGE

	Type		male
	  Anim          male\emote_gift_burst 33
	playfx		"Emotes\CostumeChanges\GiftBurst\GiftExplosion.fx" 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_gift_burst 33
	playfx		"Emotes\CostumeChanges\GiftBurst\GiftExplosion.fx" 0 CONSTANT
	TEnd

MEnd

## AIR Gift Burst###########################################################

Move A_Emote_Transform_Gift_Pre
	Requires	FLY EMOTE CCCHANGE GIFT STAND TWO
	Priority        16
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove	A_Emote_Transform_Gift
	Flags		NoSizeScale CANTMOVE

	Type		male
	  Anim          male\air_emote_gift_burst 1 22
	  PlayFx       "Events\Winter\Gift_Burst1.fx" 0
	TEnd

MEnd

Move A_Emote_Transform_Gift
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove        A_Emote_Transform_Gift_Post
	Flags		NoSizeScale Cantmove
	Sets		COSTUMECHANGE

	Type		male
	  Anim          male\air_emote_gift_burst 22 33
	  PlayFx        GENERIC\ENTITYVANISH.fx 0 CONSTANT
	TEnd

MEnd

Move A_Emote_Transform_Gift_Post
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove        FlyReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_emote_gift_burst 33
	playfx		"Explosions\Confetti\GiftExplosion.fx" 0 CONSTANT
	TEnd

MEnd

##  Confetti Throw  ###########################################################

Move Emote_Transform_Confetti_Pre
	Requires	EMOTE CCCHANGE GIFT STAND ONE
	Priority        5
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove	Emote_Transform_Confetti_Post
	Flags		NoSizeScale cantmove

	Type		male
	  Anim          male\emote_confetti_throw 1 22
	playfx		"Emotes\CostumeChanges\ConfettiThrow\ConfettiThrow.fx" 0
	TEnd

	Type		fem
	  Anim          fem\emote_confetti_throw 1 22
	playfx		"Emotes\CostumeChanges\ConfettiThrow\ConfettiThrow.fx" 0
	TEnd

	Type		huge
	  Anim          huge\emote_confetti_throw 1 22
	playfx		"Emotes\CostumeChanges\ConfettiThrow\ConfettiThrow.fx" 0
	TEnd

MEnd

Move Emote_Transform_Confetti_Post
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Ready
	Flags		NoSizeScale
	Sets		COSTUMECHANGE

	Type		male
	  Anim          male\emote_confetti_throw 22
	playfx		"Explosions\Confetti\XmasConfettiExplosion.fx" 0
	TEnd

	Type		fem
	  Anim          fem\emote_confetti_throw 22
	playfx		"Explosions\Confetti\XmasConfettiExplosion.fx" 0
	TEnd

	Type		huge
	  Anim          huge\emote_confetti_throw 22
	playfx		"Explosions\Confetti\XmasConfettiExplosion.fx" 0
	TEnd

MEnd

## AIR Confetti Throw  ###########################################################

Move A_Emote_Transform_Confetti_Pre
	Requires	FLY EMOTE CCCHANGE GIFT STAND ONE
	Priority        16
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove	A_Emote_Transform_Confetti_Post
	Flags		NoSizeScale CANTMOVE

	Type		male
	  Anim          male\air_emote_confetti_throw 1 22
	playfx		"Emotes\CostumeChanges\ConfettiThrow\ConfettiThrow.fx" 0
	TEnd

	Type		huge
	  Anim          huge\air_emote_confetti_throw 1 22
	playfx		"Emotes\CostumeChanges\ConfettiThrow\ConfettiThrow.fx" 0
	TEnd

MEnd

Move A_Emote_Transform_Confetti_Post
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove        FlyReady
	Flags		NoSizeScale
	Sets		COSTUMECHANGE

	Type		male
	  Anim          male\air_emote_confetti_throw 22
	playfx		"Explosions\Confetti\XmasConfettiExplosion.fx" 0
	TEnd

	Type		huge
	  Anim          huge\air_emote_confetti_throw 22
	playfx		"Explosions\Confetti\XmasConfettiExplosion.fx" 0
	TEnd

MEnd

## Emote Offer Gift ###########################################################

Move Emote_Offer_Gift_Pre
	Requires	EMOTE GIFT ONE
	Priority        3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Offer_Gift_One
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_offergift 1 14
	Playfx		Events\Winter\Offer_Gift1.fx 0 constant
	TEnd

	Type		huge
	  Anim          huge\emote_offergift 1 14
	Playfx		Events\Winter\Offer_Gift1.fx 0 constant
	TEnd

	Type		fem
	  Anim          fem\emote_offergift 1 14
	Playfx		Events\Winter\Offer_Gift1.fx 0 constant
	TEnd

MEnd

Move Emote_Offer_Gift_One
	Include sequencers\Members\Emotes_Idle.inc
	Flags		NoSizeScale
	NextMove	Emote_Offer_Gift_Two

	Type		male
	  Anim          male\emote_offergift 14 37
	Playfx		Events\Winter\Offer_Gift1.fx 0 constant
	TEnd

	Type		huge
	  Anim          huge\emote_offergift 14 37
	Playfx		Events\Winter\Offer_Gift1.fx 0 constant
	TEnd

	Type		fem
	  Anim          fem\emote_offergift 14 37
	Playfx		Events\Winter\Offer_Gift1.fx 0 constant
	TEnd

MEnd

Move Emote_Offer_Gift_Two
	Include sequencers\Members\Emotes_Idle.inc
	Flags		NoSizeScale
	NextMove	Emote_Offer_Gift_Post

	Type		male
	  Anim          male\emote_offergift 37 52
	Playfx		Events\Winter\Offer_Gift1.fx 0 constant
	TEnd

	Type		huge
	  Anim          huge\emote_offergift 37 52
	Playfx		Events\Winter\Offer_Gift1.fx 0 constant
	TEnd

	Type		fem
	  Anim          fem\emote_offergift 37 52
	Playfx		Events\Winter\Offer_Gift1.fx 0 constant
	TEnd

MEnd

Move Emote_Offer_Gift_Post
	Include sequencers\Members\Emotes_Idle.inc
	Flags		NoSizeScale
	NextMove	Ready

	Type		male
	  Anim          male\emote_offergift 52 78
	TEnd

	Type		huge
	  Anim          huge\emote_offergift 52 78
	TEnd

	Type		fem
	  Anim          fem\emote_offergift 52 78
	TEnd
MEnd

## AIR Emote Offer Gift ###########################################################

Move A_Emote_Offer_Gift_Pre
	Requires	FLY EMOTE GIFT ONE
	Priority        4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Offer_Gift_One
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_emote_offergift 1 14
	Playfx		Events\Winter\Offer_Gift1.fx 0 constant
	TEnd

	Type		huge
	  Anim          huge\air_emote_offergift 1 14
	Playfx		Events\Winter\Offer_Gift1.fx 0 constant
	TEnd

	Type		fem
	  Anim          fem\air_emote_offergift 1 14
	Playfx		Events\Winter\Offer_Gift1.fx 0 constant
	TEnd

MEnd

Move A_Emote_Offer_Gift_One
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		NoSizeScale
	NextMove	A_Emote_Offer_Gift_Two

	Type		male
	  Anim          male\air_emote_offergift 14 37
	Playfx		Events\Winter\Offer_Gift1.fx 0 constant
	TEnd

	Type		huge
	  Anim          huge\air_emote_offergift 14 37
	Playfx		Events\Winter\Offer_Gift1.fx 0 constant
	TEnd

	Type		fem
	  Anim          fem\air_emote_offergift 14 37
	Playfx		Events\Winter\Offer_Gift1.fx 0 constant
	TEnd

MEnd

Move A_Emote_Offer_Gift_Two
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		NoSizeScale
	NextMove	A_Emote_Offer_Gift_Post

	Type		male
	  Anim          male\air_emote_offergift 37 52
	Playfx		Events\Winter\Offer_Gift1.fx 0 constant
	TEnd

	Type		huge
	  Anim          huge\air_emote_offergift 37 52
	Playfx		Events\Winter\Offer_Gift1.fx 0 constant
	TEnd

	Type		fem
	  Anim          fem\air_emote_offergift 37 52
	Playfx		Events\Winter\Offer_Gift1.fx 0 constant
	TEnd

MEnd

Move A_Emote_Offer_Gift_Post
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		NoSizeScale
	NextMove	FlyReady

	Type		male
	  Anim          male\air_emote_offergift 52 78
	TEnd

	Type		huge
	  Anim          huge\air_emote_offergift 52 78
	TEnd

	Type		fem
	  Anim          fem\air_emote_offergift 52 78
	TEnd

MEnd

## Emote Receive Gift ###########################################################

Move Emote_Receive_Gift_Pre
	Requires	EMOTE GIFT TWO
	Priority        3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Receive_Gift
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_receivegift 1 40
	TEnd

	Type		huge
	  Anim          huge\emote_receivegift 1 40
	TEnd

	Type		fem
	  Anim          fem\emote_receivegift 1 40
	TEnd


MEnd

Move Emote_Receive_Gift
	Include sequencers\Members\Emotes_Idle.inc
	Flags		NoSizeScale
	NextMove	Emote_Receive_Gift_Post

	Type		male
	  Anim          male\emote_receivegift 40 57
	Playfx		Events\Winter\Receive_Gift1.fx 0 constant
	TEnd

	Type		huge
	  Anim          huge\emote_receivegift 40 57
	Playfx		Events\Winter\Receive_Gift1.fx 0 constant
	TEnd

	Type		fem
	  Anim          fem\emote_receivegift 40 57
	Playfx		Events\Winter\Receive_Gift1.fx 0 constant
	TEnd

MEnd

Move Emote_Receive_Gift_Post
	Include sequencers\Members\Emotes_Idle.inc
	Flags		NoSizeScale
	NextMove	Emote_Receive_Gift_Post_One

	Type		male
	  Anim          male\emote_receivegift 57 81
	Playfx		Events\Winter\Receive_Gift1.fx 0 constant
	TEnd

	Type		huge
	  Anim          huge\emote_receivegift 57 81
	Playfx		Events\Winter\Receive_Gift1.fx 0 constant
	TEnd

	Type		fem
	  Anim          fem\emote_receivegift 57 81
	Playfx		Events\Winter\Receive_Gift1.fx 0 constant
	TEnd

MEnd

Move Emote_Receive_Gift_Post_One
	Include sequencers\Members\Emotes_Idle.inc
	Flags		NoSizeScale
	NextMove	Ready

	Type		male
	  Anim          male\emote_receivegift 81
	TEnd

	Type		huge
	  Anim          huge\emote_receivegift 81
	TEnd

	Type		fem
	  Anim          fem\emote_receivegift 81
	TEnd

MEnd

## AIR Emote Receive Gift ###########################################################

Move A_Emote_Receive_Gift_Pre
	Requires	FLY EMOTE GIFT TWO
	Priority        4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Receive_Gift
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_emote_receivegift 1 40
	TEnd

	Type		huge
	  Anim          huge\air_emote_receivegift 1 40
	TEnd

	Type		fem
	  Anim          fem\air_emote_receivegift 1 40
	TEnd

MEnd

Move A_Emote_Receive_Gift
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		NoSizeScale
	NextMove	A_Emote_Receive_Gift_Post

	Type		male
	  Anim          male\air_emote_receivegift 40 57
	Playfx		Events\Winter\Receive_Gift1.fx 0 constant
	TEnd

	Type		huge
	  Anim          huge\air_emote_receivegift 40 57
	Playfx		Events\Winter\Receive_Gift1.fx 0 constant
	TEnd

	Type		fem
	  Anim          fem\air_emote_receivegift 40 57
	Playfx		Events\Winter\Receive_Gift1.fx 0 constant
	TEnd

MEnd

Move A_Emote_Receive_Gift_Post
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		NoSizeScale
	NextMove	A_Emote_Receive_Gift_Post_One

	Type		male
	  Anim          male\air_emote_receivegift 57 81
	Playfx		Events\Winter\Receive_Gift1.fx 0 constant
	TEnd

	Type		huge
	  Anim          huge\air_emote_receivegift 57 81
	Playfx		Events\Winter\Receive_Gift1.fx 0 constant
	TEnd

	Type		fem
	  Anim          fem\air_emote_receivegift 57 81
	Playfx		Events\Winter\Receive_Gift1.fx 0 constant
	TEnd

MEnd

Move A_Emote_Receive_Gift_Post_One
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		NoSizeScale
	NextMove	FlyReady

	Type		male
	  Anim          male\air_emote_receivegift 81
	TEnd

	Type		huge
	  Anim          huge\air_emote_receivegift 81
	TEnd

	Type		fem
	  Anim          fem\air_emote_receivegift 81
	TEnd

MEnd

###########################################################