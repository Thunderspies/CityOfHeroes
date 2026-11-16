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
		Sound Fast2 70 70 .7
		Lifespan 80
	End
End



#########################################################
#################### BODY GLOW 1 ########################
#########################################################

Condition
	On	Time
	Time	30

	Event	
		ENAME	hookup
		Type	local
		At	Root
		Bhvr	:PsionicPowerHandOffset2.bhvr
			
	End

	Event	
		ENAME	HandGlow
		Type	start
		At	hookup
		part	:HandGlow.part
		part	:HandGlowdown.part
		#Bhvr	:HandRotate.bhvr
		Lifespan 20
	
	End

	Event	
		ENAME	HandGlow
		Type	start
		At	hookup
		Part	:HandStreakDown.part
		Part	:HandStreak.part
		#Bhvr	:HandRotate.bhvr
		Lifespan 20
	
	End

End

Condition
	On	Time
	Time	5
	

	Event	
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	:HealEnergy.part
		part	:Heal.part
		#POther Neck
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 80
	
	End

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Head
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
		part	:HealShouldersEnergy.part
		part	:HealShoulders.part
		#POther LarmL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 80
		
	End

End


End		