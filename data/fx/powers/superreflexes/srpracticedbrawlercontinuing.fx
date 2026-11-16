#########################################################
##	template

FxInfo

Input  
	InpName	Root
End

Input  
	InpName	Head
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

#Lifespan 900


#########################################################
####################### AUDIO ###########################
#########################################################

### see below in cycle


#########################################################
#################### BODY GLOW 2 ########################
#########################################################

Condition
	On	Time
	Time	15

	Event
		EName	2
		Type	Local
		At	Chest
		Sound fast7_loop 60 60 .66
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 180
	End
End


Condition
	On	Cycle
	Time	60
	

	Event	
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part1	:SRBodyGlowLines02.part
		POther Neck
		Bhvr	Behaviors/GenericParticleFade.bhvr
		#Sound	fast7 80 60 .6
		Lifespan 20
	
	End

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Head
		part1	:SRBodyGlowLines02.part
		POther Chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 20
		
	End
	

	Event	
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part1	:SRBodyGlowLines02.part
		POther LarmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 20
		
	End

	
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part1	:SRBodyGlowLines02.part
		POther LarmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 20
		
	End


	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part1	:SRBodyGlowLines02.part
		POther LarmL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 20
		
	End
	
	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part1	:SRBodyGlowLines02.part
		POther LarmL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 20
		
	End


	Event	
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part1	:SRBodyGlowLines02.part
		POther Ulegl
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 20
		
	End

	Event	
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part1	:SRBodyGlowLines02.part
		POther Llegl
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 20
		
	End

	Event	
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part1	:SRBodyGlowLines02.part
		POther UlegR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 20
		
	End

	Event	
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part1	:SRBodyGlowLines02.part
		POther LlegR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 20
		
	End


End


End		