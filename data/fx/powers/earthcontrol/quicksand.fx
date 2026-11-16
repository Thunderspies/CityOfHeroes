#########################################################
## Earth Control - Quicksand
#########################################################

FxInfo

## SOUND FX #######################################################

Condition
	On	Time
	Time	0

	Event
		Type	start
		At	root
		Sound arcoflame 80 80 0.55
	End

	Event
		Type	start
		At	root
		Sound mudpots_loop 80 80 0.45
	End
End

## QUICKSAND #######################################################

Condition
	On	Time
	Time	1

	Event
		EName	QuicksandBase1
		Type	start
		At	root
		BHVR	:QuickSandMudSplatBase.bhvr
		Splat	OilSplat.tga
	End
End

Condition
	On	Time
	Time	1
	Repeat	5

	Event
		EName	Quicksand1
		Type	start
		At	root
		BHVR	:QuickSandMudSplat.bhvr
		Splat	QuickSandDiffuse.tga
	End
End

## RIPPLES #######################################################

Condition
	On	cycle
	Time	10

	Event
		EName	Ripple1
		Type	start
		At	Root
		BHVR	:QuickSandSplatSmall.bhvr
		Splat	Generic_Ring.tga
		LifeSpan 10
	End
End

Condition
	On	cycle
	Time	30

	Event
		EName	Ripple2
		Type	start
		At	Root
		BHVR	:QuickSandSplatRing.bhvr
		Splat	Generic_Ring.tga
		LifeSpan 20
	End
End

Condition
	On	cycle
	Time	30
	Chance	0.75

	Event
		EName	Ripple3
		Type	start
		At	Root
		BHVR	:QuickSandSplat.bhvr
		Splat	Waves02.tga
		LifeSpan 20
	End
End

#########################################################

End