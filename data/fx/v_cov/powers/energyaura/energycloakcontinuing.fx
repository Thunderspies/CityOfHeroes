#########################################################
##	template

FxInfo


#########################################################
####################### AUDIO ###########################
#########################################################



Condition
	On	Time
	Time	0
	

	Event
		Type	Local
		At 	Origin
		Sound Glow3_loop 80 80 .26
	End
End


#########################################################
#################### BODY GLOW 1 ########################
#########################################################

Condition
	On	Time
	Time	30
	

	Event	
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	:EnergyCloakStealthBodyGlow01.part
		part	:EnergyCloakStealthBodyGlow02.part
		part	:EnergyCloakStealthBodyGlowBlue01.part
		part	:EnergyCloakStealthBodyGlowBlue02.part
		POther Neck
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
	
	End

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part	:EnergyCloakStealthBodyGlow01.part
		part	:EnergyCloakStealthBodyGlow02.part
		part	:EnergyCloakStealthBodyGlowBlue01.part
		part	:EnergyCloakStealthBodyGlowBlue02.part
		POther Chest
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End
	

	Event	
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part	:EnergyCloakStealthBodyGlow01.part
		part	:EnergyCloakStealthBodyGlow02.part
		part	:EnergyCloakStealthBodyGlowBlue01.part
		part	:EnergyCloakStealthBodyGlowBlue02.part
		POther LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:EnergyCloakStealthBodyGlow01.part
		part	:EnergyCloakStealthBodyGlow02.part
		part	:EnergyCloakStealthBodyGlowBlue01.part
		part	:EnergyCloakStealthBodyGlowBlue02.part
		POther LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End


	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part	:EnergyCloakStealthBodyGlow01.part
		part	:EnergyCloakStealthBodyGlow02.part
		part	:EnergyCloakStealthBodyGlowBlue01.part
		part	:EnergyCloakStealthBodyGlowBlue02.part
		POther LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End
	
	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:EnergyCloakStealthBodyGlow01.part
		part	:EnergyCloakStealthBodyGlow02.part
		part	:EnergyCloakStealthBodyGlowBlue01.part
		part	:EnergyCloakStealthBodyGlowBlue02.part
		POther LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End


	Event	
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part	:EnergyCloakStealthBodyGlow01.part
		part	:EnergyCloakStealthBodyGlow02.part
		part	:EnergyCloakStealthBodyGlowBlue01.part
		part	:EnergyCloakStealthBodyGlowBlue02.part
		POther Ulegl
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part	:EnergyCloakStealthBodyGlow01.part
		part	:EnergyCloakStealthBodyGlow02.part
		part	:EnergyCloakStealthBodyGlowBlue01.part
		part	:EnergyCloakStealthBodyGlowBlue02.part
		POther Llegl
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part	:EnergyCloakStealthBodyGlow01.part
		part	:EnergyCloakStealthBodyGlow02.part
		part	:EnergyCloakStealthBodyGlowBlue01.part
		part	:EnergyCloakStealthBodyGlowBlue02.part
		POther UlegR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part	:EnergyCloakStealthBodyGlow01.part
		part	:EnergyCloakStealthBodyGlow02.part
		part	:EnergyCloakStealthBodyGlowBlue01.part
		part	:EnergyCloakStealthBodyGlowBlue02.part
		
		POther LlegR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End


End


End		