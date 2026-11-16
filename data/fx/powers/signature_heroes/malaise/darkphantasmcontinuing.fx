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
		part	:DarkPhantBodyGlow01.part
		part	:DarkPhantBodyRays01.part
		part	:DarkPhantRisingGlows01.part
		POther Neck
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
	
	End

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part	:DarkPhantBodyGlow01.part
		part	:DarkPhantBodyRays01.part
		part	:DarkPhantRisingGlows01.part
		POther Chest
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End
	

	Event	
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part	:DarkPhantBodyGlow01.part
		#part	:DarkPhantBodyRays01.part
		part	:DarkPhantRisingGlows01.part
		POther LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:DarkPhantBodyGlow01.part
		part	:DarkPhantBodyRays01.part
		part	:DarkPhantRisingGlows01.part
		POther LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End


	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part	:DarkPhantBodyGlow01.part
		#part	:DarkPhantBodyRays01.part
		part	:DarkPhantRisingGlows01.part
		POther LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End
	
	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:DarkPhantBodyGlow01.part
		part	:DarkPhantBodyRays01.part
		part	:DarkPhantRisingGlows01.part
		POther LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End


	Event	
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part	:DarkPhantBodyGlow01.part
		part	:DarkPhantBodyGlow01.part
		part	:DarkPhantBodyGlow01.part
		part	:DarkPhantBodyRays01.part
		part	:DarkPhantRisingGlows01.part
		POther Ulegl
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part	:DarkPhantBodyGlow01.part
		part	:DarkPhantBodyGlow01.part
		part	:DarkPhantBodyRays01.part
		part	:DarkPhantRisingGlows01.part
		POther Llegl
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part	:DarkPhantBodyGlow01.part
		part	:DarkPhantBodyGlow01.part
		part	:DarkPhantBodyGlow01.part
		part	:DarkPhantBodyRays01.part
		part	:DarkPhantRisingGlows01.part
		POther UlegR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part	:DarkPhantBodyGlow01.part
		part	:DarkPhantBodyGlow01.part
		part	:DarkPhantBodyRays01.part
		part	:DarkPhantRisingGlows01.part
		POther LlegR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	HandLGlow
		Type	Local
		At	WepL
		part	:DarkPhantBodyRays01.part
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	HandRGlow
		Type	Local
		At	WepR
		part	:DarkPhantBodyRays01.part
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	FootRGlow
		Type	Local
		At	FootR
		part	:DarkPhantBodyRays01.part
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	FootLGlow
		Type	Local
		At	FootL
		part	:DarkPhantBodyRays01.part
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Hair
		#part	:DarkPhantBodyRays01.part
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	UarmRGlow
		Type	Local
		At	UarmR
		part	:DarkPhantBodyRays01.part
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	UarmLGlow
		Type	Local
		At	UarmL
		part	:DarkPhantBodyRays01.part
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

End


End		