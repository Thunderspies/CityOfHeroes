
#####  Do NOT Hook this file up to any sequencers (redundant entries will crash game)  #######################################
##     Note: I'm taking out female and huge versions to keeps this smaller, but you will need to account for them





#####  Non Combat Power (does not stay in a mode. You can make it go back to ready or an alt ready (like below) ######


########################
###### NonCombat Power  ######
########################

Move NonCombat_Power
	Requires	BIT
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        NonCombat_Power_post
	Priority        1
	Flags		CANTMOVE
	Scale		0.5


	Type		male
	  Anim          male\Loot 1 19
	TEnd

MEnd


Move NonCombat_Power_post
	Member		<ReactIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <BlockIRQ>
	NextMove        NonCombat_Power_cycle	##(...or could go to existing ready or cycle)


	Type		male
	  Anim          male\Loot 24
	TEnd
MEnd

Move NonCombat_Power_cycle
	Member		<Ready> <ReactIRQ> <DeathIRQ> <AttackIRQ>
	Flags		Cycle

	Type		male
	  Anim          male\Skel_ready2 1
	TEnd
MEnd

######  Combat Attack2  (2 part move)  ##########################################################
##     Note: This represents a simple 2 part attack in combat mode. The first part cannot be interupted.

#################################
####### Combat Power ######
##################################

Move Combat_Power
	Requires	COMBAT BIT		##(Combat - mode bit if needed, Underhandtoss - specific bit for that power)
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        Combat_Power_2
	Priority        2
	Flags		CANTMOVE		##(needed to lock player down. Only use here at crutial part of attack)

	Type		male
	  Anim          male\Underhand_toss 1 45
	TEnd

MEnd


Move Combat_Power_2
	Requires	COMBAT				##(only if needed. Here it keeps the playre in combat mode)
	Member		<ReactIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <BlockIRQ> ##(This part of the anim is interuptable)
	NextMove        Lazy_CombatReady		##(this is going to an existing ready or cycle))
	Priority        1

	Type		male
	  Anim          male\Underhand_toss 45
	TEnd

MEnd




######  Air version of Combat Attack2  (2 part)  ##################################################



#################################
###### Air Combat Power   ######
#################################

Move A_Combat_Power
	Requires	Fly COMBAT BIT		##(Fly gets added to all moves to keep in fly mode)
	Interrupts	<AttackIRQ>		##(certian <groups> now point to "Flying" versions)
	Member		<DeathIRQ>
	NextMove        A_Combat_Power_2
	Priority        3
	Flags		 CANTMOVE


	Type		male
	  Anim          male\air_Underhand_toss 1 45
	TEnd

MEnd


Move A_Combat_Power_2
	Requires	Fly COMBAT
	Member		<ReactIRQ> <DeathIRQ> <AttackIRQ> <BlockIRQ> <FlyCombatAttack>  ## (<FlyCombatAttack> - this is unique to the mode. Others exist for different modes: wep, gun, dual...)
	NextMove        a_Lazy_CombatReady		##(make sure anims points to an air version)
	Priority        2

	Type		male
	  Anim          male\air_Underhand_toss 45
	TEnd


MEnd







######  Basic Weapon Attack  (w\ draw weapon feature)  ##########################################################
##     Note: This represents a simple 2 part attack in combat mode. The first part cannot be interupted.


##############################
###### default Draw Shotgun  ######	## (this is for when you activate this power w\ out a target)
##############################

Move Draw_Shotgun
	Requires	COMBAT WEAPON GUN SPAWNBACK
	Interrupts	<Ready> <Combat> <WepCombat> <TwoHandCombat> Draw_saber <AttackIRQ>
	Member		<ReactIRQ> <DeathIRQ> <AttackIRQ>
	NextMove        Draw_Shotgun_post
	Priority        4
	Sets            CANTMOVE
	Flags		FixArmReg NoSizeScale



	Type		male
	  Anim          male\draw_shotgun_basic 1 20
	TEnd

MEnd


Move Draw_Shotgun_post
	Requires	COMBAT WEAPON GUN
	Member		<ReactIRQ> <DeathIRQ> <TwoHandCombat> <AttackIRQ> <BlockIRQ>
	NextMove        D_Shotgun_cycle
	Priority        3
	Flags		FixArmReg NoSizeScale


	Type		male
	  Anim          male\draw_shotgun_basic 21
	TEnd

MEnd


Move D_Shotgun_cycle
	Requires	COMBAT WEAPON GUN
	Member		<ReactIRQ> <DeathIRQ> <TwoHandCombat> <AttackIRQ> <BlockIRQ>
	Flags		Cycle FixArmReg
	NextMove        PostShotgun
	Priority        3
	Scale           0.50

	Type		male
	  Anim          male\shotgun_cycle 1
	TEnd

MEnd

##############################
###### Buckshot (Shotgun)  ######	## (This has a 1st & 2nd attacks - The 1st had a wep draw added)
##############################

Move 1st_BuckShot
	Requires	COMBAT WEAPON GUN SPAWNBACK LOW SHOTGUN  ##(Bits 1-3 = mode, bit 4 = indicates 1st, bits 5,6 = specific attack)
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        1st_BuckShot2
	Priority        6
	Flags		PitchToTarget NoSizeScale FixArmReg CANTMOVE

	Type		male
	  Anim          male\draw_shotgun_point 1 39		## (Draws shotgun - try to keep frames 30 or less)
	  PitchAngle	0.3
	  PitchEnd	38
	TEnd

MEnd


Move 1st_BuckShot2
	Requires	COMBAT WEAPON GUN			## (all mode bits - keeps in stance)
	Member		<DeathIRQ>
	NextMove        1st_BuckShot_post
	Priority        3
	Flags		FixArmReg NoSizeScale CANTMOVE


	Type		male
	  Anim          male\shotgun_shoot 31 50
	TEnd

MEnd


Move 1st_BuckShot_post
	Requires	COMBAT WEAPON GUN			## (all mode bits - keeps in stance)
	Member		<ReactIRQ> <DeathIRQ> <TwoHandAttack> <BlockIRQ> <AttackIRQ>
	NextMove        D_Shotgun_cycle
	Priority        3
	Scale           0.50
	Flags		FixArmReg NoSizeScale

	Type		male
	  Anim          male\shotgun_shoot 50
	TEnd

MEnd

###

Move 2nd_shotgunshoot
	Requires	COMBAT WEAPON GUN LOW SHOTGUN		##(bits similar to 1st minus "SPAWNBACK")
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        2nd_shotgunshoot2			## (If your animation allows it, point to 1st_shotgunshoot2 instead)
	Flags		PitchToTarget FixArmReg CANTMOVE

	Type		male
	  Anim          male\shotgun_shoot 1 31
	  PitchAngle	0.3
	TEnd

MEnd


Move 2nd_shotgunshoot2
	Requires	COMBAT WEAPON GUN
	Priority        3
	Member		<ReactIRQ> <DeathIRQ> <TwoHandAttack> <BlockIRQ> <AttackIRQ>
	NextMove        D_Shotgun_cycle
	Scale           0.7
	Flags		FixArmReg NoSizeScale

	Type		male
	  Anim          male\shotgun_shoot 31
	  PitchAngle	0.3
	  PitchEnd	38
	TEnd
MEnd


#############           Emotes              ###########################################




####################################################
##### Simple Emote - goes back to a bobble ready   #####
####################################################

Move Simple_Emote
	Requires	EMOTE ROLLDICE HIGH
	Priority        3
	Interrupts	<Ready> <MonsterGroup> <ReactIRQ> <Combat> <WepCombat> <TwoHandCombat> <PostMoveCycle> <Block>
	Member		<ReactIRQ> <DeathIRQ> <AttackIRQ> <Block>
	NextMove        Simple_Emote_post
	Scale		1.5
	Interpolate	12

	Type		male
	  Anim          male\roll_dice 125 201
	TEnd
MEnd

Move Simple_Emote_post
	Member		<ReactIRQ> <DeathIRQ> <AttackIRQ> <Ready> <MonsterGroup>
	NextMove        postReady_back
	Priority        1
	Interpolate	8

	Type		male
	  Anim          male\roll_dice 1 120
	  PlayFx        GENERIC\Dice_1.FX  80 Constant
	  PlayFx        GENERIC\Prop_Dice.FX  69
	TEnd
MEnd

#################################################
#### Loop Emote - Emote w\ built in cycle	#####
#################################################

Move LoopEmote
	Requires	EMOTE DANCE POSE BOX
	Interrupts	<Ready> <MonsterGroup> <ReactIRQ> <Combat> <WepCombat> <TwoHandCombat> <PostMoveCycle> <Block>
	Member		<ReactIRQ> <DeathIRQ> <AttackIRQ> <Block>
	NextMove        LoopEmote_cycle
	Priority        4
	Interpolate	8

	Type		male
	  Anim          male\Loot 3 50
	  PlayFx        GENERIC\Boombox_sound.FX  0 Constant
	TEnd
MEnd

Move LoopEmote_cycle
	Member		<Ready> <MonsterGroup> <ReactIRQ> <DeathIRQ> <AttackIRQ>
	Flags		Cycle

	Type		male
	  Anim          male\dance_base 1
	  PlayFx        GENERIC\Boombox_sound.FX  0 Constant
	TEnd
MEnd