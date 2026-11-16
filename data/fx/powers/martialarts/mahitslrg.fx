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
		Part1	:MAHitStar01Lrg.part
		Part2	:MAHitStar02Lrg.part
		Part3	:MAHitStar03Lrg.part
		Part4	:MAHitGlow01Lrg.part
		Part5	:MAHitRays01Lrg.part
		Sound Hit6 100.0 100.0 .9

	End

End


End
