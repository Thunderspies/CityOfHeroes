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
		Part2	:HitRing.part	
		Bhvr	behaviors/GenericParticleFade.bhvr
	End

End

Condition
	On	Time
	Time	13

	Event	
		Ename	LightRays
		Type	Posit
		At	chest
		Part2	:HitRing2.part	
		Bhvr	behaviors/GenericParticleFade.bhvr
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
		part1	:RadiationLightHit.part
		Bhvr	behaviors/GenericParticleFade.bhvr
		LifeSpan	25
	End

	Event
		
		Type	Posit
		At	chest
		part1	:RadiationElectronsHit.part
		part2	:RadiationBubbleBlastDotsHit.part
		Part3	:RadiationBubblesHit.part
		Part4	:RadiationBubblesSlow.part
		Bhvr	behaviors/GenericParticleFade.bhvr
		PMagnet	chest
		
	End

	Event
		Type	Posit
		At	chest
		#part2	:RadiationAuraGreenHit.part
		part3	:RadiationAuraYellowHit2.part
		Bhvr	behaviors/GenericParticleFade.bhvr
		
		
	End


End


End				