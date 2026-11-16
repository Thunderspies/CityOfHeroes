#########################################################
##	Fire_Ball
##
FxInfo

##################################


Condition
	On	Time
	Time	20

	Event
		EName	2
		Type	Local
		At	FootR
		part1	CustomizeablePowers\DarkArmor\LiteSmoke.part
		part2	CustomizeablePowers\DarkArmor\LiteSmoke2.part
		Part3	CustomizeablePowers\DarkArmor\LiteDarkBlastHitTendril.part
		Sound miasma_loop 60 60 .79
		Bhvr	Behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 440
		Part	CustomizeablePowers\DarkArmor\sparklies.part
		Part	CustomizeablePowers\DarkArmor\sparkliesStar.part
	End


#LegL



	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	CustomizeablePowers\DarkArmor\LiteSmoke.part
		part2	CustomizeablePowers\DarkArmor\LiteSmoke2.part
		Part3	CustomizeablePowers\DarkArmor\LiteDarkBlastHitTendril.part
		Part	CustomizeablePowers\DarkArmor\sparklies.part
		Part	CustomizeablePowers\DarkArmor\sparkliesStar.part
		Sound whirlwind_loop 60 60 .42
		Bhvr	Behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 280


	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	CustomizeablePowers\DarkArmor\LiteSmoke.part
		part2	CustomizeablePowers\DarkArmor\LiteSmoke2.part
		Part3	CustomizeablePowers\DarkArmor\LiteDarkBlastHitTendril.part
		Part	CustomizeablePowers\DarkArmor\sparklies.part
		Part	CustomizeablePowers\DarkArmor\sparkliesStar.part
	End

	Event
		EName	5
		Type	Local
		At	FootL
		part1	CustomizeablePowers\DarkArmor\LiteSmoke.part
		part2	CustomizeablePowers\DarkArmor\LiteSmoke2.part
		Part3	CustomizeablePowers\DarkArmor\LiteDarkBlastHitTendril.part
		Part	CustomizeablePowers\DarkArmor\sparklies.part
		Part	CustomizeablePowers\DarkArmor\sparkliesStar.part
		Sound rage 100 100 1.0

	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	CustomizeablePowers\DarkArmor\LiteSmoke.part
		part2	CustomizeablePowers\DarkArmor\LiteSmoke2.part
		Part3	CustomizeablePowers\DarkArmor\LiteDarkBlastHitTendril.part
		Part	CustomizeablePowers\DarkArmor\sparklies.part
		Part	CustomizeablePowers\DarkArmor\sparkliesStar.part

	End


#LegR


	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	CustomizeablePowers\DarkArmor\LiteSmoke.part
		part2	CustomizeablePowers\DarkArmor\LiteSmoke2.part
		Part3	CustomizeablePowers\DarkArmor\LiteDarkBlastHitTendril.part
		Part	CustomizeablePowers\DarkArmor\sparklies.part
		Part	CustomizeablePowers\DarkArmor\sparkliesStar.part
	End


###############################################################################

	Event
		EName	9
		Type	Local
		At	Head
		part1	CustomizeablePowers\DarkArmor\LiterSmoke.part
		part2	CustomizeablePowers\DarkArmor\LiterSmoke2.part
		Part3	CustomizeablePowers\DarkArmor\LiterDarkBlastHitTendril.part
		Part	CustomizeablePowers\DarkArmor\sparklies.part
		Part	CustomizeablePowers\DarkArmor\sparkliesStar.part
	End


##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	CustomizeablePowers\DarkArmor\LiterSmoke.part
		part2	CustomizeablePowers\DarkArmor\LiterSmoke2.part
		Part3	CustomizeablePowers\DarkArmor\LiterDarkBlastHitTendril.part
		Part	CustomizeablePowers\DarkArmor\sparklies.part
		Part	CustomizeablePowers\DarkArmor\sparkliesStar.part

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	CustomizeablePowers\DarkArmor\LiterSmoke.part
		part2	CustomizeablePowers\DarkArmor\LiterSmoke2.part
		Part3	CustomizeablePowers\DarkArmor\LiterDarkBlastHitTendril.part
		Part	CustomizeablePowers\DarkArmor\sparklies.part
		Part	CustomizeablePowers\DarkArmor\sparkliesStar.part
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part1	CustomizeablePowers\DarkArmor\LiterSmoke.part
		part2	CustomizeablePowers\DarkArmor\LiterSmoke2.part
		Part3	CustomizeablePowers\DarkArmor\LiterDarkBlastHitTendril.part
		Part	CustomizeablePowers\DarkArmor\sparklies.part
		Part	CustomizeablePowers\DarkArmor\sparkliesStar.part

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	CustomizeablePowers\DarkArmor\LiterSmoke.part
		part2	CustomizeablePowers\DarkArmor\LiterSmoke2.part
		Part3	CustomizeablePowers\DarkArmor\LiterDarkBlastHitTendril.part
		Part	CustomizeablePowers\DarkArmor\sparklies.part
		Part	CustomizeablePowers\DarkArmor\sparkliesStar.part
	End

End

################################################################################
##Lens Flares
###########################################################

Condition
	On 	cycle
	Time 	20
	Chance	.3

	Event
		EName	3
		Type	Local
		At	LarmL
		part1	CustomizeablePowers\DarkArmor\FortitudeRaysSmall.part
		part2	CustomizeablePowers\DarkArmor\FortitudeRing.part
		part3	CustomizeablePowers\DarkArmor\FortitudeRing.part
		part4	CustomizeablePowers\DarkArmor\FortitudeDots.part

		lifespan	25
	End

End

Condition
	On 	cycle
	Time 	19
	Chance	.3

	Event
		EName	3
		Type	Local
		At	LarmR
		part1	CustomizeablePowers\DarkArmor\FortitudeRaysSmall.part
		part2	CustomizeablePowers\DarkArmor\FortitudeRing.part
		part3	CustomizeablePowers\DarkArmor\FortitudeRing.part
		part4	CustomizeablePowers\DarkArmor\FortitudeDots.part

		lifespan	25
	End

End

Condition
	On 	cycle
	Time 	21
	Chance	.3

	Event
		EName	3
		Type	Local
		At	UarmL
		part1	CustomizeablePowers\DarkArmor\FortitudeRaysSmall.part
		part2	CustomizeablePowers\DarkArmor\FortitudeRing.part
		part3	CustomizeablePowers\DarkArmor\FortitudeRing.part
		part4	CustomizeablePowers\DarkArmor\FortitudeDots.part

		lifespan	25
	End

End


Condition
	On 	cycle
	Time 	22
	Chance	.3

	Event
		EName	3
		Type	Local
		At	UarmR
		part1	CustomizeablePowers\DarkArmor\FortitudeRaysSmall.part
		part2	CustomizeablePowers\DarkArmor\FortitudeRing.part
		part3	CustomizeablePowers\DarkArmor\FortitudeRing.part
		part4	CustomizeablePowers\DarkArmor\FortitudeDots.part

		lifespan	25
	End


End

Condition
	On 	cycle
	Time 	17
	Chance	.3

	Event
		EName	3
		Type	Local
		At	LlegL
		part1	CustomizeablePowers\DarkArmor\FortitudeRaysSmall.part
		part2	CustomizeablePowers\DarkArmor\FortitudeRing.part
		part3	CustomizeablePowers\DarkArmor\FortitudeRing.part
		part4	CustomizeablePowers\DarkArmor\FortitudeDots.part

		lifespan	25
	End

End

Condition
	On 	cycle
	Time 	18
	Chance	.3

	Event
		EName	3
		Type	Local
		At	LlegR
		part1	CustomizeablePowers\DarkArmor\FortitudeRaysSmall.part
		part2	CustomizeablePowers\DarkArmor\FortitudeRing.part
		part3	CustomizeablePowers\DarkArmor\FortitudeRing.part
		part4	CustomizeablePowers\DarkArmor\FortitudeDots.part

		lifespan	25
	End


End

#######################################

End

