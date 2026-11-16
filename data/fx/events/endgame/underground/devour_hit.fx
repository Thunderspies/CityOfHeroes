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
		Part1	:MAHitStar01Lrg.part
		Part2	:MAHitStar02Lrg.part
		Part3	:MAHitStar03Lrg.part
		Part4	:MAHitGlow01Lrg.part
		Part5	:MAHitRays01Lrg.part
		Sound Devour_Hit_01 100.0 100.0 .5
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
		Part1	:MAEaglesClawShards01.part

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
		Part1	:MAEaglesClawHitsRing01.part
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
		Part1	:MAEaglesClawHitsRing01.part

		Lifespan 5

	End

End


End
