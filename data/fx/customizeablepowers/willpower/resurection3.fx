#########################################################
##	chill touch hit
FxInfo
LifeSpan	200

########################################################

Condition
	On 	Time
	Time 	30


	Event
		EName	Left
		Type	Local
		At	wepL

		Part1	CustomizeablePowers\Willpower\Stamina2.part
		Part2	CustomizeablePowers\Willpower\StaminaSmall2.part
		Part3	CustomizeablePowers\Willpower\AbilityDots.part
		Part4	CustomizeablePowers\Willpower\AbilitySparklingDots.part
		Sound heal5 70 70 .75
		Lifespan 70
	End

	Event
		EName	Right
		Type	Local
		At	wepR

		Part1	CustomizeablePowers\Willpower\Stamina2.part
		Part2	CustomizeablePowers\Willpower\StaminaSmall2.part
		Part3	CustomizeablePowers\Willpower\AbilityDots.part
		Part4	CustomizeablePowers\Willpower\AbilitySparklingDots.part
		Lifespan 70
	End

End
############################################################################################

Condition
	On 	Time
	Time 	80

	Event
		EName 	BottleRocketHookup
		Type	Local
		At	Root
		Bhvr	CustomizeablePowers\Willpower\teleportscale.bhvr
		Anim	FX_teleportbase
		Lifespan 60
	End

	Event
		EName 	BottleRocket02
		Type	Local
		At	BottleRocketHookup
		AnimPiv Waist
		Part1	CustomizeablePowers\Willpower\RessurectionBottleRocketSparks.part
		Part2	CustomizeablePowers\Willpower\BottleRocketHead.part
		Lifespan 60
	End


	Event
		EName 	BottleRocket03
		Type	Local
		At	BottleRocketHookup
		AnimPiv Neck
		Part1	CustomizeablePowers\Willpower\RessurectionBottleRocketSparks.part
		Part2	CustomizeablePowers\Willpower\BottleRocketHead.part
		Lifespan 60
	End

	Event
		EName 	BottleRocket04
		Type	Local
		At	BottleRocketHookup
		AnimPiv Head
		Part1	CustomizeablePowers\Willpower\RessurectionBottleRocketSparks.part
		Part2	CustomizeablePowers\Willpower\BottleRocketHead.part
		Lifespan 60
	End

	Event
		EName	Outerglow
		Type	Local
		At	Root

		Part1	CustomizeablePowers\Willpower\Sparklies.part
		Part2	CustomizeablePowers\Willpower\RingSparklies.part
		Part3	CustomizeablePowers\Willpower\SharpRingSparklies.part
		Part4	CustomizeablePowers\Willpower\streaks.part
		Part5	CustomizeablePowers\Willpower\Glowstreaks.part
		Lifespan 40
	End

End

#############################################################################################

Condition
	On	Time
	Time	78

	Event
		EName	Ring
		Type	Local
		At	Root
		part1	CustomizeablePowers\Empathy\StaminaRingUp.part
		#Sound	heal1 70 25 .8

	End

End

Condition
	On	Time
	Time	85

	Event
		EName	Ring
		Type	Local
		At	Root
		part1	CustomizeablePowers\Empathy\StaminaRingUp.part

	End

End

Condition
	On	Time
	Time	95

	Event
		EName	Ring
		Type	Local
		At	Root
		part1	CustomizeablePowers\Empathy\StaminaRingUp.part

	End

End

############################################################################################

Condition
	On 	Time
	Time 	40

	Event
		EName	1
		Type	Local
		At	UlegL
		part1	CustomizeablePowers\Willpower\RegenBodyGlows2.part
		PMagnet	Hips
		Lifespan 100
	End

	Event
		EName	2
		Type	Local
		At	UlegR
		part1	CustomizeablePowers\Willpower\RegenBodyGlows2.part
		PMagnet	Hips
		Lifespan 100
	End

	Event
		EName	3
		Type	Local
		At	LlegL
		part1	CustomizeablePowers\Willpower\RegenBodyGlows2.part
		PMagnet	ULEGL
		Lifespan 100

	End

	Event
		EName	4
		Type	Local
		At	LlegR
		part1	CustomizeablePowers\Willpower\RegenBodyGlows2.part
		PMagnet	ULEGR
		Lifespan 100


	End

	Event
		EName	5
		Type	Local
		At	FootL
		part1	CustomizeablePowers\Willpower\RegenBodyGlows3.part
		PMagnet	LLEGL
		Lifespan 100
		Lifespan 100

	End

	Event
		EName	6
		Type	Local
		At	FootR
		part1	CustomizeablePowers\Willpower\RegenBodyGlows3.part
		PMagnet	LLEGR
		Lifespan 100

	End

####################################################################################
##Original
##RegenBodyGlows2.part
#####################################################################################

	Event
		EName	7
		Type	Local
		At	FootR
		part1	CustomizeablePowers\Willpower\RegenBodyGlows.part
		PMagnet LlegR
		Lifespan 100

	End


	Event
		EName	8
		Type	Local
		At	ULEGL
		part1	CustomizeablePowers\Willpower\RegenBodyGlows.part
		Lifespan 100


	End

	Event
		EName	9
		Type	Local
		At	LLEGL
		part1	CustomizeablePowers\Willpower\RegenBodyGlows.part
		PMagnet UlegL
		Lifespan 100

	End

	Event
		EName	10
		Type	Local
		At	FootL
		part1	CustomizeablePowers\Willpower\RegenBodyGlows.part
		PMagnet LlegL
		Lifespan 100

	End



	Event
		EName	11
		Type	Local
		At	ULEGR
		part1	CustomizeablePowers\Willpower\RegenBodyGlows.part
		Lifespan 100
	End

	Event
		EName	12
		Type	Local
		At	LLEGR
		part1	CustomizeablePowers\Willpower\RegenBodyGlows.part
		PMagnet UlegL
		Lifespan 100
	End

###############################################################################

	Event
		EName	13
		Type	Local
		At	Head
		part1	CustomizeablePowers\Willpower\RegenBodyGlows.part
		Lifespan 100
	End

##########################################################################
##Arm
##################################################################

	Event
		EName	14
		Type	Local
		At	UArmL
		part1	CustomizeablePowers\Willpower\RegenBodyGlows.part
		Lifespan 100
	End

	Event
		EName	15
		Type	Local
		At	LArmL
		part1	CustomizeablePowers\Willpower\RegenBodyGlows.part
		Lifespan 100
	End

	Event
		EName	16
		Type	Local
		At	WepL
		part1	CustomizeablePowers\Willpower\RegenBodyGlows.part
		Lifespan 100
	End

	Event
		EName	17
		Type	Local
		At	UArmR
		part1	CustomizeablePowers\Willpower\RegenBodyGlows.part
		Lifespan 100
	End

	Event
		EName	18
		Type	Local
		At	LArmR
		part1	CustomizeablePowers\Willpower\RegenBodyGlows.part
		Lifespan 100
	End

	Event
		EName	19
		Type	Local
		At	WepR
		part1	CustomizeablePowers\Willpower\RegenBodyGlows.part
		Lifespan 100
	End

End
###################################################################################

End