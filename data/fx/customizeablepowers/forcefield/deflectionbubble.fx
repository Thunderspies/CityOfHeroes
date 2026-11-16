#############################################################
## DeflectionBubble.fx
#############################################################

FxInfo

## SOUND FX ######################################################

Condition
	On	Time
	Time 	7

	Event
		Type 	Local
		At	Origin
		Sound forcehands3 80 80 .4
	End
End

Condition
	On	Time
	Time 	18

	Event
		Type 	Local
		At	Origin
		Sound ForceField3_loop 100.0 100.0 .42
		Bhvr	Behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 220
	End
End

Condition
	On	Time
	Time 	18

	Event
		Type 	Local
		At	Origin
		Sound ForceField6_loop 100.0 100.0 .26
		Bhvr	Behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 220
	End
End

## HANDS ######################################################

Condition
	On 	Time
	Time 	20

	Event
		EName 	Prime
		Type	Posit
		At	WepR
		Part1	CustomizeablePowers\ForceField\HandGlowsc.part
		Lifespan 5
	End

	Event
		EName 	Prime
		Type	Posit
		At	WepL
		Part1	CustomizeablePowers\ForceField\HandGlowsc.part
		Lifespan 5
	End
End

## FORCE FIELD ######################################################

Condition
	On 	Time
	Time 	18

	Event
		Type	Local
		At	Root
		Bhvr	CustomizeablePowers\ForceField\ForceBubbleLarge.bhvr
		BhvrOverride
			TintGeom	1
		End
		Part1	CustomizeablePowers\ForceField\bubblesphereLarge.part
		Geom	Tintable_FX_ForceBubble
	End

	Event
		Type	PositOnly
		At	HIPS
		Part1	CustomizeablePowers\ForceField\RepulsionBubbleRing2.part
	End
End

Condition
	On 	Time
	Time 	37

	Event
		Type	Local
		At	Root
		Part1	CustomizeablePowers\ForceField\RepulsionBubbleRingBurst2.part
	End
End

########################################################

End