#############################################################
## Spines.fx
#############################################################

FxInfo
#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	root
		Bhvr	Behaviors\caltropsFadein.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	SlateSpines_Caltrops
	End
End

#############################################################

End