#############################################################
## Fireflies_Body.fx
#############################################################

FxInfo

#########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	SorceryPowerPool_MysticFlight_01 100 100 .8
	End
End

Condition
	On	Time
	Time	0

	Event
		Ename	SpinAnchor
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset 0 1 0
			Spin	0.0 0.05 0.0
		End
		ChildFX	:CHILD_Continuous_Body.fx
		Lifespan 80
	End
End

Condition
	On	Cycle
	Time	20

	Event
		Ename	SpinAnchor
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset 0 1 0
			Spin	0.0 0.05 0.0
		End
		ChildFX	:CHILD_Continuous_Body.fx
		Lifespan 80
	End
End

Condition
	On	Cycle
	Time	30
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