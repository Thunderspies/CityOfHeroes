#############################################################
## MAHitsLrg.fx
#############################################################

FxInfo
LifeSpan	30

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
		At	Chest
		Part1	CustomizeablePowers\MartialArts\MAHitStar01Lrg.part
		Part2	CustomizeablePowers\MartialArts\MAHitStar02Lrg.part
		Part3	CustomizeablePowers\MartialArts\MAHitStar03Lrg.part
		Part4	CustomizeablePowers\MartialArts\MAHitGlow01Lrg.part
		Sound Hit6 100.0 100.0 .9

	End

End


End
