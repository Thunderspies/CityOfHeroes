#############################################################
## EarthEmote_CHILD.fx
#############################################################

FxInfo
#############################################################


Condition
	On 	Time
	Time 	0

	Event
		Type	PositOnly
		At	Root
		BhvrOverride
			PyrRotateJitter	0 180 0
		End
		ChildFX	:CHILD_EarthUp.fx
	End
End

Condition
	On 	Time
	Time 	2

	Event
		Type	PositOnly
		At	Root
		BhvrOverride
			PyrRotateJitter	0 180 0
		End
		ChildFX	:CHILD_EarthUp.fx
	End
End

Condition
	On 	Time
	Time 	3

	Event
		Type	PositOnly
		At	Root
		BhvrOverride
			PyrRotateJitter	0 180 0
		End
		ChildFX	:CHILD_EarthUp.fx
	End
End

Condition
	On 	Time
	Time 	5

	Event
		Type	PositOnly
		At	Root
		BhvrOverride
			PyrRotateJitter	0 180 0
		End
		ChildFX	:CHILD_EarthUp.fx
	End
End
#############################################################
End