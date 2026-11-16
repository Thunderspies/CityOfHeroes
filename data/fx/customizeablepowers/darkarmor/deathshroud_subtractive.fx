#########################################################
## Dark Armor - Death Shroud (Subtractive)
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
		Type	Local
		At	Root
		Sound forcefield5_loop 60 60 .44
		Bhvr	Behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan	700
	End

	Event
		Type	Local
		At	Root
		Sound forcefield5_loop 60 60 .44
		Bhvr	Behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan	700
	End
End

## POWER FX #######################################################

Condition
	On	Time
	Time	60

	Event
		Type	Local
		At	FootR
		part1	CustomizeablePowers\DarkArmor\LiteSmoke.part
		part2	CustomizeablePowers\DarkArmor\LiteSmoke2.part
		part2	CustomizeablePowers\DarkArmor\LiteSmoke_Dark.part
		Part3	CustomizeablePowers\DarkArmor\DarkBlastHitTendrilDown_Subtractive.part
	End

	Event
		Type	Local
		At	FootL
		part1	CustomizeablePowers\DarkArmor\LiteSmoke.part
		part2	CustomizeablePowers\DarkArmor\LiteSmoke2.part
		part2	CustomizeablePowers\DarkArmor\LiteSmoke_Dark.part
		Part3	CustomizeablePowers\DarkArmor\DarkBlastHitTendrilDown_Subtractive.part
	End

	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	CustomizeablePowers\DarkArmor\LiteSmoke.part
		part2	CustomizeablePowers\DarkArmor\LiteSmoke2.part
		part2	CustomizeablePowers\DarkArmor\LiteSmoke_Dark.part
		Part3	CustomizeablePowers\DarkArmor\DarkBlastHitTendrilDown_Subtractive.part
	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	CustomizeablePowers\DarkArmor\LiteSmoke.part
		part2	CustomizeablePowers\DarkArmor\LiteSmoke2.part
		part2	CustomizeablePowers\DarkArmor\LiteSmoke_Dark.part
		Part3	CustomizeablePowers\DarkArmor\DarkBlastHitTendrilDown_Subtractive.part
	End

	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	CustomizeablePowers\DarkArmor\LiteSmoke.part
		part2	CustomizeablePowers\DarkArmor\LiteSmoke2.part
		part2	CustomizeablePowers\DarkArmor\LiteSmoke_Dark.part
		Part3	CustomizeablePowers\DarkArmor\DarkBlastHitTendrilDown_Subtractive.part
	End

	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	CustomizeablePowers\DarkArmor\LiteSmoke.part
		part2	CustomizeablePowers\DarkArmor\LiteSmoke2.part
		part2	CustomizeablePowers\DarkArmor\LiteSmoke_Dark.part
		Part3	CustomizeablePowers\DarkArmor\DarkBlastHitTendrilDown_Subtractive.part
	End
End

#########################################################

Condition
	On	Time
	Time	75

	Event
		EName	9a5
		Type	Local
		At	Root
		part	CustomizeablePowers\DarkArmor\DarkArmorDamageRing.Part
		Part1	CustomizeablePowers\DarkArmor\DarkArmorRing.Part
		part2	CustomizeablePowers\DarkArmor\DarkBlastHitTendrilOut_Subtractive.part
		part3	CustomizeablePowers\DarkArmor\SmokeOut.part
		part4	CustomizeablePowers\DarkArmor\Smoke2Out.part
		part	CustomizeablePowers\DarkArmor\SmokeOut_Dark.part

		Sound rage 60 60 .75
	End
End

#########################################################

End