#########################################################
## Fire Contrcol - Hot Feet
#########################################################

FxInfo

## HANDS #######################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	WepR
		Part1	CustomizeablePowers\FireManipulation\FireHandz.part
		Sound Firehand_loop 100.0 100.0 .45
		LifeSpan 60
	End

	Event
		Type	Local
		At	WepL
		Part1	CustomizeablePowers\FireManipulation\FireHandz.part
		Sound Ignite2 100.0 100.0 .8
		LifeSpan 60
	End
End

## GROUND FX #######################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		Part1	CustomizeablePowers\FireManipulation\HotfeetRing.part
		LifeSpan 10
	End
End

Condition
	On 	cycle
	Time 	30

	Event
		Type	Local
		At	Root
		Part1	CustomizeablePowers\FireManipulation\HotfeetRing.part
		LifeSpan 30
	End
End

Condition
	On 	Time
	Time 	25

	Event
		EName	splat
		Type	Local
		At	root
		Part	CustomizeablePowers\FireManipulation\HotFeetBaseFlat.part
		Part	CustomizeablePowers\FireManipulation\HotFeetBaseFlat2.part
	End
End

#############################################################

Condition
	On	Time
	Time	30

	Event
		EName	Pivot
		Type	local
		At	root
		Bhvr	Behaviors\soundFade2.bhvr
		Geom	HotFeet
		Sound flameburst_loop 60 60 .39
		Flags	PowerLoopingSound
		Lifespan 480
	End

	Event
		Ename	fireSound
		Type	Local
		At	root
		Sound fireballfly 60 60 .7
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	FlamesOffset
		Type	Local
		At	Root
		Geom	HotFeet01
	End
End

Condition
	On 	cycle
	Time 	20
	Chance	0.4

	Event
		Type	start
		At	FlamesOffset
		altpiv	1
		ChildFx	:Child_HotFeet_Flames1.fx
		LifeSpan 8
	End
End

Condition
	On 	cycle
	Time 	22
	Chance	0.4

	Event
		Type	start
		At	FlamesOffset
		altpiv	2
		ChildFx	:Child_HotFeet_Flames1.fx
		LifeSpan 9
	End
End

Condition
	On 	cycle
	Time 	27
	Chance	0.4
	Event
		Type	start
		At	FlamesOffset
		altpiv	3
		ChildFx	:Child_HotFeet_Flames1.fx
		LifeSpan 8
	End
End

Condition
	On 	cycle
	Time 	25
	Chance	0.4

	Event
		Type	start
		At	FlamesOffset
		altpiv	4
		ChildFx	:Child_HotFeet_Flames1.fx
		LifeSpan 9
	End
End

Condition
	On 	cycle
	Time 	26
	Chance	0.4
	Event
		Type	start
		At	FlamesOffset
		altpiv	5
		ChildFx	:Child_HotFeet_Flames1.fx
		LifeSpan 10
	End
End

Condition
	On 	cycle
	Time 	33
	Chance	0.4

	Event
		EName	Fire6
		Type	start
		At	FlamesOffset
		altpiv	6
		ChildFx	:Child_HotFeet_Flames1.fx
		LifeSpan 8
	End
End

Condition
	On 	cycle
	Time 	27
	Chance	.4

	Event
		EName	Fire7
		Type	start
		At	FlamesOffset
		altpiv	7
		ChildFx	:Child_HotFeet_Flames1.fx
		LifeSpan 9
	End
End

Condition
	On 	cycle
	Time 	30
	Chance	0.4

	Event
		EName	Fire8
		Type	start
		At	FlamesOffset
		altpiv	8
		ChildFx	:Child_HotFeet_Flames1.fx
		LifeSpan 10
	End
End

Condition
	On 	cycle
	Time 	24
	Chance	.4

	Event
		EName	Fire9
		Type	start
		At	FlamesOffset
		altpiv	9
		ChildFx	:Child_HotFeet_Flames1.fx
		LifeSpan 8
	End
End

Condition
	On 	cycle
	Time 	28
	Chance	0.4

	Event
		EName	Fire10
		Type	start
		At	FlamesOffset
		altpiv	10
		ChildFx	:Child_HotFeet_Flames1.fx
		LifeSpan 10
	End
End

Condition
	On 	cycle
	Time 	30
	Chance	0.4

	Event
		EName	Fire11
		Type	start
		At	FlamesOffset
		altpiv	11
		ChildFx	:Child_HotFeet_Flames1.fx
		LifeSpan 9
	End
End

Condition
	On 	cycle
	Time 	31
	Chance	0.4

	Event
		EName	Fire11
		Type	start
		At	FlamesOffset
		altpiv	12
		ChildFx	:Child_HotFeet_Flames1.fx
		LifeSpan 8
	End
End

Condition
	On 	cycle
	Time 	26
	Chance	0.4

	Event
		EName	Fire11
		Type	start
		At	FlamesOffset
		altpiv	13
		ChildFx	:Child_HotFeet_Flames1.fx
		LifeSpan 10
	End
End

Condition
	On 	cycle
	Time 	34
	Chance	0.4

	Event
		EName	Fire11
		Type	start
		At	FlamesOffset
		altpiv	14
		ChildFx	:Child_HotFeet_Flames1.fx
		LifeSpan 9
	End
End

########################################################################################################

End