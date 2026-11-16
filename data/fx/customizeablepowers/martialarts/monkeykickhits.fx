#############################################################
## MonkeyKickHits.fx
#############################################################

FxInfo
Lifespan 100

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On 	Time
	Time 	2

	Event
		EName 	HitFX
		Type	Start
		At	LlegR
		Part1	CustomizeablePowers\MartialArts\MAHitStar01Lrg.part
		Part2	CustomizeablePowers\MartialArts\MAHitStar02Lrg.part
		Part3	CustomizeablePowers\MartialArts\MAHitStar03Lrg.part
		Part4	CustomizeablePowers\MartialArts\MAHitGlow01Lrg.part
		Part5	CustomizeablePowers\MartialArts\MAHitRays01Lrg.part
	End
End

Condition
	On 	Time
	Time 	34

	Event
		EName 	HitFX
		Type	Local
		At	Chest
		Part1	CustomizeablePowers\MartialArts\MACraneKickDust.part
	End
End

Condition
	On 	Time
	Time 	33

	Event
		EName 	HitFX
		Type	Local
		At	Head
		Part1	CustomizeablePowers\MartialArts\MACraneKickDust.part
	End
End

Condition
	On 	Time
	Time 	35

	Event
		EName 	HitFX
		Type	Local
		At	Llegl
		Part1	CustomizeablePowers\MartialArts\MACraneKickDust.part
	End
End

Condition
	On 	Time
	Time 	36

	Event
		EName 	HitFX
		Type	Local
		At	LlegR
		Part1	CustomizeablePowers\MartialArts\MACraneKickDust.part
	End
End

Condition
	On 	Time
	Time 	35

	Event
		EName 	HitFX
		Type	Local
		At	Hips
		Part1	CustomizeablePowers\MartialArts\MACraneKickDust.part
	End
End

#############################################################

End