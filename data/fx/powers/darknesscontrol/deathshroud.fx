#########################################################
## Dark Armor - Death Shroud
#########################################################

FxInfo

#########################################################

Input
	Inpname	Hips
End

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
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan	700
	End

	Event
		EName	3
		Type	Local
		At	Root
		Sound miasma_loop 60 60 .55
		bhvr	behaviors/soundFade3.bhvr
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
		part1	:LiteSmoke.part
		part2	:LiteSmoke2.part
		Part3	:DarkBlastHitTendrilDown.part
	End

	Event
		EName	5
		Type	Local
		At	FootL
		part1	:LiteSmoke.part
		part2	:LiteSmoke2.part
		Part3	:DarkBlastHitTendrilDown.part
	End

	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	:LiteSmoke.part
		part2	:LiteSmoke2.part
		Part3	:DarkBlastHitTendrilDown.part
	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	:LiteSmoke.part
		part2	:LiteSmoke2.part
		Part3	:DarkBlastHitTendrilDown.part
	End


	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	:LiteSmoke.part
		part2	:LiteSmoke2.part
		Part3	:DarkBlastHitTendrilDown.part
	End

	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	:LiteSmoke.part
		part2	:LiteSmoke2.part
		Part3	:DarkBlastHitTendrilDown.part
	End
End

#########################################################

Condition
	On	Time
	Time	75

	Event
		Type	Local
		At	Root
		part	:DarkArmorDamageRing.Part
		Part1	:DarkArmorRing.Part
		part3	:SmokeOut.part
		part4	:Smoke2Out.part
		Sound rage 60 60 .75
	End
End

#########################################################


End