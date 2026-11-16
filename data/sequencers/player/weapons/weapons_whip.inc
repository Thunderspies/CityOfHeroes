#############################################################
## Weapons_Whip.inc
#############################################################

### DRAW WHIP #######################################################
#
#Move Draw_Whip
#	Requires	COMBAT WHIP SPAWNSIDE
#	Priority        4
#	Interrupts	<AttackIRQ>
#	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <AttackIRQ> <MoveIRQ> <IRQEmotesRighthand>
#	NextMove        WepReady
#
#	Scale		1.25
#
#	Type		male
#	  Anim          male\draw_saber 1
#	TEnd
#MEnd

## WHIP CORRUPTION #######################################################

Move Whip_Attack1
	Requires	COMBAT WHIP ATTACK LOW
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Whip_Attack1_Post
	Sets            CANTMOVE
	SticksOnChild	COMBAT WEAPON WHIP ATTACK LOW
	Interpolate	0

	Type		male
	  Anim          male\DS_whip_corruption 1 38
	TEnd
MEnd

Move Whip_Attack1_Post
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Lazy_CombatReady
	Interpolate	0

	Type		male
	  Anim          male\DS_whip_corruption 38
	TEnd
MEnd

## WHIP LASH #######################################################

Move Whip_Attack2
	Requires	COMBAT WHIP ATTACK HIGH
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Whip_Attack2_Post
	Sets            CANTMOVE
	SticksOnChild	COMBAT WEAPON WHIP ATTACK HIGH
	Interpolate	0

	Type		male
	  Anim          male\DS_whip_lash 1 55
	TEnd
MEnd

Move Whip_Attack2_Post
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Lazy_CombatReady
	Interpolate	0

	Type		male
	  Anim          male\DS_whip_lash 55
	TEnd
MEnd

## WHIP STRIKE #######################################################

Move Whip_Attack3
	Requires	COMBAT WHIP ATTACK CONE
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Whip_Attack3_Post
	Sets            CANTMOVE
	SticksOnChild	COMBAT WEAPON WHIP ATTACK CONE
	Scale		2
	Interpolate	0

	Type		male
	  Anim          male\DS_whip_strike 1 140
	TEnd
MEnd

Move Whip_Attack3_Post
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Lazy_CombatReady
	Scale		2
	Interpolate	0

	Type		male
	  Anim          male\DS_whip_strike 140
	TEnd
MEnd

## DEMON SUMMON #######################################################

Move Whip_Summon
	Requires	COMBAT DEMONSUMMON
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Whip_Summon_Post
	Sets            CANTMOVE

	Type		male
	  Anim          male\DS_whip_summon 1 95
	TEnd
MEnd

Move Whip_Summon_Post
	Include sequencers\Members\Combat.inc
	NextMove        Lazy_CombatReady

	Type		male
	  Anim          male\DS_whip_summon 95
	TEnd
MEnd

## DEMON UPGRADE - ENCHANT/ABYSSAL #######################################################

Move Whip_Upgrade
	Requires	COMBAT DEMONUPGRADE
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Whip_Upgrade_Post
	SticksOnChild	DEMONUPGRADE
	Sets            CANTMOVE

	Type		male
	  Anim          male\DS_whip_upgrade 1 65
	TEnd
MEnd

Move Whip_Upgrade_Post
	Include sequencers\Members\Combat.inc
	NextMove        Lazy_CombatReady

	Type		male
	  Anim          male\DS_whip_upgrade 65
	TEnd
MEnd

## DEMON BUFF - HELL ON EARTH #######################################################

Move Whip_Buff
	Requires	COMBAT WHIP ATTACK DEMONBUFF
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Whip_Buff_Post
	Sets            CANTMOVE
	SticksOnChild	COMBAT WHIP ATTACK DEMONBUFF
	Interpolate	0

	Type		male
	  Anim          male\DS_whip_buff 1 121
	TEnd
MEnd

Move Whip_Buff_Post
	Requires	COMBAT WHIP ATTACK DEMONBUFF
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Lazy_CombatReady
	Interpolate	0

	Type		male
	  Anim          male\DS_whip_buff 122
	TEnd
MEnd

#############################################################
## Boost_up react
#############################################################

Move DemonSummoner_Hit_Boostup
	Requires	HIT BOOSTUP
	Priority        2
	Interrupts	<ReactIRQ>
	Include sequencers\Members\Reactions.inc
	NextMove        Ready

	Type		male
	  Anim          male\absorb 1
	TEnd

	Type		huge
	  Anim          huge\boostup 1
	TEnd

	Type		fem
	  Anim          fem\boostup 1
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_boostaura 1
	TEnd
MEnd