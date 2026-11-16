#############################################################
## NeuronsTower_Portal.fx
#############################################################

FxInfo
Lifespan 330

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Offset
		Type	Start
		At	Origin
		BhvrOverride
			PositionOffset	0 3 0
			PyrRotate	90 90 90
		End
		Part	:Circle_Fill_Subtractive.part
		Lifespan 50
	End
End

Condition
	On 	Time
	Time 	30

	Event
		EName	Offset
		Type	Start
		At	Origin
		BhvrOverride
			PositionOffset	0 3 0
			PyrRotate	90 90 90
		End
		Part	:Circle_GrandvilleTower.part
		Lifespan 80
	End
End

Condition
	On 	Time
	Time 	125

	Event
		EName	Offset
		Type	Start
		At	Origin
		BhvrOverride
			PositionOffset	0 3 0
			PyrRotate	90 90 90
		End
		Part	:Circle_Fill_Subtractive.part
		Lifespan 75
	End
End

Condition
	On	Time
	Time	180

	Event
		EName	Offset
		Type	Start
		At	Origin
		BhvrOverride
			PositionOffset	0 -5 0
			PyrRotate	90 90 90
		End
		Part	:Circle_AtlasPark.part
		Lifespan 80
	End
End

Condition
	On 	Time
	Time 	280

	Event
		EName	Offset
		Type	Start
		At	Origin
		BhvrOverride
			PositionOffset	0 3 0
			PyrRotate	90 90 90
		End
		Part	:Circle_Fill_Subtractive.part
		Lifespan 50
	End
End
#############################################################

End