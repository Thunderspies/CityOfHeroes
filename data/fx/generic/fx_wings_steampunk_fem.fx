#############################################################
## Header
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Back
		Bhvr	behaviors/WingScale_Tech_Male.bhvr
		Anim	FX_wings_STEAMPUNK
		BhvrOverride
			Scale			1.1 1.1 1.1
			PositionOffset		0 0 0.10
			PYRrotate 		0 0 0
 			PYRrotateJitter 	0 0 0
		End
	End
End

#############################################################

End