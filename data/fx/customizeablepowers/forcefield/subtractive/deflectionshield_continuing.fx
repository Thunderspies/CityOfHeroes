#############################################################
## DeflectionShield_Continuing.FX
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	5

	Event
		EName 	Prime2
		Type	Posit
		At	HIPS
		Bhvr	CustomizeablePowers\ForceField\ForceBubbleA.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	tintable_FX_ForceBubble
	End
End

#############################################################

End