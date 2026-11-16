#############################################################
## ClayStoneArmor.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Pivot
		Type	local
		At	root
		Bhvr	CustomizeablePowers\StoneArmor\ScaleClay.bhvr
		Part	CustomizeablePowers\StoneArmor\ClayArmorRings.part
		Part	CustomizeablePowers\StoneArmor\HotFeetBaseFlat.part
		Part	CustomizeablePowers\StoneArmor\HotFeetBaseFlat2.part
		Geom	HotFeet
	End
End

#############################################################

Condition
	On	Time
	Time 	0

	Event
		Type	Local
		At 	origin
		Sound Stone4 100 100 0.9
	End
End

Condition
	On	Time
	Time 	18

	Event
		Type	Local
		At 	origin
		Bhvr	Behaviors\soundFade3.bhvr
		Sound mudpots_loop 100 100 0.66
		Flags	PowerLoopingSound
		Lifespan 400
	End
End

#############################################################

Condition
	On 	cycle
	Time 	32
	Chance	0.25

	Event
		Type	start
		At	Pivot
		altpiv	2
		ChildFX	CustomizeablePowers\StoneArmor\Child_MudPots.fx
		Lifespan 19
	End
End

Condition
	On 	cycle
	Time 	37
	Chance	0.25

	Event
		Type	start
		At	Pivot
		altpiv	3
		ChildFX	CustomizeablePowers\StoneArmor\Child_MudPots.fx
		Lifespan 19
	End
End

Condition
	On 	cycle
	Time 	35
	Chance	0.25

	Event
		Type	start
		At	Pivot
		altpiv	4
		ChildFX	CustomizeablePowers\StoneArmor\Child_MudPots.fx
		Lifespan 19
	End
End

Condition
	On 	cycle
	Time 	36
	Chance	0.25

	Event
		Type	start
		At	Pivot
		altpiv	5
		ChildFX	CustomizeablePowers\StoneArmor\Child_MudPots.fx
		Lifespan 19
	End
End

Condition
	On 	cycle
	Time 	43
	Chance	0.25

	Event
		Type	start
		At	Pivot
		altpiv	6
		ChildFX	CustomizeablePowers\StoneArmor\Child_MudPots.fx
		Lifespan 19
	End
End

Condition
	On 	cycle
	Time 	37
	Chance	0.25

	Event
		Type	start
		At	Pivot
		altpiv	7
		ChildFX	CustomizeablePowers\StoneArmor\Child_MudPots.fx
		Lifespan 19
	End
End

Condition
	On 	cycle
	Time 	40
	Chance	0.25

	Event
		Type	start
		At	Pivot
		altpiv	8
		ChildFX	CustomizeablePowers\StoneArmor\Child_MudPots.fx
		Lifespan 19
	End
End

Condition
	On 	cycle
	Time 	34
	Chance	0.25

	Event
		Type	start
		At	Pivot
		altpiv	9
		ChildFX	CustomizeablePowers\StoneArmor\Child_MudPots.fx
		Lifespan 19
	End
End

Condition
	On 	cycle
	Time 	38
	Chance	0.25

	Event
		Type	start
		At	Pivot
		altpiv	10
		ChildFX	CustomizeablePowers\StoneArmor\Child_MudPots.fx
		Lifespan 19
	End
End


Condition
	On 	cycle
	Time 	40
	Chance	0.25

	Event
		Type	start
		At	Pivot
		altpiv	11
		ChildFX	CustomizeablePowers\StoneArmor\Child_MudPots.fx
		Lifespan 19
	End
End

Condition
	On 	cycle
	Time 	41
	Chance	0.25

	Event
		Type	start
		At	Pivot
		altpiv	12
		ChildFX	CustomizeablePowers\StoneArmor\Child_MudPots.fx
		Lifespan 19
	End
End

Condition
	On 	cycle
	Time 	36
	Chance	0.25

	Event
		Type	start
		At	Pivot
		altpiv	13
		ChildFX	CustomizeablePowers\StoneArmor\Child_MudPots.fx
		Lifespan 19
	End
End

Condition
	On 	cycle
	Time 	44
	Chance	0.25

	Event
		Type	start
		At	Pivot
		altpiv	14
		ChildFX	CustomizeablePowers\StoneArmor\Child_MudPots.fx
		Lifespan 19
	End
End

#############################################################

End