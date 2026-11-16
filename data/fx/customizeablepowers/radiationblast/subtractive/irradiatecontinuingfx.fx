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
		part1	:RadiationLightHit.part
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
		part1	:RadiationElectronsHit.part
		part2	:RadiationBubbleBlastDotsHit.part
		Part3	:RadiationBubblesHit.part
		Part4	:RadiationBubblesSlow.part
		part3	:RadiationAuraYellowHit2.part
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
		Part2	:HitRing.part
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
		Part2	:HitRing2.part
		Lifespan 1
	End
End

#############################################################

End