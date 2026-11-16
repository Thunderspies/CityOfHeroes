#############################################################
## Stance - Ghost Widow's signature stance
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
## Stance - Ghost Widow's signature stance
#############################################################

Move Stance_GhostWidow
	Requires	EMOTE POSE WIDOWSATTACK
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand>
	Member		<DeathIRQ> <AttackIRQ>  <MoveIRQ> <ReactIRQ> <HitIRQ> <IRQEmotesALL>
	Flags		NoSizeScale
	Priority        2
	NextMove	Stance_GhostWidow_Cycle

	Type		male
	  Anim          fem\widows_attack 90
	TEnd
MEnd

Move Stance_GhostWidow_Cycle
	Member		<DeathIRQ> <AttackIRQ>  <MoveIRQ> <ReactIRQ> <HitIRQ> <IRQEmotesALL>
	Flags		NoSizeScale Cycle
	Priority        1

	Type		male
	  Anim          fem\widows_attack 90
	TEnd
MEnd

#############################################################

Move A_Stance_GhostWidow
	Requires	EMOTE POSE WIDOWSATTACK FLY
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand>
	Member		<DeathIRQ> <AttackIRQ>  <MoveIRQ> <ReactIRQ> <HitIRQ> <IRQEmotesALL>
	Flags		NoSizeScale
	Priority        3
	NextMove	A_Stance_GhostWidow_Cycle

	Type		male
	  Anim          fem\widows_attack 90
	TEnd
MEnd

Move A_Stance_GhostWidow_Cycle
	Member		<DeathIRQ> <AttackIRQ>  <MoveIRQ> <ReactIRQ> <HitIRQ> <IRQEmotesALL>
	Flags		NoSizeScale Cycle
	Priority        1

	Type		male
	  Anim          fem\widows_attack 90
	TEnd
MEnd

#############################################################
## Stance - Heroic 1
#############################################################

Move Emote_Stance_Heroic01
	Requires	EMOTE POSE GOOD ONE
	Priority	4
	Interrupts	<Ready> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Member		<DeathIRQ> <AttackIRQ>  <Jumps> <MoveIRQ> <ReactIRQ> <HitIRQ> <IRQEmotesALL>
	NextMove	Emote_Stance_Heroic01_Cycle
	Scale		0.5
	Interpolate	5
	Flags		FIXARMREG

	Type		male
	  Anim          male\Stance_Heroic01 1
	TEnd

	Type		fem
	  Anim          fem\Stance_Heroic01 1
	TEnd
MEnd

Move Emote_Stance_Heroic01_Cycle
	Member		<DeathIRQ> <AttackIRQ>  <Jumps> <MoveIRQ> <ReactIRQ> <HitIRQ> <IRQEmotesALL>
	Scale		0.5
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\Stance_Heroic01 1
	TEnd

	Type		fem
	  Anim          fem\Stance_Heroic01 1
	TEnd
MEnd

#############################################################

Move A_Emote_Stance_Heroic01
	Requires	FLY EMOTE POSE GOOD ONE
	Priority	5
	Interrupts	<Ready> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Member		<IRQEmotesALL> <DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <MoveIRQ> #<FlyReady> #<Block>
	NextMove	A_Emote_Stance_Heroic01_Cycle
	Scale		0.5
	Interpolate	5
	Flags		FIXARMREG

	Type		male
	  Anim          male\air_Stance_Heroic01 1
	TEnd

	Type		fem
	  Anim          fem\air_Stance_Heroic01 1
	TEnd

	Type		huge
	  Anim          huge\air_Stance_Heroic01 1
	TEnd
MEnd

Move A_Emote_Stance_Heroic01_Cycle
	Requires	FLY
	Priority        1
	Member		<IRQEmotesALL> <DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <MoveIRQ> #<FlyReady> #<Block>
	NextMove	PreFall
	Scale		0.5
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\air_Stance_Heroic01 1
	TEnd

	Type		fem
	  Anim          fem\air_Stance_Heroic01 1
	TEnd

	Type		huge
	  Anim          huge\air_Stance_Heroic01 1
	TEnd
MEnd

#############################################################
## Stance - Heroic 2
#############################################################

Move Emote_Stance_Heroic02
	Requires	EMOTE POSE GOOD TWO
	Priority	4
	Interrupts	<Ready> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Member		<DeathIRQ> <AttackIRQ>  <Jumps> <MoveIRQ> <ReactIRQ> <HitIRQ> <IRQEmotesALL>
	NextMove	Emote_Stance_Heroic02_Cycle
	Scale		0.5
	Interpolate	5
	Flags		FIXARMREG

	Type		male
	  Anim          male\Stance_Heroic02 1
	TEnd

	Type		fem
	  Anim          fem\Stance_Heroic02 1
	TEnd
MEnd

Move Emote_Stance_Heroic02_Cycle
	Member		<DeathIRQ> <AttackIRQ>  <Jumps> <MoveIRQ> <ReactIRQ> <HitIRQ> <IRQEmotesALL>
	Scale		0.5
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\Stance_Heroic02 1
	TEnd

	Type		fem
	  Anim          fem\Stance_Heroic02 1
	TEnd
MEnd

#############################################################

Move A_Emote_Stance_Heroic02
	Requires	FLY EMOTE POSE GOOD TWO
	Priority	5
	Interrupts	<Ready> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Member		<IRQEmotesALL> <DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <MoveIRQ> #<FlyReady> #<Block>
	NextMove	A_Emote_Stance_Heroic02_Cycle
	Scale		0.5
	Interpolate	5
	Flags		FIXARMREG

	Type		male
	  Anim          male\air_Stance_Heroic02 1
	TEnd
MEnd

Move A_Emote_Stance_Heroic02_Cycle
	Requires	FLY
	Priority        1
	Member		<IRQEmotesALL> <DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <MoveIRQ> #<FlyReady> #<Block>
	NextMove	PreFall
	Scale		0.5
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\air_Stance_Heroic02 1
	TEnd
MEnd


#############################################################
## Stance - Villainous 1
#############################################################

Move Emote_Stance_Villainous01
	Requires	EMOTE POSE EVIL ONE
	Priority	4
	Interrupts	<Ready> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Member		<DeathIRQ> <AttackIRQ>  <Jumps> <MoveIRQ> <ReactIRQ> <HitIRQ> <IRQEmotesALL>
	NextMove	Emote_Stance_Villainous01_Cycle
	Scale		0.75
	Interpolate	5
	Flags		FIXARMREG

	Type		male
	  Anim          male\Stance_Villainous01 1
	TEnd
MEnd

Move Emote_Stance_Villainous01_Cycle
	Member		<DeathIRQ> <AttackIRQ>  <Jumps> <MoveIRQ> <ReactIRQ> <HitIRQ> <IRQEmotesALL>
	Scale		0.75
	NextMove	PreFall
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\Stance_Villainous01 1
	TEnd
MEnd

#############################################################

Move A_Emote_Stance_Omega
	Requires	FLY EMOTE POSE EVIL ONE
	Priority	5
	Interrupts	<Ready> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Member		<IRQEmotesALL> <DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <MoveIRQ> #<FlyReady> #<Block>
	NextMove	A_Emote_Stance_Omega_Cycle
	Scale		0.625
	Interpolate	5
	Flags		FIXARMREG

	Type		male
	  Anim          male\air_Stance_Villainous01 1
	TEnd
MEnd

Move A_Emote_Stance_Omega_Cycle
	Requires	FLY
	Priority        1
	Member		<IRQEmotesALL> <DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <MoveIRQ> #<FlyReady> #<Block>
	NextMove	PreFall
	Scale		0.625
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\air_Stance_Villainous01 1
	TEnd
MEnd

#############################################################
## Stance - Villainous 2
#############################################################

Move Emote_Stance_Villainous02
	Requires	EMOTE POSE EVIL TWO
	Priority	4
	Interrupts	<Ready> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Member		<DeathIRQ> <AttackIRQ>  <Jumps> <MoveIRQ> <ReactIRQ> <HitIRQ> <IRQEmotesALL>
	NextMove	Emote_Stance_Villainous02_Cycle
	Scale		0.5
	Interpolate	5
	Flags		FIXARMREG

	Type		male
	  Anim          male\Stance_Villainous02 1
	TEnd

	Type		fem
	  Anim          fem\Stance_Villainous02 1
	TEnd

	Type		huge
	  Anim          huge\Stance_Villainous02 1
	TEnd
MEnd

Move Emote_Stance_Villainous02_Cycle
	Member		<DeathIRQ> <AttackIRQ>  <Jumps> <MoveIRQ> <ReactIRQ> <HitIRQ> <IRQEmotesALL>
	Scale		0.5
	NextMove	PreFall
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\Stance_Villainous02 1
	TEnd

	Type		fem
	  Anim          fem\Stance_Villainous02 1
	TEnd

	Type		huge
	  Anim          huge\Stance_Villainous02 1
	TEnd
MEnd

#############################################################

Move A_Emote_Stance_Villainous02
	Requires	FLY EMOTE POSE EVIL TWO
	Priority	5
	Interrupts	<Ready> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Member		<IRQEmotesALL> <DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <MoveIRQ> #<FlyReady> #<Block>
	NextMove	A_Emote_Stance_Villainous02_Cycle
	Scale		0.625
	Interpolate	5
	Flags		FIXARMREG

	Type		male
	  Anim          male\air_Stance_Villainous02 1
	TEnd

	Type		fem
	  Anim          fem\air_Stance_Villainous02 1
	TEnd

	Type		huge
	  Anim          huge\air_Stance_Villainous02 1
	TEnd
MEnd

Move A_Emote_Stance_Villainous02_Cycle
	Requires	FLY
	Priority        1
	Member		<IRQEmotesALL> <DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <MoveIRQ> #<FlyReady> #<Block>
	NextMove	PreFall
	Scale		0.625
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\air_Stance_Villainous02 1
	TEnd

	Type		fem
	  Anim          fem\air_Stance_Villainous02 1
	TEnd

	Type		huge
	  Anim          huge\air_Stance_Villainous02 1
	TEnd
MEnd