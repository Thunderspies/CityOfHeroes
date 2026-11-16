#########################################################
##	template

FxInfo

Lifespan 80

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
		Lifespan 80
	End
End



#########################################################
#################### BODY GLOW 1 ########################
#########################################################


Condition
	On	Time
	Time	24

	Event	
		ENAME	HandGlow
		Type	Local
		At	wepR
		part	:HandGlowElude.part
		Bhvr	:HandRotate.bhvr
		Lifespan 20
	
	End

End

Condition
	On	Time
	Time	26

	Event	
		ENAME	HandGlow
		Type	Local
		At	wepR
		Part	:HandStreakElude.part
		Bhvr	:HandRotate.bhvr
		Lifespan 20
	
	End

End

Condition
	On	Time
	Time	5
	

	Event	
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	:BodyGlowLinesDark.part
		part	:HeadCircles02a.part
		POther Neck
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 80
	
	End

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Head
		part	:BodyGlowLinesDark.part
		part	:HeadCircles02a.part
		POther Chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 80
		
	End
	

	Event	
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part	:BodyGlowLinesDark.part
		part	:HeadCircles02a.part
		POther LarmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 80
		
	End

	
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:BodyGlowLinesDark.part
		part	:HeadCircles02a.part
		POther LarmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 80
		
	End


	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part	:BodyGlowLinesDark.part
		part	:HeadCircles02a.part
		POther LarmL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 80
		
	End
	
	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		
		part	:BodyGlowLinesDark.part
		part	:HeadCircles02a.part
		POther LarmL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 80
		
	End


	Event	
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part	:BodyGlowLinesDark.part
		part	:HeadCircles02a.part
		POther Ulegl
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 80
		
	End

	Event	
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part	:BodyGlowLinesDark.part
		part	:HeadCircles02a.part
		POther Llegl
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 80
		
	End

	Event	
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part	:BodyGlowLinesDark.part
		part	:HeadCircles02a.part
		POther UlegR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 80
		
	End

	Event	
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part	:BodyGlowLinesDark.part
		part	:HeadCircles02a.part
		POther LlegR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 80
		
	End


End

Condition
	On	Cycle
	Time	120
	Chance	.8
	

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Head
		part	:HeadCircles01.part
		part	:HeadCirclesDark.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 80
	
	End


End


End		