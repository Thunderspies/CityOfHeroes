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
		Sound Invisible1 80 80 .46
	End
End




Condition
	On	Time
	Time	10
	

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
	Time	0
	

	Event	
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	:DarkIllusDecoyChestGlow01.part
		part	:DarkIllusDecoyBodyGlow02.part
		part	:DarkIllusDecoyBodyNoise01.part
		part	:DarkIllusPhantBodyRays01.part
		POther Neck
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
	
	End

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part	:DarkIllusDecoyBodyGlow01.part
		part	:DarkIllusDecoyBodyGlow02.part
		#part	:DarkIllusDecoyBodyNoise01.part
		part	:DarkIllusPhantBodyRays01.part
		POther Chest
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End
	

	Event	
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part	:DarkIllusDecoyBodyGlow01.part
		part	:DarkIllusDecoyBodyGlow02.part
		part	:DarkIllusDecoyBodyNoise01.part
		part	:DarkIllusPhantBodyRays01.part
		POther LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:DarkIllusDecoyBodyGlow01.part
		part	:DarkIllusDecoyBodyGlow02.part
		part	:DarkIllusDecoyBodyNoise01.part
		part	:DarkIllusPhantBodyRays01.part
		POther LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End


	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part	:DarkIllusDecoyBodyGlow01.part
		part	:DarkIllusDecoyBodyGlow02.part
		part	:DarkIllusDecoyBodyNoise01.part
		part	:DarkIllusPhantBodyRays01.part
		POther LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End
	
	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:DarkIllusDecoyBodyGlow01.part
		part	:DarkIllusDecoyBodyGlow02.part
		part	:DarkIllusDecoyBodyNoise01.part
		part	:DarkIllusPhantBodyRays01.part
		POther LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End


	Event	
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part	:DarkIllusDecoyBodyGlow01.part
		part	:DarkIllusDecoyBodyGlow02.part
		part	:DarkIllusDecoyBodyNoise01.part
		part	:DarkIllusPhantBodyRays01.part
		POther Ulegl
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part	:DarkIllusDecoyBodyGlow01.part
		part	:DarkIllusDecoyBodyGlow02.part
		part	:DarkIllusDecoyBodyNoise01.part
		part	:DarkIllusPhantBodyRays01.part
		POther Llegl
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part	:DarkIllusDecoyBodyGlow01.part
		part	:DarkIllusDecoyBodyGlow02.part
		part	:DarkIllusDecoyBodyNoise01.part
		part	:DarkIllusPhantBodyRays01.part
		POther UlegR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part	:DarkIllusDecoyBodyGlow01.part
		part	:DarkIllusDecoyBodyGlow02.part
		part	:DarkIllusDecoyBodyNoise01.part
		part	:DarkIllusPhantBodyRays01.part
		POther LlegR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End


End


End		