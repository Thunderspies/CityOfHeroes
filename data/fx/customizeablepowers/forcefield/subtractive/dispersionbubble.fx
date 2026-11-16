#############################################################
## DispersionBubble.fx
#############################################################

FxInfo

## HANDS ######################################################

Condition
	On	Time
	Time 	7

	Event
		Type 	Local
		At	Origin
		Sound forcehands3c 80 80 .8
	End
End

Condition
	On	Time
	Time 	17

	Event
		Type 	Local
		At	Origin
		Bhvr	Behaviors\soundFade3.bhvr
		Sound ForceField3_loop 100.0 100.0 .42
		Flags	PowerLoopingSound
		Lifespan 220
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	hand1
		Type	local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	:HandGlowsA.part
		Lifespan 1
	End

	Event
		EName 	hand2
		Type	local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	:HandGlowsA.part
		Lifespan 1
	End
End

Condition
	On 	Time
	Time 	35

	Event
		EName 	Prime3
		Type	local
		At	WepR
		Bhvr	CustomizeablePowers\ForceField\ForceBubbleHands.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	tintable_Dark_FX_ForceBubble
		Lifespan 9
	End

	Event
		EName 	Prime4
		Type	local
		At	WepL
		Bhvr	CustomizeablePowers\ForceField\ForceBubbleHands.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	tintable_Dark_FX_ForceBubble
		Lifespan 9
	End
End

## BUBBLE ######################################################

Condition
	On 	Time
	Time 	17

	Event
		Type	Create
		Inherit	Position
		Update	Position
		At	Hips
		Bhvr	CustomizeablePowers\ForceField\DispersionBubbleLarge2.bhvr
		Part1	:bubblesphereLarge.part
	End

	Event
		Type	Create
		Inherit	Position
		Update	Position
		At	Hips
		Bhvr	CustomizeablePowers\ForceField\DispersionBubbleLarge.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	tintable_Dark_FX_ForceBubble
	End

	Event
		Type	Create
		Inherit	Position
		Update	Position
		At	Hips
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part1	:RepulsionBubbleRing.part
	End
End

Condition
	On 	Time
	Time 	27

	Event
		Type	Local
		At	Root
		Part1	:RepulsionBubbleRingBurst.part
	End
End

########################################################

End