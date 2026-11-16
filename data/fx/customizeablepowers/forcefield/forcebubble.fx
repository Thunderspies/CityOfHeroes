#############################################################
## ForceBubble.fx
#############################################################

FxInfo

## SOUND FX ######################################################

Condition
	On	Time
	Time 	4

	Event
		Type 	Local
		At	Origin
		Sound forcehands5 80 80 .22
	End
End

Condition
	On	Time
	Time 	10

	Event
		Type 	Local
		At	Origin
		Sound ForceField3_loop 100.0 100.0 .42
		Bhvr	Behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 220
	End
End
## HANDS ######################################################

Condition
	On 	Time
	Time 	10

	Event
		EName 	Prime
		Type	Posit
		At	WepR
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\ForceField\HandGlows.part
	End

	Event
		EName 	Prime
		Type	Posit
		At	WepL
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\ForceField\HandGlows.part
	End
End

Condition
	On 	Time
	Time 	55

	Event
		EName 	Prime3
		Type	Posit
		At	WepR
		Bhvr	CustomizeablePowers\ForceField\ForceBubbleHands.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	tintable_FX_ForceBubble
		Lifespan 9
	End

	Event
		EName 	Prime4
		Type	Posit
		At	WepL
		Bhvr	CustomizeablePowers\ForceField\ForceBubbleHands.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	tintable_FX_ForceBubble
		Lifespan 9
	End
End

## FORCE FIELD ######################################################

Condition
	On 	Time
	Time 	5

	Event
		Ename	Prime
		Type	Posit
		At	Hips
		Bhvr	CustomizeablePowers\ForceField\ForceBubble2.bhvr
		Part1	CustomizeablePowers\ForceField\bubblesphere.part
		POther	Next
		PMagnet	Next
	End

	Event
		Ename	Prime2
		Type	Posit
		At	Hips
		Bhvr	CustomizeablePowers\ForceField\ForceBubble.bhvr
		BhvrOverride
			Alpha		75
			TintGeom	1
		End
		Geom	tintable_FX_ForceBubble
	End
End

########################################################

End