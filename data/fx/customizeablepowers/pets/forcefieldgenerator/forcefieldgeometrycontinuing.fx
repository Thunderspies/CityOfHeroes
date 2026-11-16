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
		EName	Offset
		Type	Local
		At	Root
		Bhvr	behaviors\GenericParticleFade.bhvr
		Geom	RootToChestAdjustment
	End

	Event
		Type	Local
		At	Offset
		AltPiv	1
		Bhvr 	CustomizeablePowers\ForceField\ForceBubbleA.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_FX_ForceBubble
	End
End

#############################################################

End