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
			Scale			0.8 0.8 0.8
			PositionOffset		0 .08 .095
			PYRrotate 		7 0 0
# 			PYRrotateJitter 	0 0 0
			TintGeom		1
		End
	End
End

#############################################################

End