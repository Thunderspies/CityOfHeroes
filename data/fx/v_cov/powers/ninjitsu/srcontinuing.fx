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
		Sound Glow7_loop 80 80 .5
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
		part1	:SRBodyGlowLines01.part
		part1	:SRBodyGlows01.part
		POther Neck
		#Bhvr	Behaviors/TelportationFade.bhvr
		#Lifespan 3
	
	End

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Head
		#part1	:SRBodyGlowLines01.part
		part1	:SRBodyGlows01.part
		POther Chest
		#Bhvr	Behaviors/TelportationFade.bhvr
		#Lifespan 3
		
	End
	

	Event	
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part1	:SRBodyGlowLines01.part
		part1	:SRBodyGlows01.part
		POther LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr
		#Lifespan 3
		
	End

	
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part1	:SRBodyStreaks01.part
		part1	:SRBodyGlowLines01.part
		part1	:SRBodyGlows01.part
		POther LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr
		#Lifespan 3
		
	End


	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part1	:SRBodyGlowLines01.part
		part1	:SRBodyGlows01.part
		POther LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		#Lifespan 3
		
	End
	
	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part1	:SRBodyStreaks01.part
		part1	:SRBodyGlowLines01.part
		part1	:SRBodyGlows01.part
		POther LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		#Lifespan 3
		
	End


	Event	
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part1	:SRBodyGlowLines01.part
		part1	:SRBodyGlows01.part
		POther Ulegl
		#Bhvr	Behaviors/TelportationFade.bhvr
		#Lifespan 3
		
	End

	Event	
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part1	:SRBodyStreaks01.part
		part1	:SRBodyGlowLines01.part
		part1	:SRBodyGlows01.part
		POther Llegl
		#Bhvr	Behaviors/TelportationFade.bhvr
		#Lifespan 3
		
	End

	Event	
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part1	:SRBodyGlowLines01.part
		part1	:SRBodyGlows01.part
		POther UlegR
		#Bhvr	Behaviors/TelportationFade.bhvr
		#Lifespan 3
		
	End

	Event	
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part1	:SRBodyStreaks01.part
		part1	:SRBodyGlowLines01.part
		part1	:SRBodyGlows01.part
		POther LlegR
		#Bhvr	Behaviors/TelportationFade.bhvr
		#Lifespan 3
		
	End


End


End		