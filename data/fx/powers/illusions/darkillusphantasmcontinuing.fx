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
		Sound Glow6_loop 80 80 .45
	End
End


#########################################################
#################### BODY GLOW 1 ########################
#########################################################



Condition
	On	Time
	Time	0
	

	Event	
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	:DarkIllusPhantBodyGlow01.part
		part	:DarkIllusPhantBodyRays01.part
		part	:DarkIllusPhantRisingGlows01.part
		Part	:LightIllusPhantRisingGlows01.part
		POther Neck
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
	
	End

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part	:DarkIllusPhantBodyGlow01.part
		part	:DarkIllusPhantBodyRays01.part
		part	:DarkIllusPhantRisingGlows01.part
		Part	:LightIllusPhantRisingGlows01.part
		POther Chest
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End
	

	Event	
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part	:DarkIllusPhantBodyGlow01.part
		#part	:DarkIllusPhantBodyRays01.part
		part	:DarkIllusPhantRisingGlows01.part
		Part	:LightIllusPhantRisingGlows01.part
		POther LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:DarkIllusPhantBodyGlow01.part
		part	:DarkIllusPhantBodyRays01.part
		part	:DarkIllusPhantRisingGlows01.part
		Part	:LightIllusPhantRisingGlows01.part
		POther LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End


	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part	:DarkIllusPhantBodyGlow01.part
		#part	:DarkIllusPhantBodyRays01.part
		part	:DarkIllusPhantRisingGlows01.part
		Part	:LightIllusPhantRisingGlows01.part
		POther LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End
	
	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:DarkIllusPhantBodyGlow01.part
		part	:DarkIllusPhantBodyRays01.part
		part	:DarkIllusPhantRisingGlows01.part
		Part	:LightIllusPhantRisingGlows01.part
		POther LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End


	Event	
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part	:DarkIllusPhantBodyGlow01.part
		part	:DarkIllusPhantBodyRays01.part
		part	:DarkIllusPhantRisingGlows01.part
		Part	:LightIllusPhantRisingGlows01.part
		POther Ulegl
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part	:DarkIllusPhantBodyGlow01.part
		part	:DarkIllusPhantBodyRays01.part
		part	:DarkIllusPhantRisingGlows01.part
		Part	:LightIllusPhantRisingGlows01.part
		POther Llegl
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part	:DarkIllusPhantBodyGlow01.part
		part	:DarkIllusPhantBodyRays01.part
		part	:DarkIllusPhantRisingGlows01.part
		Part	:LightIllusPhantRisingGlows01.part
		POther UlegR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part	:DarkIllusPhantBodyGlow01.part
		part	:DarkIllusPhantBodyRays01.part
		part	:DarkIllusPhantRisingGlows01.part
		Part	:LightIllusPhantRisingGlows01.part
		POther LlegR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	HandLGlow
		Type	Local
		At	WepL
		part	:DarkIllusPhantBodyRays01.part
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	HandRGlow
		Type	Local
		At	WepR
		part	:DarkIllusPhantBodyRays01.part
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	FootRGlow
		Type	Local
		At	FootR
		part	:DarkIllusPhantBodyRays01.part
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	FootLGlow
		Type	Local
		At	FootL
		part	:DarkIllusPhantBodyRays01.part
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Hair
		#part	:DarkIllusPhantBodyRays01.part
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	UarmRGlow
		Type	Local
		At	UarmR
		part	:DarkIllusPhantBodyRays01.part
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	UarmLGlow
		Type	Local
		At	UarmL
		part	:DarkIllusPhantBodyRays01.part
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

End


End		