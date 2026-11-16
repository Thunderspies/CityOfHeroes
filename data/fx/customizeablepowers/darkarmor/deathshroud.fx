#########################################################
## Dark Armor - Death Shroud
#########################################################

FxInfo

## SOUNDS #######################################################

Condition

	On	Time
	Time	28

	Event
		Type	Local
		At	Origin
		Sound darkarmor2 60 60 .88
	End
End

Condition
	On	Time
	Time	60
	
	Event
		EName	2
		Type	Local
		At	Root
		Sound forcefield5_loop 60 60 .44
		Bhvr	Behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan	700
	End

	Event
		EName	3
		Type	Local
		At	Root
		Sound miasma_loop 60 60 .55
		Bhvr	Behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan	760
	End
End

## POWER FX #######################################################

Condition
	On	Time
	Time	60
	
	Event
		EName	2
		Type	Local
		At	FootR
		part1	CustomizeablePowers\DarkArmor\LiteSmoke.part
		part2	CustomizeablePowers\DarkArmor\LiteSmoke2.part
		Part3	CustomizeablePowers\DarkArmor\DarkBlastHitTendrilDown.part
	End

	Event
		EName	5		
		Type	Local
		At	FootL
		part1	CustomizeablePowers\DarkArmor\LiteSmoke.part
		part2	CustomizeablePowers\DarkArmor\LiteSmoke2.part
		Part3	CustomizeablePowers\DarkArmor\DarkBlastHitTendrilDown.part
	End

	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	CustomizeablePowers\DarkArmor\LiteSmoke.part
		part2	CustomizeablePowers\DarkArmor\LiteSmoke2.part
		Part3	CustomizeablePowers\DarkArmor\DarkBlastHitTendrilDown.part
	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	CustomizeablePowers\DarkArmor\LiteSmoke.part
		part2	CustomizeablePowers\DarkArmor\LiteSmoke2.part
		Part3	CustomizeablePowers\DarkArmor\DarkBlastHitTendrilDown.part
	End


	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	CustomizeablePowers\DarkArmor\LiteSmoke.part
		part2	CustomizeablePowers\DarkArmor\LiteSmoke2.part
		Part3	CustomizeablePowers\DarkArmor\DarkBlastHitTendrilDown.part
	End

	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	CustomizeablePowers\DarkArmor\LiteSmoke.part
		part2	CustomizeablePowers\DarkArmor\LiteSmoke2.part
		Part3	CustomizeablePowers\DarkArmor\DarkBlastHitTendrilDown.part
	End
End    

#########################################################

Condition
	On	Time
	Time	75

	Event
		Type	Local
		At	Root
		part	CustomizeablePowers\DarkArmor\DarkArmorDamageRing.Part
		Part1	CustomizeablePowers\DarkArmor\DarkArmorRing.Part
		part3	CustomizeablePowers\DarkArmor\SmokeOut.part
		part4	CustomizeablePowers\DarkArmor\Smoke2Out.part
		Sound rage 60 60 .75
	End
End

#########################################################


End