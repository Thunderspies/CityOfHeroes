#########################################################
##	template

FxInfo

###lifespan 100

#########################################################
####################### AUDIO ###########################
#########################################################




Condition
	On	Time
	Time	0
	

	Event	
		ENAME	ChestGlow
		Type	positonly
		At	Hips
		part1	:AnalyzerDebuff.part
		POther Neck
		#Bhvr	Behaviors/TelportationFade.bhvr
		#lifespan 45
	
	End

	Event	
		ENAME	HeadGlow
		Type	positonly
		At	Hair
		part1	:AnalyzerDebuff.part
		POther Chest
		#Bhvr	Behaviors/TelportationFade.bhvr
		#lifespan 40
		
	End
	

	Event	
		ENAME	RightUarmGlow
		Type	positonly
		At	SpadR
		part1	:AnalyzerDebuff.part
		POther LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr
		#lifespan 40
		
	End

	
	Event	
		ENAME	RightLarmGlow
		Type	positonly
		At	WepR
		part1	:AnalyzerDebuff.part
		POther LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr
		#lifespan 35
		
	End


	Event	
		ENAME	LeftUarmGlow
		Type	positonly
		At	SpadL
		part1	:AnalyzerDebuff.part
		POther LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		#lifespan 40
		
	End
	
	Event	
		ENAME	LeftLarmGlow
		Type	positonly
		At	WepL
		part1	:AnalyzerDebuff.part
		POther LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		#lifespan 35
		
	End


	Event	
		ENAME	LeftULegGlow
		Type	positonly
		At	LlegL
		part1	:AnalyzerDebuff.part
		POther Ulegl
		#Bhvr	Behaviors/TelportationFade.bhvr
		#lifespan 50
		
	End

	Event	
		ENAME	LeftLLegGlow
		Type	positonly
		At	FootL
		part1	:AnalyzerDebuff.part
		POther Llegl
		#Bhvr	Behaviors/TelportationFade.bhvr
		#lifespan 55
		
	End

	Event	
		ENAME	RightULegGlow
		Type	positonly
		At	LlegR
		part1	:AnalyzerDebuff.part
		POther UlegR
		#Bhvr	Behaviors/TelportationFade.bhvr
		#lifespan 50
		
	End

	Event	
		ENAME	RightLLegGlow
		Type	positonly
		At	FootR
		part1	:AnalyzerDebuff.part
		POther LlegR
		#Bhvr	Behaviors/TelportationFade.bhvr
		#lifespan 55
		
	End


End

End