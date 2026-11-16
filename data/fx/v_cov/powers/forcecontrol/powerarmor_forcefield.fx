#########################################################
## Force Field - Personal Force Flied
########################################################

FxInfo

## SOUND FX ######################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Root
		Sound CompBeep6_loop 120 120 .46
	End

	Event
		Type	Start
		At	Root
		Sound M1.ogg 120 120 .46
		Lifespan 10
	End
End

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
		bhvr	behaviors\soundFade3.bhvr
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
		Part1	POWERS\ForceField\HandGlows.part
	End

	Event
		EName 	Prime
		Type	Posit
		At	WepL
		Part1	POWERS\ForceField\HandGlows.part
	End
End

Condition
	On 	Time
	Time 	55

	Event
		EName 	Prime3
		Type	Posit
		At	WepR
		Geom	FX_ForceBubble
		Bhvr 	behaviors\ForceBubbleHands.bhvr
		LifeSpan 9
	End

	Event
		EName 	Prime4
		Type	Posit
		At	WepL
		Geom	FX_ForceBubble
		Bhvr 	behaviors\ForceBubbleHands.bhvr
		LifeSpan 9
	End
End

## FORCE FIELD ######################################################

Condition
	On 	Time
	Time 	5

	Event
		EName 	Prime
		Type	Posit
		At	HIPS
		Bhvr 	behaviors\powers\forcefield\ForceBubble2.bhvr
		Part1	powers\forcefield\bubblesphere.part
		POther	Next
		PMagnet	Next
	End

	Event
		EName 	Prime2
		Type	Posit
		At	HIPS
		Bhvr 	behaviors\powers\forcefield\ForceBubble.bhvr
		Geom	FX_ForceBubble
	End
End

########################################################

End