#########################################################
##	template

FxInfo

Flags InheritAlpha ###InheritAnimScale

Condition
	On 	Time
	Time 	1

	Event
		Type	Local
		At	Back
		Anim	FX_wings_tiny
		BhvrOverride
			Scale		0.2 0.2 0.2
			PositionOffset	0 .3 -.1
			PyrRotate		30 0 0
		End		
	End
End

End