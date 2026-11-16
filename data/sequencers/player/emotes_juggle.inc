#############################################################
## Emotes.inc - Emotes
#############################################################
## Interrupts
## <IRQEmotesALL> - Other emotes, basic readies
## <IRQEmotesCombat> - Combat cycles
## <IRQEmotesRighthand> - Right-Handed weapon combat cycles
## <IRQEmotesRighthand> - Left-Handed weapon combat cycles
## <IRQEmotesRighthand> - Dual Wield weapon combat cycles
##
## Members
## <DeathIRQ> - All emotes should be a member of this group
## <AttackIRQ> - All emotes should be a member of this group
## <AttackIRQ> - All emotes should be a member of this group
## <MoveIRQ> - All emotes should be a member of this group
## <ReactIRQ> - Emotes should be a member of this group unless we specifically don't want them interruptible by being hit
## <HitIRQ> - Emotes should be a member of this group unless we specifically don't want them interruptible by being hit
## <IRQEmotesALL> - All emotes should be a member of this group UNLESS we want to restrict spamming them (ie. Howls, slaps, etc)

#############################################################
## Juggle Balls
#############################################################

Move Emote_Juggle
	Requires	EMOTE JUGGLE
	Priority        2
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Juggle_Cycle
	Flags		NoSizeScale FixWeaponReg
	Interpolate	5
	Scale		1.25

	Type		male
	  Anim          male\Magic_Juggle 5 64
	  PlayFX	Emotes\Juggle\Juggle_Balls_Male.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\Magic_Juggle 5 64
	  PlayFX	Emotes\Juggle\Juggle_Balls_Female.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\Magic_Juggle 5 64
	  PlayFX	Emotes\Juggle\Juggle_Balls_Huge.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Juggle_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	CycleMove	Emote_Juggle_Cycle2
	Flags		Cycle NoSizeScale FixWeaponReg
	SticksOnChild	EMOTE JUGGLE ONE
	Scale		1.25

	Type		male
	  Anim		male\Magic_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Balls_Male.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim		fem\Magic_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Balls_Female.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim		huge\Magic_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Balls_Huge.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Juggle_Cycle2
	Include sequencers\Members\Emotes_Idle.inc
	CycleMove	Emote_Juggle_Cycle
	Flags		Cycle NoSizeScale FixWeaponReg
	SticksOnChild	EMOTE JUGGLE TWO
	Scale		1.25

	Type		male
	  Anim		male\Magic_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Balls_Male.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim		fem\Magic_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Balls_Female.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim		huge\Magic_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Balls_Huge.fx 0 CONSTANT
	TEnd
MEnd

#############################################################

Move A_Emote_Juggle
	Requires	FLY EMOTE JUGGLE
	Priority        3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Air_Idle.inc
	NextMove	A_Emote_Juggle_Cycle
	Flags		NoSizeScale
	Interpolate	5
	Scale		1.25

	Type		male
	  Anim          male\Magic_Air_Juggle 5 64
	  PlayFX	Emotes\Juggle\Juggle_Balls_Male.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\Magic_Air_Juggle 5 64
	  PlayFX	Emotes\Juggle\Juggle_Balls_Female.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\Magic_Air_Juggle 5 64
	  PlayFX	Emotes\Juggle\Juggle_Balls_Huge.fx 0 CONSTANT
	TEnd
MEnd

Move A_Emote_Juggle_Cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Air_Idle.inc
	CycleMove	A_Emote_Juggle_Cycle2
	NextMove	Prefall
	Flags		Cycle NoSizeScale FixWeaponReg
	SticksOnChild	EMOTE JUGGLE ONE
	Scale		1.25

	Type		male
	  Anim		male\Magic_Air_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Balls_Male.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\Magic_Air_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Balls_Female.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\Magic_Air_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Balls_Huge.fx 0 CONSTANT
	TEnd
MEnd

Move A_Emote_Juggle_Cycle2
	Requires	FLY
	Include sequencers\Members\Emotes_Air_Idle.inc
	CycleMove	A_Emote_Juggle_Cycle
	NextMove	Prefall
	Flags		Cycle NoSizeScale FixWeaponReg
	SticksOnChild	EMOTE JUGGLE TWO
	Scale		1.25

	Type		male
	  Anim		male\Magic_Air_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Balls_Male.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\Magic_Air_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Balls_Female.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\Magic_Air_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Balls_Huge.fx 0 CONSTANT
	TEnd
MEnd

#############################################################
## Juggle Magic
#############################################################

Move Emote_Juggle_Magic
	Requires	EMOTE JUGGLE MAGIC
	Priority        3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Juggle_Magic_Cycle
	Flags		NoSizeScale
	Interpolate	15
	Scale		1.25

	Type		male
	  Anim          male\Magic_Juggle 5 64
	  PlayFX	Emotes\Juggle\Juggle_Magic_Male.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\Magic_Juggle 5 64
	  PlayFX	Emotes\Juggle\Juggle_Magic_Female.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\Magic_Juggle 5 64
	  PlayFX	Emotes\Juggle\Juggle_Magic_Huge.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Juggle_Magic_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	CycleMove	Emote_Juggle_Magic_Cycle2
	Flags		Cycle NoSizeScale FixWeaponReg
	SticksOnChild	EMOTE JUGGLE ONE
	Scale		1.25

	Type		male
	  Anim		male\Magic_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Magic_Male.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim		fem\Magic_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Magic_Female.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim		huge\Magic_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Magic_Huge.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Juggle_Magic_Cycle2
	Include sequencers\Members\Emotes_Idle.inc
	CycleMove	Emote_Juggle_Magic_Cycle
	Flags		Cycle NoSizeScale FixWeaponReg
	SticksOnChild	EMOTE JUGGLE TWO
	Scale		1.25

	Type		male
	  Anim		male\Magic_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Magic_Male.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim		fem\Magic_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Magic_Female.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim		huge\Magic_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Magic_Huge.fx 0 CONSTANT
	TEnd
MEnd

#############################################################

Move A_Emote_Juggle_Magic
	Requires	FLY EMOTE JUGGLE MAGIC
	Priority        4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Air_Idle.inc
	NextMove	A_Emote_Juggle_Magic_Cycle
	Flags		NoSizeScale
	Interpolate	15
	Scale		1.25

	Type		male
	  Anim          male\Magic_Air_Juggle 5 64
	  PlayFX	Emotes\Juggle\Juggle_Magic_Male.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\Magic_Air_Juggle 5 64
	  PlayFX	Emotes\Juggle\Juggle_Magic_Female.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\Magic_Air_Juggle 5 64
	  PlayFX	Emotes\Juggle\Juggle_Magic_Huge.fx 0 CONSTANT
	TEnd
MEnd

Move A_Emote_Juggle_Magic_Cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Air_Idle.inc
	CycleMove	A_Emote_Juggle_Magic_Cycle2
	NextMove	Prefall
	Flags		Cycle NoSizeScale FixWeaponReg
	SticksOnChild	EMOTE JUGGLE ONE
	Scale		1.25

	Type		male
	  Anim		male\Magic_Air_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Magic_Male.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim		fem\Magic_Air_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Magic_Female.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim		huge\Magic_Air_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Magic_Huge.fx 0 CONSTANT
	TEnd
MEnd

Move A_Emote_Juggle_Magic_Cycle2
	Requires	FLY
	Include sequencers\Members\Emotes_Air_Idle.inc
	CycleMove	A_Emote_Juggle_Magic_Cycle
	NextMove	Prefall
	Flags		Cycle NoSizeScale FixWeaponReg
	SticksOnChild	EMOTE JUGGLE TWO
	Scale		1.25

	Type		male
	  Anim		male\Magic_Air_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Magic_Male.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim		fem\Magic_Air_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Magic_Female.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim		huge\Magic_Air_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Magic_Huge.fx 0 CONSTANT
	TEnd
MEnd

#############################################################
## Juggle Fire
#############################################################

Move Emote_Juggle_Fire
	Requires	EMOTE JUGGLE FIRE
	Priority        3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Juggle_Fire_Cycle
	Flags		NoSizeScale
	Interpolate	15
	Scale		1.25

	Type		male
	  Anim          male\Magic_Juggle 5 64
	  PlayFX	Emotes\Juggle\Juggle_Fire_Male.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\Magic_Juggle 5 64
	  PlayFX	Emotes\Juggle\Juggle_Fire_Female.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\Magic_Juggle 5 64
	  PlayFX	Emotes\Juggle\Juggle_Fire_Huge.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Juggle_Fire_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	CycleMove	Emote_Juggle_Fire_Cycle2
	Flags		Cycle NoSizeScale FixWeaponReg
	SticksOnChild	EMOTE JUGGLE ONE
	Scale		1.25

	Type		male
	  Anim		male\Magic_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Fire_Male.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim		fem\Magic_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Fire_Female.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim		huge\Magic_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Fire_Huge.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Juggle_Fire_Cycle2
	Include sequencers\Members\Emotes_Idle.inc
	CycleMove	Emote_Juggle_Fire_Cycle
	Flags		Cycle NoSizeScale FixWeaponReg
	SticksOnChild	EMOTE JUGGLE TWO
	Scale		1.25

	Type		male
	  Anim		male\Magic_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Fire_Male.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim		fem\Magic_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Fire_Female.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim		huge\Magic_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Fire_Huge.fx 0 CONSTANT
	TEnd
MEnd

#############################################################

Move A_Emote_Juggle_Fire
	Requires	FLY EMOTE JUGGLE FIRE
	Priority        4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Air_Idle.inc
	NextMove	A_Emote_Juggle_Fire_Cycle
	Flags		NoSizeScale
	Interpolate	15
	Scale		1.25

	Type		male
	  Anim          male\Magic_Air_Juggle 5 64
	  PlayFX	Emotes\Juggle\Juggle_Fire_Male.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\Magic_Air_Juggle 5 64
	  PlayFX	Emotes\Juggle\Juggle_Fire_Female.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\Magic_Air_Juggle 5 64
	  PlayFX	Emotes\Juggle\Juggle_Fire_Huge.fx 0 CONSTANT
	TEnd
MEnd

Move A_Emote_Juggle_Fire_Cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Air_Idle.inc
	CycleMove	A_Emote_Juggle_Fire_Cycle2
	NextMove	Prefall
	Flags		Cycle NoSizeScale FixWeaponReg
	SticksOnChild	EMOTE JUGGLE ONE
	Scale		1.25

	Type		male
	  Anim		male\Magic_Air_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Fire_Male.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim		fem\Magic_Air_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Fire_Female.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim		huge\Magic_Air_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Fire_Huge.fx 0 CONSTANT
	TEnd
MEnd

Move A_Emote_Juggle_Fire_Cycle2
	Requires	FLY
	Include sequencers\Members\Emotes_Air_Idle.inc
	CycleMove	A_Emote_Juggle_Fire_Cycle
	NextMove	Prefall
	Flags		Cycle NoSizeScale FixWeaponReg
	SticksOnChild	EMOTE JUGGLE TWO
	Scale		1.25

	Type		male
	  Anim		male\Magic_Air_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Fire_Male.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim		fem\Magic_Air_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Fire_Female.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim		huge\Magic_Air_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Fire_Huge.fx 0 CONSTANT
	TEnd
MEnd

#############################################################
## Juggle Electricity
#############################################################

Move Emote_Juggle_Electric
	Requires	EMOTE JUGGLE ELECTRICITY
	Priority        3
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Juggle_Electric_Cycle
	Flags		NoSizeScale
	Interpolate	15
	Scale		1.25

	Type		male
	  Anim          male\Magic_Juggle 5 64
	  PlayFX	Emotes\Juggle\Juggle_Electric_Male.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\Magic_Juggle 5 64
	  PlayFX	Emotes\Juggle\Juggle_Electric_Female.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\Magic_Juggle 5 64
	  PlayFX	Emotes\Juggle\Juggle_Electric_Huge.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Juggle_Electric_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	CycleMove	Emote_Juggle_Electric_Cycle2
	Flags		Cycle NoSizeScale FixWeaponReg
	SticksOnChild	EMOTE JUGGLE ONE
	Scale		1.25

	Type		male
	  Anim		male\Magic_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Electric_Male.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim		fem\Magic_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Electric_Female.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim		huge\Magic_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Electric_Huge.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Juggle_Electric_Cycle2
	Include sequencers\Members\Emotes_Idle.inc
	CycleMove	Emote_Juggle_Electric_Cycle
	Flags		Cycle NoSizeScale FixWeaponReg
	SticksOnChild	EMOTE JUGGLE TWO
	Scale		1.25

	Type		male
	  Anim		male\Magic_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Electric_Male.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim		fem\Magic_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Electric_Female.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim		huge\Magic_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Electric_Huge.fx 0 CONSTANT
	TEnd
MEnd

#############################################################

Move A_Emote_Juggle_Electric
	Requires	FLY EMOTE JUGGLE ELECTRICITY
	Priority        4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand>
	Include sequencers\Members\Emotes_Air_Idle.inc
	NextMove	A_Emote_Juggle_Electric_Cycle
	Flags		NoSizeScale
	Interpolate	15
	Scale		1.25

	Type		male
	  Anim          male\Magic_Air_Juggle 5 64
	  PlayFX	Emotes\Juggle\Juggle_Electric_Male.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\Magic_Air_Juggle 5 64
	  PlayFX	Emotes\Juggle\Juggle_Electric_Female.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\Magic_Air_Juggle 5 64
	  PlayFX	Emotes\Juggle\Juggle_Electric_Huge.fx 0 CONSTANT
	TEnd
MEnd

Move A_Emote_Juggle_Electric_Cycle
	Requires	FLY
	Include sequencers\Members\Emotes_Air_Idle.inc
	CycleMove	A_Emote_Juggle_Electric_Cycle2
	NextMove	Prefall
	Flags		Cycle NoSizeScale FixWeaponReg
	SticksOnChild	EMOTE JUGGLE ONE
	Scale		1.25

	Type		male
	  Anim		male\Magic_Air_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Electric_Male.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim		fem\Magic_Air_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Electric_Female.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim		huge\Magic_Air_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Electric_Huge.fx 0 CONSTANT
	TEnd
MEnd

Move A_Emote_Juggle_Electric_Cycle2
	Requires	FLY
	Include sequencers\Members\Emotes_Air_Idle.inc
	CycleMove	A_Emote_Juggle_Electric_Cycle
	NextMove	Prefall
	Flags		Cycle NoSizeScale FixWeaponReg
	SticksOnChild	EMOTE JUGGLE TWO
	Scale		1.25

	Type		male
	  Anim		male\Magic_Air_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Electric_Male.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim		fem\Magic_Air_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Electric_Female.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim		huge\Magic_Air_Juggle 65 161
	  PlayFX	Emotes\Juggle\Juggle_Electric_Huge.fx 0 CONSTANT
	TEnd
MEnd