#########################################################
##	template

FxInfo

Flags	InheritAnimScale

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


LifeSpan 115


#########################################################
####################### AUDIO ###########################
#########################################################

condition
	On 	Time
	Time	14

	Event	
		Type	Local
		At	Origin
		Sound phase2 100 100 .7
	End
End



condition
	On 	Time
	Time	50
	Event	
		Type	Local
		At	Origin
		Sound invisible1 100 100 .5
	End
End



#########################################################
#################### BODY GLOW 1 ########################
#########################################################

Condition
	On	Time
	Time	60
	
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:InvisPhaseBodyGlow01.part
		part	:InvisPhaseBodyGlow03.part
		POther LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:InvisPhaseBodyGlow01.part
		part	:InvisPhaseBodyGlow03.part
		POther LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

End

Condition
	On	Time
	Time	70

	Event	
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part	:InvisPhaseBodyGlow01.part
		part	:InvisPhaseBodyGlow03.part
		POther LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part	:InvisPhaseBodyGlow01.part
		part	:InvisPhaseBodyGlow03.part
		POther LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

End

Condition
	On	Time
	Time	75
	

	Event	
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	:InvisPhaseBodyGlow01.part
		part	:InvisPhaseBodyGlow03.part
		POther Neck
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
	
	End

End

Condition
	On	Time
	Time	80

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part	:InvisPhaseBodyGlow01.part
		part	:InvisPhaseBodyGlow03.part
		POther Chest
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

End
	
Condition
	On	Time
	Time	85

	Event	
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part	:InvisPhaseBodyGlow01.part
		part	:InvisPhaseBodyGlow03.part
		POther Ulegl
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part	:InvisPhaseBodyGlow01.part
		part	:InvisPhaseBodyGlow03.part
		POther UlegR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

End

Condition
	On	Time
	Time	90

	Event	
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part	:InvisPhaseBodyGlow01.part
		part	:InvisPhaseBodyGlow03.part
		POther Llegl
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part	:InvisPhaseBodyGlow01.part
		part	:InvisPhaseBodyGlow03.part
		POther LlegR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End


End

#########################################################
#################### BODY BURST  ########################
#########################################################

Condition
	On	Time
	Time	95
	
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:InvisPhaseBodyGlow04.part
		POther LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:InvisPhaseBodyGlow04.part
		POther LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part	:InvisPhaseBodyGlow04.part
		POther LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part	:InvisPhaseBodyGlow04.part
		POther LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	:InvisPhaseBodyGlow04.part
		POther Neck
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
	
	End

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part	:InvisPhaseBodyGlow04.part
		POther Chest
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part	:InvisPhaseBodyGlow04.part
		POther Ulegl
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part	:InvisPhaseBodyGlow04.part
		POther UlegR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part	:InvisPhaseBodyGlow04.part
		POther Llegl
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part	:InvisPhaseBodyGlow04.part
		POther LlegR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End


End

End		