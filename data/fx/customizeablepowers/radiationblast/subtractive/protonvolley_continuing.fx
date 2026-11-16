#########################################################
##	RadiationAura
FxInfo

LifeSpan	40

Input
	InpName	chest
End

########################################################


Condition
	On	Time
	Time	7

	Event
		Ename	LightRays
		Type	Posit
		At	chest
		Part2	CustomizeablePowers\RadiationBlast\HitRing.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
	End

End

Condition
	On	Time
	Time	13

	Event
		Ename	LightRays
		Type	Posit
		At	chest
		Part2	CustomizeablePowers\RadiationBlast\HitRing2.part
		BHVR	Behaviors\GenericParticleFade.bhvr
		#LifeSpan	25
	End

End

Condition
	On	Time
	Time	0

	Event
		Ename	LightRays2
		Type	Posit
		At	chest
		part1	CustomizeablePowers\RadiationBlast\RadiationLightHit.part
		BHVR	Behaviors\GenericParticleFade.bhvr
		LifeSpan	25
	End

	Event

		Type	Posit
		At	chest
		part1	CustomizeablePowers\RadiationBlast\RadiationElectronsHit.part
		part2	CustomizeablePowers\RadiationBlast\RadiationBubbleBlastDotsHit.part
		Part3	CustomizeablePowers\RadiationBlast\RadiationBubblesHit.part
		Part4	CustomizeablePowers\RadiationBlast\RadiationBubblesSlow.part
		BHVR	Behaviors\GenericParticleFade.bhvr
		PMagnet	chest

	End

	Event
		Type	Posit
		At	chest
		#part2	:RadiationAuraGreenHit.part
		part3	CustomizeablePowers\RadiationBlast\RadiationAuraYellowHit2.part
		BHVR	Behaviors\GenericParticleFade.bhvr


	End


End


End