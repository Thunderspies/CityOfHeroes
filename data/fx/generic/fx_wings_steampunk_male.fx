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
		Bhvr	behaviors/WingScale_Angel_Male.bhvr
		Anim	FX_wings_STEAMPUNK
		BhvrOverride
			Scale			1.3 1.3 1.3
			PositionOffset		0 0 0.09
#			PYRrotate 		0 0 0
# 			PYRrotateJitter 	0 0 0
		End
	End
End

#############################################################

End