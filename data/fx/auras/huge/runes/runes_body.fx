#############################################################
## Fireflies_Body.fx
#############################################################

FxInfo

#########################################################

Condition
	On	Cycle
	Time	20

	Event
		Ename	SpinAnchor
		Type	Local
		At	Origin
		BhvrOverride
			Spin	0.0 0.05 0.0
		End
		ChildFX	:CHILD_Continuous_Body.fx
		Lifespan 80
	End
End

Condition
	On	Cycle
	Time	15
	Random	1

	Event
		Type	Start
		At	SpinAnchor
		ChildFX	:CHILD_Runes_Body.fx
		Lifespan 90
	End

	Event
		Type	Start
		At	SpinAnchor
		ChildFX	:CHILD_Runes2_Body.fx
		Lifespan 90
	End

	Event
		Type	Start
		At	SpinAnchor
		ChildFX	:CHILD_Runes3_Body.fx
		Lifespan 90
	End

	Event
		Type	Start
		At	SpinAnchor
		ChildFX	:CHILD_Runes4_Body.fx
		Lifespan 90
	End

	Event
		Type	Start
		At	SpinAnchor
		ChildFX	:CHILD_Runes5_Body.fx
		Lifespan 90
	End

	Event
		Type	Start
		At	SpinAnchor
		ChildFX	:CHILD_Runes6_Body.fx
		Lifespan 90
	End

	Event
		Type	Start
		At	SpinAnchor
		ChildFX	:CHILD_Runes7_Body.fx
		Lifespan 90
	End
End

#############################################################

End