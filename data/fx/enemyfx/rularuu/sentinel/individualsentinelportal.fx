
FxInfo

Lifespan	100

##################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Eye
		Type	Local
		At	root
		
		Part	:EyeRingsUp.part
		lifespan	70
	End

End

Condition
	On	time
	Time	45
	Event	
		ENAME	Lightning
		Type	Local
		At	chest
		BHVR	Behaviors/GenericParticleFade.bhvr
		part	:Storm_Lightning_01.part
		Lifespan	240
	End
	
	Event	
		ENAME	Lightning1
		Type	Local
		At	chest
		BHVR	Behaviors/GenericParticleFade.bhvr
		part	:Storm_Lightning_Flash_01.part
		Lifespan	223
	End

	Event	
		ENAME	Lightning
		Type	Local
		At	chest
		part	:Storm_Lightning_02.part
		BHVR	Behaviors/GenericParticleFade.bhvr
		Lifespan	227
	End
	
	Event	
		ENAME	Lightning
		Type	Local
		At	chest
		BHVR	Behaviors/GenericParticleFade.bhvr
		part	:Storm_Lightning_Flash_01.part
		Lifespan	227
	End


	Event	
		ENAME	Lightning
		Type	Local
		At	chest
		part	:Storm_Lightning_03.part
		BHVR	Behaviors/GenericParticleFade.bhvr
		Lifespan	227
		
	End			
End

Condition

	On	Time
	Time	30

	Event	
		ENAME	Cloud
		Type	Local
		At	chest
		part	:Storm_Heart_01a.part
		part	:Storm_Center_01a.part
		part	:Storm_Center_02a.part

		Lifespan	15

	End

End

Condition

	On	Time
	Time	30
	
	Event
		EName	Portal2
		Type	start
		At	root
		BHVR	Behaviors/IndividualSentinelOffset.bhvr
		Part	:PortalWaves1.part
		Part	:PortalRings.part
		Part	:PortalRingsSolid.part

		LifeSpan	160
	End

End

End				