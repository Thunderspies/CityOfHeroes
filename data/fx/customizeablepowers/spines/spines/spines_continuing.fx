#############################################################
## Spines_Continuing.fx
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
		Type	Create
		Inherit	ALL
		Update	Position Rotation
		At	chest
		Bhvr	CustomizeablePowers\Spines\Spines\QuillProjectile4.bhvr
		BhvrOverride
			Scale			0.25 0.25 0.25
			TintGeom		1
		End
		Geom	Tintable_Spines_Hit
	End
End

#############################################################

End