#############################################################
## HEADER
#############################################################

FxInfo

Flags InheritAlpha InheritAnimScale DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Back
		BhvrOverride
			PositionOffset	0.0 0.0 -0.25
			PyrRotate	0.0 0.0 0.0
			Scale		1.25 1.25 1.25
		End
		Anim	Player_Crab_Backpack2_01
	End
End

#############################################################

End