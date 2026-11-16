#########################################################
##	template

FxInfo

Input  
	InpName	Root
End

Input  
	InpName	Hair
End

Input  
	InpName	Eyes
End

Input  
	InpName	Neck
End

Input  
	InpName	Chest
End

Input  
	InpName	SpadR
End

Input  
	InpName	SpadL
End

Input  
	InpName	UarmR
End

Input  
	InpName	UarmL
End

Input  
	InpName	LarmR
End

Input  
	InpName	LarmL
End

Input  
	InpName	WepR
End

Input  
	InpName	WepL
End

Input  
	InpName	Hips
End

Input  
	InpName	UlegR
End

Input  
	InpName	UlegL
End

Input  
	InpName	LlegR
End

Input  
	InpName	LlegL
End

Input  
	InpName	FootR
End

Input  
	InpName	FootL
End

Input  
	InpName	ToeL
End

Input  
	InpName	ToeR
End

Input
	InpName Origin
End

LifeSpan 60

#########################################################
####################### AUDIO ###########################
#########################################################

Condition
	On	Time
	Time 	8
	
	Event
		Type	Local
		At 	Origin
		Sound invisible1 100 100 .6
	End
End



#########################################################
#################### BODY GLOW 1 ########################
#########################################################



Condition
	On	Time
	Time	20
	

	Event	
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	:IllusPhantBodyGlow01.part
		POther Neck
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
	
	End

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part	:IllusPhantBodyGlow01.part
		POther Chest
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End
	

	Event	
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part	:IllusPhantBodyGlow01.part
		POther LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:IllusPhantBodyGlow01.part
		POther LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End


	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part	:IllusPhantBodyGlow01.part
		POther LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End
	
	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:IllusPhantBodyGlow01.part
		POther LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End


	Event	
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part	:IllusPhantBodyGlow01.part
		POther Ulegl
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part	:IllusPhantBodyGlow01.part
		POther Llegl
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part	:IllusPhantBodyGlow01.part
		POther UlegR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part	:IllusPhantBodyGlow01.part
		POther LlegR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End


End


End		