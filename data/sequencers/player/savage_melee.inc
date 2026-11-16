#############################################################
## Savage Melee
#############################################################

Move SavageReady
	Requires	COMBAT
	Priority        1
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <Combat> <AttackIRQ> <MoveIRQ> <IRQEmotesCombat>
	Flags		Cycle
	NextMove        postReady_back
	Scale           0.7

	Type		male
	  Anim          male\savage_melee_ready 1 20
	TEnd

MEnd

## Savage Strike  #############################################

Move Savage_Strike
	Requires	COMBAT WIDOWSLASH ANGRY QUICK
	Priority	4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>  #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Savage_Strike_post
	Sets            CANTMOVE
	Flags		NOSIZESCALE PitchToTarget
	SticksOnChild	PUNCH LEFT

	Type		male
	  Anim          male\savage_strike 1 20
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd

MEnd

Move Savage_Strike_post
	Requires	COMBAT
	Include 	sequencers\Members\Combat.inc
	NextMove	SavageReady
	Flags		ReqInputs

	Type		male
	  Anim          male\savage_strike 21
	TEnd

MEnd

## Savage Strike B  #############################################

Move Savage_Strike_B
	Requires	COMBAT WIDOWSLASH ANGRY QUICK
	Priority	4
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Savage_Strike_B_post
	Sets            CANTMOVE
	Flags		PitchToTarget
	SticksOnChild	PUNCH RIGHT

	Type		male
	  Anim          male\savage_strike_B 1 20
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd

MEnd

Move Savage_Strike_B_post
	NextMove	SavageReady
	Include 	sequencers\Members\Combat.inc

	Type		male
	  Anim          male\savage_strike_B 21
	TEnd

MEnd

## Savage Strike air #############################################

Move A_Savage_Strike
	Requires	FLY COMBAT WIDOWSLASH ANGRY QUICK
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>  #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	A_Savage_Strike_post
	Sets            CANTMOVE
	Flags		NOSIZESCALE PitchToTarget
	SticksOnChild	PUNCH LEFT

	Type		male
	  Anim          male\savage_strike_air 1 20
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd

MEnd

Move A_Savage_Strike_post
	Include 	sequencers\Members\Combat_Air.inc
	NextMove	C_flyready
	Flags		ReqInputs

	Type		male
	  Anim          male\savage_strike_air 21
	TEnd

MEnd

Move A_Savage_Strike_B
	Requires	FLY COMBAT WIDOWSLASH ANGRY QUICK
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	A_Savage_Strike_B_post
	Sets            CANTMOVE
	Flags		PitchToTarget
	SticksOnChild	PUNCH RIGHT

	Type		male
	  Anim          male\savage_strike_B_air 1 20
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd

MEnd

Move A_Savage_Strike_B_post
	NextMove	C_flyready
	Include 	sequencers\Members\Combat_Air.inc

	Type		male
	  Anim          male\savage_strike_B_air 21
	TEnd

MEnd

## Maiming Slash #############################################

Move Maiming_Slash
	Requires	COMBAT WIDOWSLASH ANGRY STRONG SLASH
	Priority	5
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Maiming_Slash_post
	Sets            CANTMOVE
	Flags		PitchToTarget
	SticksOnChild	PUNCH LEFT

	Type		male
	  Anim          male\maiming_slash 1 35
	  PlayFX	Feet\StartScuff.fx 1
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd



MEnd

Move Maiming_Slash_post
	Requires	COMBAT
	NextMove	SavageReady
	Include 	sequencers\Members\Combat.inc

	Type		male
	  Anim          male\maiming_slash 36
	  PlayFX	Feet\StartScuff.fx 1
	  PlayFX	Feet\MaleRight.fx 10
	TEnd

MEnd

## Maiming Slash Air #############################################

Move A_Maiming_Slash
	Requires	FLY COMBAT WIDOWSLASH ANGRY STRONG SLASH
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	A_Maiming_Slash_post
	Sets            CANTMOVE
	Flags		PitchToTarget
	SticksOnChild	PUNCH LEFT

	Type		male
	  Anim          male\maiming_slash_air 1 35
	  PlayFX	Feet\StartScuff.fx 1
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd



MEnd

Move A_Maiming_Slash_post
	Requires	FLY COMBAT
	NextMove	C_flyready
	Include 	sequencers\Members\Combat_Air.inc

	Type		male
	  Anim          male\maiming_slash_air 36
	  PlayFX	Feet\StartScuff.fx 1
	  PlayFX	Feet\MaleRight.fx 10
	TEnd

MEnd


###################
###### Shred  ######
###################

Move Shred
	Requires	COMBAT WIDOWSLASH ANGRY VANDAL
	Priority        4
	Interrupts	<AttackIRQ> Speed_react
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shred_post
	Sets            CANTMOVE
	Flags		NoSizeScale PitchToTarget

	Type		male
	  Anim          male\shred 1 65
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd

MEnd

Move Shred_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        SavageReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\shred 66
	TEnd

MEnd

###################
###### Shred Air ######
###################

Move A_Shred
	Requires	FLY COMBAT WIDOWSLASH ANGRY VANDAL
	Priority        15
	Interrupts	<AttackIRQ> Speed_react
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shred_post
	Sets            CANTMOVE
	Flags		NoSizeScale PitchToTarget

	Type		male
	  Anim          male\shred_air 1 65
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd

MEnd

Move A_Shred_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove        C_flyready
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\shred_air 66
	TEnd

MEnd

## Vicious Slash #############################################

Move Vicious_Slash
	Requires	COMBAT WIDOWSLASH ANGRY TERRIFY
	Priority	4
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	Vicious_Slash_post
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		male
	  Anim          male\vicious_slash 1 50
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd

MEnd

Move Vicious_Slash_post
	Requires	COMBAT
	NextMove	SavageReady
	Include 	sequencers\Members\Combat.inc


	Type		male
	  Anim          male\vicious_slash 51
	TEnd

MEnd

## Vicious_Slash Air #############################################

Move A_Vicious_Slash
	Requires	FLY COMBAT WIDOWSLASH ANGRY TERRIFY
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<AttackIRQ> <DeathIRQ> #<ReactIRQ> <HitIRQ>  <BlockIRQ>  <PostMoveCycle> <MoveIRQ> <IRQEmotesCombat>
	NextMove	A_Vicious_Slash_post
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		male
	  Anim          male\vicious_slash_air 1 50
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd

MEnd

Move A_Vicious_Slash_post
	Requires	FLY COMBAT
	NextMove	C_flyready
	Include 	sequencers\Members\Combat_Air.inc


	Type		male
	  Anim          male\vicious_slash_air 51
	TEnd

MEnd

## Vicious Slash Hit #############################################

Move Vicious_Slash_Hit
	Requires	HIT WIDOWSLASH ANGRY TERRIFY
	Priority        4
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions.inc
	NextMove        CombatReady
	Flags		HitReact

	Type		male
	  Anim          male\vicious_slash_HQ 1 52
	TEnd

MEnd

Move C_Vicious_Slash_Hit
	Requires	COMBAT HIT WIDOWSLASH ANGRY TERRIFY
	Priority        5
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions.inc
	NextMove        CombatReady
	Flags		HitReact

	Type		male
	  Anim          male\vicious_slash_HQ 1 52
	TEnd

MEnd

## Vicious Slash Hit Air #############################################

Move A_Vicious_Slash_Hit
	Requires	FLY HIT WIDOWSLASH ANGRY TERRIFY
	Priority        15
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions_air.inc
	NextMove        C_flyready
	Flags		HitReact

	Type		male
	  Anim          male\vicious_slash_HQ_air 1 52
	TEnd

MEnd

Move A_C_Vicious_Slash_Hit
	Requires	FLY COMBAT HIT WIDOWSLASH ANGRY TERRIFY
	Priority        16
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions_Air.inc
	NextMove        C_flyready
	Flags		HitReact

	Type		male
	  Anim          male\vicious_slash_HQ_air 1 52
	TEnd

MEnd

## Vicious Slash Hit Weapon #############################################

Move W_Vicious_Slash_Hit
	Requires	HIT WIDOWSLASH ANGRY TERRIFY WEAPON
	Priority        5
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions.inc
	NextMove        C_flyready
	Flags		HitReact

	Type		male
	  Anim          male\vicious_slash_HQ 1 52
	TEnd

MEnd

Move W_C_Vicious_Slash_Hit
	Requires	COMBAT HIT WIDOWSLASH ANGRY TERRIFY WEAPON
	Priority        6
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions.inc
	NextMove        C_flyready
	Flags		HitReact

	Type		male
	  Anim          male\vicious_slash_HQ 1 52
	TEnd

MEnd

## Vicious Slash Hit Weapon Air #############################################

Move A_W_Vicious_Slash_Hit
	Requires	FLY HIT WIDOWSLASH ANGRY TERRIFY WEAPON
	Priority        16
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions_Air.inc
	NextMove        W_FlyReady
	Flags		HitReact

	Type		male
	  Anim          male\vicious_slash_HQ_air 1 52
	TEnd

MEnd

Move A_W_C_Vicious_Slash_Hit
	Requires	FLY COMBAT HIT WIDOWSLASH ANGRY TERRIFY WEAPON
	Priority        17
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions_Air.inc
	NextMove        W_FlyReady
	Flags		HitReact

	Type		male
	  Anim          male\vicious_slash_HQ_air 1 52
	TEnd

MEnd

## Spinning Strike Hit Shot Gun #############################################

Move SG_Vicious_Slash_Hit
	Requires	HIT WIDOWSLASH ANGRY TERRIFY WEAPON GUN
	Priority        6
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions.inc
	NextMove        ShotgunReady
	Flags		HitReact

	Type		male
	  Anim          male\vicious_slash_HQ 1 52
	TEnd

MEnd

Move SG_C_Vicious_Slash_Hit
	Requires	COMBAT HIT WIDOWSLASH ANGRY TERRIFY WEAPON GUN
	Priority        7
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions.inc
	NextMove        ShotgunReady
	Flags		HitReact

	Type		male
	  Anim          male\vicious_slash_HQ 1 52
	TEnd

MEnd

## Spinning Strike Hit Shot Gun Air #############################################

Move A_SG_Vicious_Slash_Hit
	Requires	FLY HIT WIDOWSLASH ANGRY TERRIFY WEAPON GUN
	Priority        17
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions_Air.inc
	NextMove        A_ShotgunReady
	Flags		HitReact

	Type		male
	  Anim          male\vicious_slash_HQ_air 1 52
	TEnd

MEnd

Move A_SG_C_Vicious_Slash_Hit
	Requires	FLY COMBAT HIT WIDOWSLASH ANGRY TERRIFY WEAPON GUN
	Priority        18
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions_Air.inc
	NextMove        A_ShotgunReady
	Flags		HitReact

	Type		male
	  Anim          male\vicious_slash_HQ_air 1 52
	TEnd

MEnd

###################
###### Rending Flurry  ######
###################

Move Rending_Flurry
	Requires	COMBAT WIDOWSLASH ANGRY CLAWFLURRY
	Priority        4
	Interrupts	<AttackIRQ> Speed_react
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Rending_Flurry_post
	Sets            CANTMOVE
	Flags		NoSizeScale PitchToTarget

	Type		male
	  Anim          male\rending_flurry 1 65
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd

MEnd

Move Rending_Flurry_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\rending_flurry 66
	TEnd

MEnd

###################
###### Rending Flurry Air ######
###################

Move A_Rending_Flurry
	Requires	FLY COMBAT WIDOWSLASH ANGRY CLAWFLURRY
	Priority        15
	Interrupts	<AttackIRQ> Speed_react
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Rending_Flurry_post
	Sets            CANTMOVE
	Flags		NoSizeScale PitchToTarget

	Type		male
	  Anim          male\rending_flurry_air 1 65
	  PitchAngle	0.3
	  PitchEnd	50
	TEnd

MEnd

Move A_Rending_Flurry_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove        C_flyready
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\rending_flurry_air 66
	TEnd

MEnd

###################
###### Hemorrhage  ######
###################

Move Hemorrhage
	Requires	COMBAT WIDOWSLASH ANGRY FIERCE
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Hemorrhage_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\hemorrhage 1 60
	TEnd

MEnd

Move Hemorrhage_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        SavageReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\hemorrhage 61
	TEnd

MEnd

###################
###### Hemorrhage Air ######
###################

Move A_Hemorrhage
	Requires	FLY COMBAT WIDOWSLASH ANGRY FIERCE
	Priority        15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Hemorrhage_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\hemorrhage_air 1 60
	TEnd

MEnd

Move A_Hemorrhage_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove        C_flyready
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\hemorrhage_air 61
	TEnd

MEnd

############################
###### Savage Leap ######
###########################

Move Savage_Leap
	Requires	COMBAT WIDOWSLASH ANGRY TELEPORT
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Savage_Leap_2
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          male\maiming_slash 1 12
	TEnd
MEnd

Move Savage_Leap_2
	Requires	COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Savage_Leap_post
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          male\maiming_slash 13 40
	TEnd
MEnd

Move Savage_Leap_post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        SavageReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\maiming_slash 41
	TEnd
MEnd

############################
###### Savage Leap Air ######
###########################

Move A_Savage_Leap
	Requires	FLY COMBAT WIDOWSLASH ANGRY TELEPORT
	Priority        15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Savage_Leap_2
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          male\maiming_slash_air 1 12
	TEnd
MEnd

Move A_Savage_Leap_2
	Requires	FLY COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Savage_Leap_post
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          male\maiming_slash_air 13 40
	TEnd
MEnd

Move A_Savage_Leap_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove        C_flyready
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\maiming_slash_air 41
	TEnd
MEnd

## Assassins Frenzy #############################################

Move Assassins_Frenzy_Windup
	Requires	COMBAT WIDOWSLASH ANGRY WINDUP
	Priority	4
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Windups.inc
	Flags		NoSizeScale PitchToTarget
	NextMove	Assassins_Frenzy_Windup_Cycle

	Type		male
	  Anim		male\assassins_frenzy 1 10
	TEnd
MEnd

Move Assassins_Frenzy_Windup_Cycle
	Requires	COMBAT WIDOWSLASH ANGRY WINDUP
	Include sequencers\Members\Windups.inc
	Flags		Cycle NoSizeScale PitchToTarget
	NextMove	SavageReady

	Type		male
	  Anim		male\assassins_frenzy 11 26
	TEnd
MEnd

#############################################################

Move Assassins_Frenzy
	Requires	COMBAT WIDOWSLASH ANGRY STAB
	Priority	4
	Interrupts	<AttackIRQ> <Windups>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Assassins_Frenzy_Post
	Sets		CANTMOVE
	Flags		NoSizeScale PitchToTarget

	Type		male
	  Anim		male\assassins_frenzy 27 57
	TEnd
MEnd

Move Assassins_Frenzy_Post
	Include sequencers\Members\Combat.inc
	NextMove	CombatReady
	Flags		NoSizeScale

	Type		male
	  Anim		male\assassins_frenzy 58
	TEnd
MEnd

## Assassins Frenzy Air #############################################

Move A_Assassins_Frenzy_Windup
	Requires	FLY COMBAT WIDOWSLASH ANGRY WINDUP
	Priority	15
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Windups_air.inc
	Flags		NoSizeScale PitchToTarget
	NextMove	A_Assassins_Frenzy_Windup_Cycle

	Type		male
	  Anim		male\assassins_frenzy_air 1 10
	TEnd
MEnd

Move A_Assassins_Frenzy_Windup_Cycle
	Requires	FLY COMBAT WIDOWSLASH ANGRY WINDUP
	Include sequencers\Members\Windups_air.inc
	Flags		Cycle NoSizeScale PitchToTarget
	NextMove	C_flyready

	Type		male
	  Anim		male\assassins_frenzy_air 11 26
	TEnd
MEnd

#############################################################

Move A_Assassins_Frenzy
	Requires	FLY COMBAT WIDOWSLASH ANGRY STAB
	Priority	15
	Interrupts	<AttackIRQ> <Windups>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Assassins_Frenzy_Post
	Sets		CANTMOVE
	Flags		NoSizeScale PitchToTarget

	Type		male
	  Anim		male\assassins_frenzy_air 27 57
	TEnd
MEnd

Move A_Assassins_Frenzy_Post
	Include sequencers\Members\Combat_air.inc
	NextMove	C_flyready
	Flags		NoSizeScale

	Type		male
	  Anim		male\assassins_frenzy_air 58
	TEnd
MEnd





