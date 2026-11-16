#############################################################
## Peacebringer - Essence Boost
#############################################################

FxInfo

Lifespan 3600

## HEAL ###########################################################

Condition
	On	Time
	Time	1

	Event
		Type	Local
		At	WepR
		bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:EssenceBoost_HandsGlow1.part
		Sound regen8c 60 60 .6
		LifeSpan	47
	End

	Event
		Type	Local
		At	WepL
		bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:EssenceBoost_HandsGlow1.part
		LifeSpan	47
	End

	Event
		Type	Local
		At	Chest
		bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:EssenceBoost_HandsGlow1.part
		LifeSpan	47
	End
End

## CONDITIONAL ###########################################################

Condition
	On	Time
	Time	1
	
	Event
		Type	Local
		At	Chest
		Until	SHAPESHIFT
		bhvr	behaviors/soundFade3.bhvr
		Sound glow5_loop 60 60 .1
		Lifespan	220
	End
End

Condition
	On	Time
	Time	20
	
	Event
		EName	EssenceBoost_ContinuingFX
		Type	Local
		At	Origin
		Until	SHAPESHIFT
		ChildFX	:Dullpain_Conditional.fx
		Lifespan	3600
	End
End


#############################################################

End				