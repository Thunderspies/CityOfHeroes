#########################################################
##	template

FxInfo

Lifespan 100

#########################################################
####################### AUDIO ###########################
#########################################################


Condition
	On	Time
	Time	0
	

	Event
		Type	Local
		At 	Origin
		Sound kinetic4 80 80 .8
	End
End



#########################################################
#################### BODY GLOW 1 ########################
#########################################################


Condition
	On	Time 
	Time	0
	
	Event	
		ENAME	SpinNode
		Type	Local
		At	Hips	
		#Bhvr	POWERS\Kinetics\KineticScaleLarge.bhvr
		
	End

	Event	
		ENAME	Swirl1
		Type	Local
		At	SpinNode
		part	:AnalyzerTestSwirl02.part
		part	:AnalyzerTestSwirl01.part
		Bhvr	POWERS\Kinetics\KineticSpin1.bhvr
		Lifespan 20
		
	End

	Event	
		ENAME	Swirl2
		Type	Local
		At	SpinNode
		part	:AnalyzerTestSwirl02.part
		part	:AnalyzerTestSwirl01.part
		Bhvr	POWERS\Kinetics\KineticSpin2.bhvr
		Lifespan 20
		
	End

	Event	
		ENAME	Swirl3
		Type	Local
		At	SpinNode
		part	:AnalyzerTestSwirl02.part
		part	:AnalyzerTestSwirl01.part
		Bhvr	POWERS\Kinetics\KineticSpin3.bhvr
		Lifespan 20
		
	End

	Event	
		ENAME	Glows1
		Type	Local
		At	SpinNode
		part	:AnalyzerExpandingGlows01.part
		part	:AnalyzerExpandingRings01.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 20
		
	End

	Event	
		ENAME	Flyaways1
		Type	Start
		At	Swirl1
		part	:AnalyzerSwirlFlyAway01.part
		#Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 20
		
	End

	Event	
		ENAME	Flyaways2
		Type	Start
		At	Swirl2
		part	:AnalyzerSwirlFlyAway02.part
		#Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 20
		
	End


End


#########################################################
#################### BODY GLOW 2 ########################
#########################################################



Condition
	On	Time
	Time	0
	

	Event	
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part1	:AnalyzerBodyGlowsBlue01.part
		POther Neck
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 45
	
	End

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part1	:AnalyzerBodyGlowsBlue01.part
		POther Chest
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 40
		
	End
	

	Event	
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part1	:AnalyzerBodyGlowsBlue01.part
		POther LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 40
		
	End

	
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part1	:AnalyzerBodyGlowsBlue01.part
		POther LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 35
		
	End


	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part1	:AnalyzerBodyGlowsBlue01.part
		POther LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 40
		
	End
	
	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part1	:AnalyzerBodyGlowsBlue01.part
		POther LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 35
		
	End


	Event	
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part1	:AnalyzerBodyGlowsBlue01.part
		POther Ulegl
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 50
		
	End

	Event	
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part1	:AnalyzerBodyGlowsBlue01.part
		POther Llegl
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 55
		
	End

	Event	
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part1	:AnalyzerBodyGlowsBlue01.part
		POther UlegR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 50
		
	End

	Event	
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part1	:AnalyzerBodyGlowsBlue01.part
		POther LlegR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 55
		
	End


End

End