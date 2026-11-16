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
			PositionOffset	0.0 0.0 0.0
			PyrRotate	0.0 0.0 0.0
			Scale		0.675 0.675 0.675
		End
		Anim	Player_Crab_Backpack2_02
	End
End

#############################################################

End