#############################################################
## EaglesClawHits.fx
#############################################################

FxInfo
LifeSpan	100

#########################################################

Input
	Inpname	Hips
End

#########################################################


Condition
	On 	Time
	Time 	3

	Event
		EName 	HitFX
		Type	local
		At	Chest
		Part1	CustomizeablePowers\MartialArts\Subtractive\MAHitStar01Lrg.part
		Part2	CustomizeablePowers\MartialArts\Subtractive\MAHitStar02Lrg.part
		Part3	CustomizeablePowers\MartialArts\Subtractive\MAHitStar03Lrg.part
		Part4	CustomizeablePowers\MartialArts\Subtractive\MAHitGlow01Lrg.part
		Part5	CustomizeablePowers\MartialArts\Subtractive\MAHitRays01Lrg.part
		Sound thunderhit 100.0 100.0 .7
		Lifespan 100

	End

End

Condition
	On 	Time
	Time 	3

	Event
		EName 	HitFX
		Type	local
		At	Chest
		Part1	CustomizeablePowers\MartialArts\Subtractive\MAEaglesClawShards01.part

		Lifespan 5

	End

End

Condition
	On 	Time
	Time 	0

	Event
		EName 	HitFX
		Type	local
		At	Chest
		Part1	CustomizeablePowers\MartialArts\Subtractive\MAEaglesClawHitsRing01.part
		Sound hit5 60 60 .6

		Lifespan 5

	End

End

Condition
	On 	Time
	Time 	3

	Event
		EName 	HitFX
		Type	local
		At	Chest
		Part1	CustomizeablePowers\MartialArts\Subtractive\MAEaglesClawHitsRing01.part

		Lifespan 5

	End

End


End
