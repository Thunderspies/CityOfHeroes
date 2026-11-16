#########################################################
##	template

FxInfo

Lifespan 80
#########################################################
####################### AUDIO ###########################
#########################################################

Condition
	On	Time
	Time	0
	

	Event
		Type	Local
		At 	Origin
		Sound snake_heal 70 70 .9
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
		At	Fore_LlegL
		Part	:HealGlowies.part
		part	:HealEnergyOut.part
		part	:HealOut.part
		#POther Neck
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 80
	
	End

	Event	
		ENAME	ChestGlow
		Type	Local
		At	Fore_FootL
		Part	:HealGlowies.part
		part	:HealEnergyOut.part
		part	:HealOut.part
		#POther Neck
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 80
	
	End


	Event	
		ENAME	ChestGlow
		Type	Local
		At	Fore_UlegL
		Part	:HealGlowies.part
		part	:HealEnergyOut.part
		part	:HealOut.part
		#POther Neck
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 80
	
	End

#	Event	
#		ENAME	ChestGlow
#		Type	Local
#		At	Hips
#		part	:HealEnergy.part
#		part	:Heal.part
#		#POther Neck
#		Bhvr	Behaviors/GenericParticleFade.bhvr
#		Lifespan 80
#	
#	End

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Head
		Part	:HealGlowies.part
		part	:HealEnergy.part
		part	:Heal.part
		#POther Chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 80
		
	End
	

	Event	
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		Part	:HealGlowies.part
		part	:HealEnergy.part
		part	:Heal.part
		#POther LarmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 80
		
	End

	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		Part	:HealGlowies.part
		part	:HealEnergy.part
		part	:Heal.part
		#POther LarmL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 80
		
	End
		
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		Part	:HealGlowies.part
		part	:HealLowerArmsEnergy.part
		part	:HealLowerArms.part
		#POther LarmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 80
		
	End

	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		Part	:HealGlowies.part
		part	:HealLowerArmsEnergy.part
		part	:HealLowerArms.part
		#POther LarmL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 80
		
	End


	
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	LarmR
		Part	:HealGlowies.part
		part	:HealLowerArmsEnergy.part
		part	:HealLowerArms.part
		#POther LarmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 80
		
	End


	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	LarmL
		Part	:HealGlowies.part
		part	:HealLowerArmsEnergy.part
		part	:HealLowerArms.part
		#POther LarmL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 80
		
	End
	
	
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	UarmR
		Part	:HealGlowies.part
		part	:HealShouldersEnergy.part
		part	:HealShoulders.part
		#POther LarmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 80
		
	End


	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	UarmL
		Part	:HealGlowies.part
		part	:HealShouldersEnergy.part
		part	:HealShoulders.part
		#POther LarmL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 80
		
	End

###########################################################################
##Lower Body
####################################
	
#	Event	
#		ENAME	RightLarmGlow
#		Type	Local
#		At	ULegR
#		part	:HealLowerArmsEnergy.part
#		part	:HealLowerArms.part
#		#POther LarmR
#		Bhvr	Behaviors/GenericParticleFade.bhvr
#		Lifespan 80
#		
#	End
#
#
#	Event	
#		ENAME	LeftUarmGlow
#		Type	Local
#		At	ULegL
#		part	:HealLowerArmsEnergy.part
#		part	:HealLowerArms.part
#		#POther LarmL
#		Bhvr	Behaviors/GenericParticleFade.bhvr
#		Lifespan 80
#		
#	End
#
#	Event	
#		ENAME	RightLarmGlow
#		Type	Local
#		At	LLegR
#		part	:HealLowerArmsEnergy.part
#		part	:HealLowerArms.part
#		#POther LarmR
#		Bhvr	Behaviors/GenericParticleFade.bhvr
#		Lifespan 80
#		
#	End
#
#
#	Event	
#		ENAME	LeftUarmGlow
#		Type	Local
#		At	LLegL
#		part	:HealLowerArmsEnergy.part
#		part	:HealLowerArms.part
#		#POther LarmL
#		Bhvr	Behaviors/GenericParticleFade.bhvr
#		Lifespan 80
#		
#	End
#################################################################
##LowerLeg
####################################3
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	Hind_FootR
		Part	:HealGlowies.part
		part	:HealTail.part
		part	:HealEnergyTail.part
		#POther LarmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 80
		
	End

	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	Fore_FootL
		Part	:HealGlowies.part
		part	:HealTail.part
		part	:HealEnergyTail.part
		#POther LarmL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 80
		
	End

	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	hind_FootL
		Part	:HealGlowies.part
		part	:HealTail.part
		part	:HealEnergyTail.part
		#POther LarmL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 80
		
	End
	
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	Hind_UlegR
		Part	:HealGlowies.part
		part	:HealTail.part
		part	:HealEnergyTail.part
		#POther LarmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 80
		
	End

	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	Hind_UlegL
		Part	:HealGlowies.part
		part	:HealTail.part
		part	:HealEnergyTail.part
		#POther LarmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 80
		
	End

	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	hind_LlegL
		Part	:HealGlowies.part
		part	:HealTail.part
		part	:HealEnergyTail.part
		#POther LarmL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 80
		
	End
	
	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	hind_ToeL
		Part	:HealGlowies.part
		part	:HealTail.part
		part	:HealEnergyTail.part
		#POther LarmL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 80
		
	End

End		