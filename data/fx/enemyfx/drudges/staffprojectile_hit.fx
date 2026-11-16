#############################################################
## HEADER
#############################################################

FxInfo

Lifespan 90

#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		Sound 	CoL_LightStaffBolt_Hit_01 100 100 .8
	End


	Event
		EName	Pow
		Type 	Local
		At	Chest
		Part	:HitFlash_Soul.part
		Part	:ProjectileHitFlare_Soul.part
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
		Part	:ProjectileHitFlash_Secondary_Soul.part
		Part	:ProjectileHitStreaks_Soul.part
		Part	:ProjectileHitStreaks2_Soul.part
		Lifespan 10
	End
End


#############################################################

End