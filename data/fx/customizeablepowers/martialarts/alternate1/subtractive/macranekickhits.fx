#############################################################
## MACraneKickHits.fx
#############################################################

FxInfo
Lifespan 100

########################################################


Condition
	On 	Time
	Time 	5

	Event
		EName 	HitFX
		Type	Start
		At	Chest
		Part1	CustomizeablePowers\MartialArts\MAHitStar01Lrg.part
		Part2	CustomizeablePowers\MartialArts\MAHitStar02Lrg.part
		Part3	CustomizeablePowers\MartialArts\MAHitStar03Lrg.part
		Part4	CustomizeablePowers\MartialArts\MAHitGlow01Lrg.part
		Part5	CustomizeablePowers\MartialArts\MAHitRays01Lrg.part
		Sound Hit3 100.0 100.0 .7

	End

End

Condition
	On 	Time
	Time 	2

	Event
		EName 	HitFX
		Type	Posit
		At	Chest
		Part1	CustomizeablePowers\MartialArts\MACraneKickTrail.part
		Sound thunderhit 100.0 100.0 .9
		Lifespan 16

	End

End

Condition
	On 	Time
	Time 	23

	Event
		EName 	HitFX
		Type	Local
		At	Chest
		Part1	CustomizeablePowers\MartialArts\MACraneKickDust.part


	End

End

Condition
	On 	Time
	Time 	25

	Event
		EName 	HitFX
		Type	Local
		At	Head
		Part1	CustomizeablePowers\MartialArts\MACraneKickDust.part


	End

End

Condition
	On 	Time
	Time 	24

	Event
		EName 	HitFX
		Type	Local
		At	Llegl
		Part1	CustomizeablePowers\MartialArts\MACraneKickDust.part


	End

End

Condition
	On 	Time
	Time 	26

	Event
		EName 	HitFX
		Type	Local
		At	LlegR
		Part1	CustomizeablePowers\MartialArts\MACraneKickDust.part


	End

End

Condition
	On 	Time
	Time 	23

	Event
		EName 	HitFX
		Type	Local
		At	Hips
		Part1	CustomizeablePowers\MartialArts\MACraneKickDust.part


	End

End


End
