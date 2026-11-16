#############################################################
## ChestCannon_Male.fx
#############################################################

FxInfo

Flags InheritAlpha DontSuppress DontInheritTexFromCostume

#############################################################
## CHEST CANNON
#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	ChestCannon
		Type	Local
		At	Chest
		Anim	FX_PraetorianClockwork_ChestCannon_Male
	End
End

#############################################################

#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		Type	Local
#		At	ChestCannon
#		AnimPiv	Eyes
#		#Geom	GEO_FX_TestSphere
#	End
#End

#############################################################

End