#############################################################
## IrradiateContinuingFx.fx
#############################################################

FxInfo

Input
	Inpname	Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Posit
		At	chest
		Bhvr	behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\RadiationBlast\RadiationLightHit.part
		LifeSpan 25
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Posit
		At	chest
		Bhvr	behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\RadiationBlast\RadiationElectronsHit.part
		part2	CustomizeablePowers\RadiationBlast\RadiationBubbleBlastDotsHit.part
		Part3	CustomizeablePowers\RadiationBlast\RadiationBubblesHit.part
		Part4	CustomizeablePowers\RadiationBlast\RadiationBubblesSlow.part
		part3	CustomizeablePowers\RadiationBlast\RadiationAuraYellowHit2.part
		PMagnet	chest
	End
End

#############################################################

Condition
	On	Time
	Time	7

	Event
		Type	Posit
		At	chest
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part2	CustomizeablePowers\RadiationBlast\HitRing.part
		Lifespan 1
	End
End

Condition
	On	Time
	Time	13

	Event
		Type	Posit
		At	chest
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part2	CustomizeablePowers\RadiationBlast\HitRing2.part
		Lifespan 1
	End
End

#############################################################

End