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
		Sound Fast2 70 70 .7
	End
End



#########################################################
#################### BODY GLOW 1 ########################
#########################################################



Condition
	On	Time
	Time	5
	

	Event	
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part1	:BodyGlowLinesDark.part
		part1	:HeadCircles02.part
		POther Neck
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 30
	
	End

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Head
		part1	:BodyGlowLinesDark.part
		part1	:HeadCircles02.part
		POther Chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 30
		
	End
	

	Event	
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part1	:BodyGlowLinesDark.part
		part1	:HeadCircles02.part
		POther LarmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 30
		
	End

	
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part1	:BodyGlowLinesDark.part
		part1	:HeadCircles02.part
		POther LarmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 30
		
	End


	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part1	:BodyGlowLinesDark.part
		part1	:HeadCircles02.part
		POther LarmL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 30
		
	End
	
	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part1	:BodyGlowLinesDark.part
		part1	:HeadCircles02.part
		POther LarmL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 30
		
	End


	Event	
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part1	:BodyGlowLinesDark.part
		part1	:HeadCircles02.part
		POther Ulegl
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 30
		
	End

	Event	
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part1	:BodyGlowLinesDark.part
		part1	:HeadCircles02.part
		POther Llegl
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 30
		
	End

	Event	
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part1	:BodyGlowLinesDark.part
		part1	:HeadCircles02.part
		POther UlegR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 30
		
	End

	Event	
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part1	:BodyGlowLinesDark.part
		part1	:HeadCircles02.part
		POther LlegR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 30
		
	End


End

Condition
	On	Cycle
	Time	120
	

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Head
		part1	:HeadCircles01.part
		part1	:HeadCirclesDark.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 30
	
	End


End


End		