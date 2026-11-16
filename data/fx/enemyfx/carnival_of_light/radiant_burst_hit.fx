#############################################################
## Radiant_Burst_Hit.fx
#############################################################

FxInfo
Lifespan 90
#########################################################

Condition
	On	Time
	Time	0

	Event
		EName	Pow
		Type 	Local
		At	Chest
		Part	:HitFlash.part
		Part	:HitFlare.part
		Lifespan 2
	End
End

Condition
	On	Time
	Time	4

	Event
		EName	Pow
		Type 	Local
		At	Chest
		Part	:HitFlash_Secondary.part
		Part	:HitStreaks.part
		Lifespan 2
	End
End

#############################################################
End