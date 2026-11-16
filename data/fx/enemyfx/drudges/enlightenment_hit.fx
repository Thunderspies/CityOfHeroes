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
		Part	:HitFlash_Soul.part
		Part	:HitFlare_Soul.part
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
		Part	:HitFlash_Secondary_Soul.part
		Part	:HitStreaks_Soul.part
		Lifespan 2
	End
End

#############################################################
End