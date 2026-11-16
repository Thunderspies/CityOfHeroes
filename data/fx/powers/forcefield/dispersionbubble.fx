#########################################################
## Dispersion Bubble
########################################################

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
		Sound ForceField3_loop 100.0 100.0 .42
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 220
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	hand1
		Type	local
		At	WepR
		Part1	:HandGlowsA.part
		Bhvr 	behaviors/GenericParticleFade.bhvr
		Lifespan	1
	End
	
	Event
		EName 	hand2
		Type	local
		At	WepL
		Part1	:HandGlowsA.part
		Bhvr 	behaviors/GenericParticleFade.bhvr
		Lifespan	1
	End	
End

Condition
	On 	Time
	Time 	35

	Event
		EName 	Prime3
		Type	local
		At	WepR
		Geom	FX_ForceBubble
		Bhvr 	behaviors/ForceBubbleHands.bhvr
		LifeSpan	9
	End
	
	Event
		EName 	Prime4
		Type	local
		At	WepL
		Geom	FX_ForceBubble
		Bhvr 	behaviors/ForceBubbleHands.bhvr
		LifeSpan	9
	End
End

## BUBBLE ######################################################

Condition
	On 	Time
	Time 	17

	Event
		EName 	Prime
		Type	Create
		Inherit	Position
		Update	Position
		At	HIPS
		Bhvr 	behaviors/powers/forcefield/DispersionBubbleLarge2.bhvr
		Part1	powers/forcefield/bubblesphereLarge.part
	End

	Event
		EName 	Prime2
		Type	Create
		Inherit	Position
		Update	Position
		At	HIPS
		Bhvr 	behaviors/powers/forcefield/DispersionBubbleLarge.bhvr
		Geom	FX_ForceBubble
		#LifeSpan	50
	End

	Event
		EName 	Ring
		Type	Create
		Inherit	Position
		Update	Position
		At	HIPS
		Part1	:RepulsionBubbleRing.part
	End
End

Condition
	On 	Time
	Time 	27

	Event
		EName 	Ring
		Type	Create
		Inherit	Position
		Update	Position
		At	HIPS
		Part1	:RepulsionBubbleRingBurst.part
	End
End

########################################################

End