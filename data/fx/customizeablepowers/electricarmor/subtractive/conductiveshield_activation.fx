#############################################################
## ConductiveShield_Activation.fx
#############################################################

FxInfo

#LifeSpan 130

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		ChildFx	:Child_ElectricalSurge01.fx
		Sound elecbuildup3 70 70 0.7
		LifeSpan 10
	End
End

Condition
	On	Time
	Time	13

	Event
		Type	Local
		At	Origin
		Sound elecarmor4_loop 70 70 0.3
		bhvr	behaviors\soundFade6.bhvr
		Flags	PowerLoopingSound
		Lifespan 350
	End
End

#############################################################

Condition
	On	Time
	Time	10
	Repeat	2

	Event
		Type	Local
		At	Root
		ChildFx	:Child_ElectricalSurge01.fx
		LifeSpan 10
	End
End

Condition
	On	Time
	Time	13
	Repeat	2

	Event
		Type	Local
		At	Root
		ChildFx	:Child_ElectricalSurge01.fx
		LifeSpan 10
	End
End

Condition
	On	Time
	Time	15
	Repeat	3

	Event
		Type	Local
		At	Root
		ChildFx	:Child_ElectricalSurge01.fx
		LifeSpan 10
	End
End

Condition
	On	Time
	Time	17
	Repeat	2

	Event
		Type	Local
		At	Root
		ChildFx	:Child_ElectricalSurge01.fx
		LifeSpan 10
	End
End

Condition
	On	Time
	Time	20
	Repeat	3

	Event
		Type	Local
		At	Root
		ChildFx	:Child_ElectricalSurge01.fx
		LifeSpan 10
	End
End

Condition
	On	Time
	Time	35
	Repeat	1

	Event
		Type	Local
		At	Root
		ChildFx	:Child_ElectricalSurge01.fx
		LifeSpan 10
	End
End

Condition
	On	Time
	Time	40
	Repeat	1

	Event
		Type	Local
		At	Root
		ChildFx	:Child_ElectricalSurge01.fx
		LifeSpan 10
	End
End

Condition
	On	Time
	Time	50
	Repeat	1

	Event
		Type	Local
		At	Root
		ChildFx	:Child_ElectricalSurge01.fx
		LifeSpan 10
	End
End

#############################################################

End