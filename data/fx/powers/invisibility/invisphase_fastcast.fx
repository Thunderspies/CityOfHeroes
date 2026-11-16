#########################################################
##	template

FxInfo

LifeSpan 95


#########################################################
####################### AUDIO ###########################
#########################################################

condition
	On 	Time
	Time	10

	Event	
		Type	Local
		At	Origin
		Sound phase2 100 100 .7
	End
End



condition
	On 	Time
	Time	10
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
	Time	15
	
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
	Time	15

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
	Time	20
	

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
	Time	25

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
	Time	30

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
	Time	35

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
	Time	40
	
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