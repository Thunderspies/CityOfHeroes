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
		BHVR	Powers\EarthControl\QuickSandMudSplatBase.bhvr
		BhvrOverride
			StartColor	255 255 255
			PrimaryTint	90
			SecondaryTint	0
		End
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
		BHVR	Powers\EarthControl\QuickSandMudSplat.bhvr
		BhvrOverride
			StartColor	255 255 255
			PrimaryTint	40
			SecondaryTint	50
		End
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
		BHVR	Powers\EarthControl\QuickSandSplatSmall.bhvr
		BhvrOverride
			StartColor	255 255 255
			PrimaryTint	0
			SecondaryTint	90
		End
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
		BHVR	Powers\EarthControl\QuickSandSplatRing.bhvr
		BhvrOverride
			StartColor	255 255 255
			PrimaryTint	0
			SecondaryTint	90
		End
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
		BHVR	Powers\EarthControl\QuickSandSplat.bhvr
		BhvrOverride
			StartColor	255 255 255
			PrimaryTint	0
			SecondaryTint	90
		End
		Splat	Waves02.tga
		LifeSpan 20
	End
End

#########################################################

End