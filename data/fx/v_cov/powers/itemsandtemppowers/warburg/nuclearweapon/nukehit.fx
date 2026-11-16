#########################################################
##	RadiationAura
FxInfo

LifeSpan	45

Input  
	InpName	chest
End

########################################################


Condition
	On	Time
	Time	0

	Event
		ENAME	Glow
		Type	SetLight	
		Bhvr	:NukeHitLight.bhvr

		Lifespan 60
	End

	Event
		Type	Local
		At	Hips
		part	:NukeHitFlash.part
		Bhvr	behaviors/GenericParticleFade.bhvr
		
		LifeSpan	15
			
	End


	Event	
		Ename	LightRays2
		Type	Posit
		At	chest
		part1	POWERS\RadiationControl\RadiationLightHit.part
		part1	POWERS\RadiationControl\RadiationLightHit.part
		Bhvr	behaviors/GenericParticleFade.bhvr
		#LifeSpan	25
	End

	Event
		
		Type	Posit
		At	chest
		part1	POWERS\RadiationControl\RadiationElectronsHit.part
		part2	POWERS\RadiationControl\RadiationBubbleBlastDotsHit.part
		Part3	POWERS\RadiationControl\RadiationBubblesHit.part
		Part4	POWERS\RadiationControl\RadiationBubblesSlow.part
		Bhvr	behaviors/GenericParticleFade.bhvr
		PMagnet	chest
		LifeSpan	15
		
	End

	Event
		Type	Posit
		At	chest
		part2	POWERS\RadiationControl\RadiationAuraGreenHit.part
		part3	POWERS\RadiationControl\RadiationAuraYellowHit2.part
		Bhvr	behaviors/GenericParticleFade.bhvr
		LifeSpan	10
			
	End

	Event	
		Ename	LightRing
		Type	Local
		At	root
		Part2	:NukeHitRing.part	
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0 3.3 0
		End

	End

End


End				