#########################################################
##	template

FxInfo
#LifeSpan	40

#########################################################
####################### AUDIO ###########################
#########################################################

Condition
	On	Time
	Time	0
	

	Event
		Type	Local
		At 	Origin
		Sound Hide2 70 70 .7
	End
End

Condition
	On	Time
	Time	0
	

	Event
		Type	Local
		At 	Origin
		Sound Hide2_loop 70 70 .5
		Bhvr	Behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 200
	End
End



#########################################################
#################### BODY GLOW 1 ########################
#########################################################



Condition
	On	Time
	Time	5
	
	Event	
		ENAME	HeadGlow
		Type	Local
		At	Head
		part	CustomizeablePowers\Regeneration\HideLinesDark.part
		part	CustomizeablePowers\Regeneration\HideEnergyDark.part
		POther Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 30
		
	End
	

	Event	
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part	CustomizeablePowers\Regeneration\HideLinesDark.part
		part	CustomizeablePowers\Regeneration\HideEnergyDark.part
		POther LarmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 30
		
	End

	
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	CustomizeablePowers\Regeneration\HideLinesDark.part
		part	CustomizeablePowers\Regeneration\HideEnergyDark.part
		POther LarmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 30
		
	End


	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part	CustomizeablePowers\Regeneration\HideLinesDark.part
		part	CustomizeablePowers\Regeneration\HideEnergyDark.part
		POther LarmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 30
		
	End
	
	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	CustomizeablePowers\Regeneration\HideLinesDark.part
		part	CustomizeablePowers\Regeneration\HideEnergyDark.part
		POther LarmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 30
		
	End


	Event	
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part	CustomizeablePowers\Regeneration\HideLinesDark.part
		part	CustomizeablePowers\Regeneration\HideEnergyDark.part
		POther Ulegl
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 30
		
	End

	Event	
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part	CustomizeablePowers\Regeneration\HideLinesDark.part
		part	CustomizeablePowers\Regeneration\HideEnergyDark.part
		POther Llegl
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 30
		
	End

	Event	
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part	CustomizeablePowers\Regeneration\HideLinesDark.part
		part	CustomizeablePowers\Regeneration\HideEnergyDark.part
		POther UlegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 30
		
	End

	Event	
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part	CustomizeablePowers\Regeneration\HideLinesDark.part
		part	CustomizeablePowers\Regeneration\HideEnergyDark.part
		POther LlegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 30
		
	End


End

############################################################################################################


Condition
	On	Cycle
	Time	30
	#Chance	.7

	Event	
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	CustomizeablePowers\Regeneration\HideLinesDark2.part
		part	CustomizeablePowers\Regeneration\HideEnergyDark2.part
		POther Neck
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 30
	
	End

#	Event	
#		ENAME	HeadGlow
#		Type	Local
#		At	Head
#		part	:HideLinesDark2.part
#		part	:HideEnergyDark2.part
#		POther Chest
#		Bhvr	Behaviors/GenericParticleFade.bhvr
#		Lifespan 30
#		
#	End
	

	Event	
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part	CustomizeablePowers\Regeneration\HideLinesDark2.part
		part	CustomizeablePowers\Regeneration\HideEnergyDark2.part
		POther LarmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 30
		
	End

	
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	CustomizeablePowers\Regeneration\HideLinesDark2.part
		part	CustomizeablePowers\Regeneration\HideEnergyDark2.part
		POther LarmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 30
		
	End


	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part	CustomizeablePowers\Regeneration\HideLinesDark2.part
		part	CustomizeablePowers\Regeneration\HideEnergyDark2.part
		POther LarmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 30
		
	End
	
	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	CustomizeablePowers\Regeneration\HideLinesDark2.part
		part	CustomizeablePowers\Regeneration\HideEnergyDark2.part
		POther LarmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 30
		
	End


	Event	
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part	CustomizeablePowers\Regeneration\HideLinesDark2.part
		part	CustomizeablePowers\Regeneration\HideEnergyDark2.part
		POther Ulegl
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 30
		
	End

	Event	
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part	CustomizeablePowers\Regeneration\HideLinesDark2.part
		part	CustomizeablePowers\Regeneration\HideEnergyDark2.part
		POther Llegl
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 30
		
	End

	Event	
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part	CustomizeablePowers\Regeneration\HideLinesDark2.part
		part	CustomizeablePowers\Regeneration\HideEnergyDark2.part
		POther UlegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 30
		
	End

	Event	
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part	CustomizeablePowers\Regeneration\HideLinesDark2.part
		part	CustomizeablePowers\Regeneration\HideEnergyDark2.part
		POther LlegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 30
		
	End


End

End		