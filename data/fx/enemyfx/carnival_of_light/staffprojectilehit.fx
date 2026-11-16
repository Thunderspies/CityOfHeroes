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
		Part	:HitFlash.part
		Part	:ProjectileHitFlare.part
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
		Part	:ProjectileHitFlash_Secondary.part
		Part	:ProjectileHitStreaks.part
		Part	:ProjectileHitStreaks2.part
		Lifespan 10
	End
End


#############################################################

End