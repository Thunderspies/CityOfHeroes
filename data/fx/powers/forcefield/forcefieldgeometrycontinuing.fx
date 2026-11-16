#############################################################
## ForceFieldGeometryContinuing.fx
#############################################################

FxInfo

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On 	Time
	Time 	5

	Event
		Type	Posit
		At	HIPS
		Bhvr 	behaviors\powers\forcefield\ForceBubbleA.bhvr
		Geom	FX_ForceBubble
	End
End

#############################################################

End