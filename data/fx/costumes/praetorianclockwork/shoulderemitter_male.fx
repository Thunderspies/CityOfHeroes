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
		Anim	FX_PraetorianClockwork_ShoulderEmitter_Male
	End
End

#############################################################

End