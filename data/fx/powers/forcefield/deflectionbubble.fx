#########################################################
## Force Field - Force Bubble
########################################################

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
		bhvr	behaviors/soundFade3.bhvr
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
		bhvr	behaviors/soundFade3.bhvr
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
		Part1	:HandGlowsc.part
		LifeSpan	5		
	End
	
	Event
		EName 	Prime
		Type	Posit
		At	WepL
		Part1	:HandGlowsc.part
		LifeSpan	5
	End	
End

## FORCE FIELD ######################################################

Condition
	On 	Time
	Time 	18

	Event
		EName 	Prime
		Type	PositOnly
		At	HIPS
		Bhvr 	behaviors/powers/forcefield/ForceBubbleLarge2.bhvr
		Part1	powers/forcefield/bubblesphereLarge.part
		#LifeSpan	30
	End

	Event
		EName 	Prime2
		Type	PositOnly 
		At	HIPS
		Bhvr 	behaviors/powers/forcefield/ForceBubbleLarge.bhvr
		Geom	FX_ForceBubble
		#LifeSpan	50
	End

	Event
		EName 	Ring
		Type	PositOnly 
		At	Root
		Part1	:RepulsionBubbleRing2.part
	End
End

Condition
	On 	Time
	Time 	37
	Event
		EName 	Ring
		Type	PositOnly 
		At	Root
		Part1	:RepulsionBubbleRingBurst2.part
	End
End

########################################################

End