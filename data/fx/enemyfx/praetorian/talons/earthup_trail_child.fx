#############################################################
## EarthEmote_CHILD.fx
#############################################################

FxInfo
#############################################################


Condition
	On 	Time
	Time 	3
	Repeat 2

	Event
		Type	Start
		At	Origin
		BhvrOverride
			PyrRotateJitter	90 180 0
			Scale		0.2 0.2 0.2
			PositionOffset	0 -1 0
		End
		ChildFX	:EarthUp_Trail_GRANDCHILD.fx
	End
End

Condition
	On 	Time
	Time 	7
	Repeat 2

	Event
		Type	Start
		At	Origin
		BhvrOverride
			PyrRotateJitter	90 180 0
			Scale		0.2 0.2 0.2
			PositionOffset	0 -1 0
		End
		ChildFX	:EarthUp_Trail_GRANDCHILD.fx
	End
End

Condition
	On 	Time
	Time 	10
	Repeat 2

	Event
		Type	Start
		At	Origin
		BhvrOverride
			PyrRotateJitter	90 180 0
			Scale		0.2 0.2 0.2
			PositionOffset	0 -1 0
		End
		ChildFX	:EarthUp_Trail_GRANDCHILD.fx
	End
End

Condition
	On 	Time
	Time 	13
	Repeat 2

	Event
		Type	Start
		At	Origin
		BhvrOverride
			PyrRotateJitter	90 180 0
			Scale		0.2 0.2 0.2
			PositionOffset	0 -1 0
		End
		ChildFX	:EarthUp_Trail_GRANDCHILD.fx
	End
End
#############################################################
End