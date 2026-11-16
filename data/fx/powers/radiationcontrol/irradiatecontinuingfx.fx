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
		part1	POWERS\RadiationControl\RadiationLightHit.part
		LifeSpan 25
	End

	Event
		Type	Posit
		At	chest
		Bhvr	behaviors\GenericParticleFade.bhvr
		part1	POWERS\RadiationControl\RadiationElectronsHit.part
		part2	POWERS\RadiationControl\RadiationBubbleBlastDotsHit.part
		Part3	POWERS\RadiationControl\RadiationBubblesHit.part
		Part4	POWERS\RadiationControl\RadiationBubblesSlow.part
		PMagnet	chest
	End

	Event
		Type	Posit
		At	chest
		Bhvr	behaviors\GenericParticleFade.bhvr
		part3	POWERS\RadiationControl\RadiationAuraYellowHit2.part
	End
End

Condition
	On	Time
	Time	7

	Event
		Type	Posit
		At	chest
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part2	POWERS\RadiationControl\HitRing.part
	End
End

Condition
	On	Time
	Time	13

	Event
		Type	Posit
		At	chest
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part2	POWERS\RadiationControl\HitRing2.part
	End
End

#############################################################

End