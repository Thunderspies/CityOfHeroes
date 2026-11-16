#############################################################
## EarthEmote_CHILD.fx
#############################################################

FxInfo
#############################################################


Condition
	On 	Time
	Time 	0
	Repeat 2

	Event
		Type	Local
		At	Origin
		BhvrOverride
			PyrRotateJitter	0 180 0
			Scale		1.5 1.5 1.5
		End
		ChildFX	:EarthUp_CHILD.fx
	End
End

Condition
	On 	Time
	Time 	2

	Event
		Type	Local
		At	Origin
		BhvrOverride
			PyrRotateJitter	0 180 0
			Scale		1.5 1.5 1.5
		End
		ChildFX	:EarthUp_CHILD.fx
	End
End

Condition
	On 	Time
	Time 	3

	Event
		Type	Local
		At	Origin
		BhvrOverride
			PyrRotateJitter	0 180 0
			Scale		1.5 1.5 1.5
		End
		ChildFX	:EarthUp_CHILD.fx
	End
End

Condition
	On 	Time
	Time 	5

	Event
		Type	Local
		At	Origin
		BhvrOverride
			PyrRotateJitter	0 180 0
			Scale		1.5 1.5 1.5
		End
		ChildFX	:EarthUp_CHILD.fx
	End
End
#############################################################
End