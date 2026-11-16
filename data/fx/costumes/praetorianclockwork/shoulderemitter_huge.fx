#############################################################
## ShoulderEmitter_Male.fx
#############################################################

FxInfo

Flags InheritAlpha DontSuppress DontInheritTexFromCostume

#############################################################
## SHOULDER EMITTER
#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	ShoulderEmitter
		Type	Local
		At	Chest
		Anim	FX_PraetorianClockwork_ShoulderEmitter_Huge
	End
End

#############################################################

#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		Type	Local
#		At	ShoulderEmitter
#		AnimPiv	Hair
#		#Geom	GEO_FX_TestSphere
#	End
#End

#############################################################

End