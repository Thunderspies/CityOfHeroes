#############################################################
## WristBlaster_Huge.fx
#############################################################

FxInfo

Flags InheritAlpha DontSuppress DontInheritTexFromCostume

#############################################################
## WRIST BLASTER
#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	WristBlaster
		Type	Local
		At	LArmR
		Anim	FX_PraetorianClockwork_WristBlaster_Huge
	End
End

#############################################################

#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		Type	Local
#		At	WristBlaster
#		AnimPiv	Neck
#		#Geom	GEO_FX_TestSphere
#	End
#
#	Event
#		Type	Local
#		At	WristBlaster
#		AnimPiv	Head
#		#Geom	GEO_FX_TestSphere
#	End
#End

#############################################################

End