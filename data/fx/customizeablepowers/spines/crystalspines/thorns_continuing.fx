#############################################################
## Impale_Continuing.FX
#############################################################

FxInfo

ClampMinScale 0.75 0.75 0.75
ClampMaxScale 1.25 1.25 1.25

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On 	Time
	Time 	0

	Event

		Type	Local
		At	chest
		Sound Quills6 70 70 0.7
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Create
		Inherit	ALL
		Update	Position Rotation
		At	chest
		At	chest
		Bhvr	CustomizeablePowers\Spines\Thorns\QuillHit01.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_Thorns_Hit
	End
End

#############################################################

End