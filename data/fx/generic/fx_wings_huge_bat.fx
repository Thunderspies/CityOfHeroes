#############################################################
## Header
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	1

	Event
		Type	Local
		At	Back
		Bhvr	behaviors/WingScale_BAT_Huge.bhvr
		BhvrOverride
			PositionOffset	0 .35 -.35 #0 .3 -.2 #0 .5 -.42
			PYRrotate 	-10 0 0
		End
		Anim	FX_wings_BAT
	End
End

#############################################################

End