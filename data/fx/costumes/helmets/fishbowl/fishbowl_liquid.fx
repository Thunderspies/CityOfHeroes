#############################################################
## HEADER
#############################################################

FxInfo

Flags InheritAlpha InheritGeoScale

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	BubbleOffset
		Type	Local
		At	Chest
		BhvrOverride
			TintGeom	1
		End
		GEOM	FX_GEO_Fishbowl_OFFSETS
	End

	Event
		Type	Local
		At	Chest
		BhvrOverride
			TintGeom	1
		End
		GEOM	GEO_Fishbowl_Liquid
	End
End

Condition
	On	Cycle
	Time	10
	Random	1

	Event
		EName	Bubbles01
		Type	Local
		At	BubbleOffset
		AltPiv	2
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 -0.05 0.0
			StartJitter	0.0 0.2 0.0
			FadeOutLength	15
		End
		Part	:FishBowl_Bubbles.part
		Lifespan 	30
		LifespanJitter	10
	End

	Event
		EName	Bubbles02
		Type	Local
		At	BubbleOffset
		AltPiv	3
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 -0.05 0.0
			StartJitter	0.0 0.2 0.0
			FadeOutLength	15
		End
		Part	:FishBowl_Bubbles.part
		Lifespan 	30
		LifespanJitter	10
	End

	Event
		EName	Bubbles03
		Type	Local
		At	BubbleOffset
		AltPiv	4
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 -0.05 0.0
			StartJitter	0.0 0.2 0.0
			FadeOutLength	15
		End
		Part	:FishBowl_Bubbles.part
		Lifespan 	30
		LifespanJitter	10
	End

	Event
		EName	Bubbles04
		Type	Local
		At	BubbleOffset
		AltPiv	5
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 -0.05 0.0
			StartJitter	0.0 0.2 0.0
			FadeOutLength	15
		End
		Part	:FishBowl_Bubbles.part
		Lifespan 	30
		LifespanJitter	10
	End

	Event
		EName	Bubbles05
		Type	Local
		At	BubbleOffset
		AltPiv	6
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 -0.05 0.0
			StartJitter	0.0 0.2 0.0
			FadeOutLength	15
		End
		Part	:FishBowl_Bubbles.part
		Lifespan 	30
		LifespanJitter	10
	End
End

#############################################################

End