#############################################################
## Weapons_Whip_a.inc
#############################################################

### DRAW WHIP #######################################################
#
#Move A_Draw_Whip
#	Requires	FLY COMBAT WHIP SPAWNSIDE
#	Priority        15
#	Interrupts	<AttackIRQ>
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <MoveIRQ> <IRQEmotesRighthand>
#	NextMove        WepReady
#	Flags		NOSIZESCALE
#	Scale		1.25
#
#	Type		male
#	  Anim          male\air_draw_saber 1
#	TEnd
#MEnd

### AIR WHIP CORRUPTION #######################################################

Move A_Whip_Attack1
	Requires	FLY COMBAT WHIP ATTACK LOW
	Priority        15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Whip_Attack1_Post
	Sets            CANTMOVE
	Flags		NoSizeScale
	SticksOnChild	FLY COMBAT WEAPON WHIP ATTACK LOW
	Interpolate	0

	Type		male
	  Anim          male\air_DS_whip_corruption 1 38
	TEnd
MEnd

Move A_Whip_Attack1_Post
	Include sequencers\Members\Combat_Air.inc
	NextMove        Lazy_C_flycycle_Blend
	Flags		NoSizeScale
	Interpolate	0

	Type		male
	  Anim          male\air_DS_whip_corruption 38
	TEnd
MEnd


### AIR WHIP LASH #######################################################

Move A_Whip_Attack2
	Requires	FLY COMBAT WHIP ATTACK HIGH
	Priority        15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Whip_Attack2_Post
	Sets            CANTMOVE
	Flags		NoSizeScale
	SticksOnChild	FLY COMBAT WEAPON WHIP ATTACK HIGH

	Type		male
	  Anim          male\air_DS_whip_lash 1 55
	TEnd
MEnd

Move A_Whip_Attack2_Post
	Include sequencers\Members\Combat_Air.inc
	NextMove        Lazy_C_flycycle_Blend
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_DS_whip_lash 55
	TEnd
MEnd

### AIR WHIP STRIKE #######################################################

Move A_Whip_Attack3
	Requires	FLY COMBAT WHIP ATTACK CONE
	Priority        15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Whip_Attack3_Post
	Sets            CANTMOVE
	SticksOnChild	FLY COMBAT WEAPON WHIP ATTACK CONE
	Scale		2

	Type		male
	  Anim          male\air_DS_whip_strike 1 140
	TEnd
MEnd

Move A_Whip_Attack3_Post
	Include sequencers\Members\Combat_Air.inc
	NextMove        Lazy_C_flycycle_Blend
	Flags		NoSizeScale
	Scale		2

	Type		male
	  Anim          male\air_DS_whip_strike 140
	TEnd
MEnd

### AIR DEMON SUMMON #######################################################

Move A_Whip_Summon
	Requires	FLY COMBAT DEMONSUMMON
	Priority        13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Whip_Summon_Post
	Sets            CANTMOVE
	Flags		NoSizeScale
	SticksOnChild	FLY COMBAT WEAPON WHIP ATTACK DEMONSUMMON
	Interpolate	0

	Type		male
	  Anim          male\DS_whip_summon_air 1 95
	TEnd
MEnd

Move A_Whip_Summon_Post
	Include sequencers\Members\Combat_Air.inc
	NextMove        Lazy_C_flycycle_Blend
	Flags		NoSizeScale
	Interpolate	0

	Type		male
	  Anim          male\DS_whip_summon_air 95
	TEnd
MEnd

### AIR DEMON UPGRADE #######################################################

Move A_Whip_Upgrade
	Requires	FLY COMBAT DEMONUPGRADE
	Priority        13
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Whip_Upgrade_Post
	Sets            CANTMOVE
	Flags		NoSizeScale pitchtotarget
	SticksOnChild	FLY DEMONUPGRADE
	Interpolate	0

	Type		male
	  Anim          male\DS_whip_upgrade_air 1 65
	TEnd
MEnd

Move A_Whip_Upgrade_Post
	Include sequencers\Members\Combat_Air.inc
	NextMove        Lazy_C_flycycle_Blend
	Flags		NoSizeScale
	Interpolate	0

	Type		male
	  Anim          male\DS_whip_upgrade_air 65
	TEnd
MEnd

## AIR DEMON BUFF #######################################################

Move A_Whip_Buff
	Requires	FLY COMBAT WHIP ATTACK DEMONBUFF
	Priority        15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Whip_Buff_Post
	Sets            CANTMOVE
	Flags		NoSizeScale pitchtotarget
	SticksOnChild	FLY COMBAT WEAPON WHIP ATTACK DEMONBUFF SPAWNSIDE
	Interpolate	0

	Type		male
	  Anim          male\air_DS_whip_buff 1 45
	TEnd
MEnd

Move A_Whip_Buff_Post
	Include sequencers\Members\Combat_Air.inc
	NextMove        Lazy_C_flycycle_Blend
	Flags		NoSizeScale
	Interpolate	0

	Type		male
	  Anim          male\air_DS_whip_buff 45
	TEnd
MEnd