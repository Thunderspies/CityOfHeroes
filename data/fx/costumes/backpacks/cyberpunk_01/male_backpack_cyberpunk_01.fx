#############################################################
## HEADER
#############################################################

FxInfo

Flags InheritAlpha DontSuppress //InheritAnimScale

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Chest

		Geom	FX_BackDetail1_Cyberpunk_01
		BhvrOverride
			Scale			1 1 1
			PositionOffset		0 0 0
#			PYRrotate 		0 0 0
# 			PYRrotateJitter 	0 0 0
			TintGeom		1
		End
	End
End

#############################################################

End