#############################################################
## DarkArmorNoRing.fx
#############################################################

FxInfo

##################################


Condition
	On	Time
	Time 	0

	Event
		Type 	Local
		At	FootR
		Sound darkmoonbeam3 60 60 .56

	End

End

Condition
	On	Time
	Time 	60

	Event
		Type 	Local
		At	FootR
		Sound miasma_loop 60 60 .42
		Bhvr	Behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 400
	End

End

Condition
	On	Time
	Time 	60

	Event
		Type 	Local
		At	FootR
		Sound obsidian_loop 60 60 .24
		Bhvr	Behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 400
	End

End

Condition
	On	Time
	Time	60

	Event
		EName	2
		Type	Local
		At	FootR
		part1	CustomizeablePowers\DarkArmor\LiteSmoke.part
		part2	CustomizeablePowers\DarkArmor\LiteSmoke2.part
		Part3	CustomizeablePowers\DarkArmor\LiteDarkBlastHitTendril.part


	End


#LegL



	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	CustomizeablePowers\DarkArmor\LiteSmoke.part
		part2	CustomizeablePowers\DarkArmor\LiteSmoke2.part
		Part3	CustomizeablePowers\DarkArmor\LiteDarkBlastHitTendril.part
		#PMagnet LLEGL


	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	CustomizeablePowers\DarkArmor\LiteSmoke.part
		part2	CustomizeablePowers\DarkArmor\LiteSmoke2.part
		Part3	CustomizeablePowers\DarkArmor\LiteDarkBlastHitTendril.part
		#PMagnet FootL
	End

	Event
		EName	5
		Type	Local
		At	FootL
		part1	CustomizeablePowers\DarkArmor\LiteSmoke.part
		part2	CustomizeablePowers\DarkArmor\LiteSmoke2.part
		Part3	CustomizeablePowers\DarkArmor\LiteDarkBlastHitTendril.part
		Sound rage 100 100 1.0

	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	CustomizeablePowers\DarkArmor\LiteSmoke.part
		part2	CustomizeablePowers\DarkArmor\LiteSmoke2.part
		Part3	CustomizeablePowers\DarkArmor\LiteDarkBlastHitTendril.part
		#PMagnet LlegR

	End


#LegR


	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	CustomizeablePowers\DarkArmor\LiteSmoke.part
		part2	CustomizeablePowers\DarkArmor\LiteSmoke2.part
		Part3	CustomizeablePowers\DarkArmor\LiteDarkBlastHitTendril.part
		#PMagnet FootR
	End


###############################################################################

	Event
		EName	9
		Type	Local
		At	Head
		part1	CustomizeablePowers\DarkArmor\LiterSmoke.part
		part2	CustomizeablePowers\DarkArmor\LiterSmoke2.part
		Part3	CustomizeablePowers\DarkArmor\LiterDarkBlastHitTendril.part
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
		#PMagnet LArmL

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	CustomizeablePowers\DarkArmor\LiterSmoke.part
		part2	CustomizeablePowers\DarkArmor\LiterSmoke2.part
		Part3	CustomizeablePowers\DarkArmor\LiterDarkBlastHitTendril.part
		#PMagnet WepL
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part1	CustomizeablePowers\DarkArmor\LiterSmoke.part
		part2	CustomizeablePowers\DarkArmor\LiterSmoke2.part
		Part3	CustomizeablePowers\DarkArmor\LiterDarkBlastHitTendril.part
		#PMagnet LArmR

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	CustomizeablePowers\DarkArmor\LiterSmoke.part
		part2	CustomizeablePowers\DarkArmor\LiterSmoke2.part
		Part3	CustomizeablePowers\DarkArmor\LiterDarkBlastHitTendril.part
		#PMagnet WepR
	End

End


End

