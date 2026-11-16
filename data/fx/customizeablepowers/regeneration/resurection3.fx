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

		Part1	CustomizeablePowers\Regeneration\Stamina2.part
		Part2	CustomizeablePowers\Regeneration\StaminaSmall2.part
		Part3	CustomizeablePowers\Regeneration\AbilityDots.part
		Part4	CustomizeablePowers\Regeneration\AbilitySparklingDots.part
		Sound heal5 70 70 .75
	End

	Event
		EName	Right
		Type	Local
		At	wepR

		Part1	CustomizeablePowers\Regeneration\Stamina2.part
		Part2	CustomizeablePowers\Regeneration\StaminaSmall2.part
		Part3	CustomizeablePowers\Regeneration\AbilityDots.part
		Part4	CustomizeablePowers\Regeneration\AbilitySparklingDots.part
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
		Bhvr	CustomizeablePowers\Regeneration\teleportscale.bhvr
		Anim	FX_teleportbase
	End

	Event
		EName 	BottleRocket02
		Type	Local
		At	BottleRocketHookup
		AnimPiv Waist
		Part1	CustomizeablePowers\Regeneration\RessurectionBottleRocketSparks.part
		Part2	CustomizeablePowers\Regeneration\BottleRocketHead.part
		#Part3	Powers/Teleport/TeleStreak.part
	End


	Event
		EName 	BottleRocket03
		Type	Local
		At	BottleRocketHookup
		AnimPiv Neck
		Part1	CustomizeablePowers\Regeneration\RessurectionBottleRocketSparks.part
		Part2	CustomizeablePowers\Regeneration\BottleRocketHead.part
		#Part3	Powers/Teleport/TeleStreak.part
	End

	Event
		EName 	BottleRocket04
		Type	Local
		At	BottleRocketHookup
		AnimPiv Head
		Part1	CustomizeablePowers\Regeneration\RessurectionBottleRocketSparks.part
		Part2	CustomizeablePowers\Regeneration\BottleRocketHead.part
		#Part3	Powers/Teleport/TeleStreak.part

	End

	Event
		EName	Outerglow
		Type	Local
		At	Root

		Part1	CustomizeablePowers\Regeneration\Sparklies.part
		Part2	CustomizeablePowers\Regeneration\RingSparklies.part
		Part3	CustomizeablePowers\Regeneration\SharpRingSparklies.part
		Part4	CustomizeablePowers\Regeneration\streaks.part
		Part5	CustomizeablePowers\Regeneration\Glowstreaks.part
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
		part1	CustomizeablePowers\Regeneration\StaminaRingUp.part
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
		part1	CustomizeablePowers\Regeneration\StaminaRingUp.part

	End

End

Condition
	On	Time
	Time	95

	Event
		EName	Ring
		Type	Local
		At	Root
		part1	CustomizeablePowers\Regeneration\StaminaRingUp.part

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
		part1	CustomizeablePowers\Regeneration\RegenBodyGlows2.part
		PMagnet	Hips
	End

	Event
		EName	2
		Type	Local
		At	UlegR
		part1	CustomizeablePowers\Regeneration\RegenBodyGlows2.part
		PMagnet	Hips
	End

	Event
		EName	3
		Type	Local
		At	LlegL
		part1	CustomizeablePowers\Regeneration\RegenBodyGlows2.part
		PMagnet	ULEGL

	End

	Event
		EName	4
		Type	Local
		At	LlegR
		part1	CustomizeablePowers\Regeneration\RegenBodyGlows2.part
		PMagnet	ULEGR


	End

	Event
		EName	5
		Type	Local
		At	FootL
		part1	CustomizeablePowers\Regeneration\RegenBodyGlows3.part
		PMagnet	LLEGL

	End

	Event
		EName	6
		Type	Local
		At	FootR
		part1	CustomizeablePowers\Regeneration\RegenBodyGlows3.part
		PMagnet	LLEGR

	End

####################################################################################
##Original
##RegenBodyGlows2.part
#####################################################################################

	Event
		EName	7
		Type	Local
		At	FootR
		part1	CustomizeablePowers\Regeneration\RegenBodyGlows.part
		PMagnet LlegR
	End


	Event
		EName	8
		Type	Local
		At	ULEGL
		part1	CustomizeablePowers\Regeneration\RegenBodyGlows.part


	End

	Event
		EName	9
		Type	Local
		At	LLEGL
		part1	CustomizeablePowers\Regeneration\RegenBodyGlows.part

		PMagnet UlegL
	End

	Event
		EName	10
		Type	Local
		At	FootL
		part1	CustomizeablePowers\Regeneration\RegenBodyGlows.part
		PMagnet LlegL
	End



	Event
		EName	11
		Type	Local
		At	ULEGR
		part1	CustomizeablePowers\Regeneration\RegenBodyGlows.part


	End


#LegR


	Event
		EName	12
		Type	Local
		At	LLEGR
		part1	CustomizeablePowers\Regeneration\RegenBodyGlows.part

		PMagnet UlegL
	End

###############################################################################

	Event
		EName	13
		Type	Local
		At	Head
		part1	CustomizeablePowers\Regeneration\RegenBodyGlows.part

	End

##########################################################################
##Arm
##################################################################

	Event
		EName	14
		Type	Local
		At	UArmL
		part1	CustomizeablePowers\Regeneration\RegenBodyGlows.part

		#PMagnet ElbowL

	End

	Event
		EName	15
		Type	Local
		At	LArmL
		part1	CustomizeablePowers\Regeneration\RegenBodyGlows.part

		#PMagnet HandL
	End

	Event
		EName	16
		Type	Local
		At	WepL
		part1	CustomizeablePowers\Regeneration\RegenBodyGlows.part

	End

	Event
		EName	17
		Type	Local
		At	UArmR
		part1	CustomizeablePowers\Regeneration\RegenBodyGlows.part

		#PMagnet ElbowL

	End

	Event
		EName	18
		Type	Local
		At	LArmR
		part1	CustomizeablePowers\Regeneration\RegenBodyGlows.part

		#PMagnet HandL
	End

	Event
		EName	19
		Type	Local
		At	WepR
		part1	CustomizeablePowers\Regeneration\RegenBodyGlows.part

	End

End
###################################################################################

Condition
	On	Time
	Time	100

	Event
		EName	Left
		Type	Destroy

	End

	Event
		EName	Right
		Type	Destroy

	End

End

Condition
	On	Time
	Time	140

	Event
		EName	1
		Type	Destroy

	End

	Event
		EName	2
		Type	Destroy

	End

	Event
		EName	3
		Type	Destroy

	End

	Event
		EName	4
		Type	Destroy

	End

	Event
		EName	5
		Type	Destroy

	End

	Event
		EName	6
		Type	Destroy

	End

	Event
		EName	7
		Type	Destroy

	End

	Event
		EName	8
		Type	Destroy

	End

	Event
		EName	9
		Type	Destroy

	End

	Event
		EName	10
		Type	Destroy

	End

	Event
		EName	11
		Type	Destroy

	End

	Event
		EName	12
		Type	Destroy

	End

	Event
		EName	13
		Type	Destroy

	End

	Event
		EName	14
		Type	Destroy

	End

	Event
		EName	15
		Type	Destroy

	End

	Event
		EName	16
		Type	Destroy

	End

	Event
		EName	17
		Type	Destroy

	End

	Event
		EName	18
		Type	Destroy

	End

	Event
		EName	19
		Type	Destroy

	End
End


Condition
	On	Time
	Time	140

	Event
		EName	BottleRocketHookup
		Type	Destroy

	End

	Event
		EName	BottleRocket02
		Type	Destroy

	End

	Event
		EName	BottleRocket03
		Type	Destroy

	End

	Event
		EName	BottleRocket04
		Type	Destroy

	End

End


Condition
	On	Time
	Time	120

	Event
		EName	OuterGlow
		Type	Destroy

	End

End

End