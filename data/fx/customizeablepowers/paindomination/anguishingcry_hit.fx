#############################################################
## AnguishingCry_Hit.fx
#############################################################

FxInfo

LifeSpan 30

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
		Part1	CustomizeablePowers\PainDomination\MAHitStar01Lrg.part
		Part2	CustomizeablePowers\PainDomination\MAHitStar02Lrg.part
		Part3	CustomizeablePowers\PainDomination\MAHitStar03Lrg.part
		Part4	CustomizeablePowers\PainDomination\MAHitGlow01Lrg.part
		Part5	CustomizeablePowers\PainDomination\MAHitRays01Lrg.part
		#Sound	Hit6 100.0 30.0 .9
	End
End

#############################################################

End