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
		EName	Pow
		Type 	Local
		At	Chest
		Part	:HitFlash.part
		Part	:HitFlare.part
		Sound CoL_LightStaff_Hit_01 80.0 80.0 .8
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