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
		BhvrOverride
			PositionOffset	0 .3 -.27
		End
		Anim	FX_wings_bugs_blur
	End
End

#############################################################

End