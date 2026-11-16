#########################################################
##	template

FxInfo
LifeSpan	40

#########################################################
####################### AUDIO ###########################
#########################################################

Condition
	On	Time
	Time	0
	

	Event
		Type	Local
		At 	Origin
		Sound Fast2 70 70 .7
	End
End



#########################################################
#################### BODY GLOW 1 ########################
#########################################################



Condition
	On	Time
	Time	5
	

	Event	
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	:HideLinesDark.part
		part	:HideEnergyDark.part
		POther Neck
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 30
	
	End

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Head
		part	:HideLinesDark.part
		part	:HideEnergyDark.part
		POther Chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 30
		
	End
	

	Event	
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part	:HideLinesDark.part
		part	:HideEnergyDark.part
		POther LarmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 30
		
	End

	
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:HideLinesDark.part
		part	:HideEnergyDark.part
		POther LarmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 30
		
	End


	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part	:HideLinesDark.part
		part	:HideEnergyDark.part
		POther LarmL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 30
		
	End
	
	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:HideLinesDark.part
		part	:HideEnergyDark.part
		POther LarmL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 30
		
	End


	Event	
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part	:HideLinesDark.part
		part	:HideEnergyDark.part
		POther Ulegl
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 30
		
	End

	Event	
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part	:HideLinesDark.part
		part	:HideEnergyDark.part
		POther Llegl
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 30
		
	End

	Event	
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part	:HideLinesDark.part
		part	:HideEnergyDark.part
		POther UlegR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 30
		
	End

	Event	
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part	:HideLinesDark.part
		part	:HideEnergyDark.part
		POther LlegR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 30
		
	End


End


End		