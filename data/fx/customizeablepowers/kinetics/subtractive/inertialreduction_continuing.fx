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

#Lifespan 200


#########################################################
####################### AUDIO ###########################
#########################################################

Condition
	On	Time
	Time	0
	

	Event
		Type	Local
		At 	Origin
		Sound kin2_loop 80 80 .22
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
		part1	CustomizeablePowers\Kinetics\KinIDBodyStreaks01.part
		POther Neck
		#Bhvr	Behaviors/TelportationFade.bhvr
		#Lifespan 3
	
	End

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Head
		part1	CustomizeablePowers\Kinetics\KinIDBodyStreaks01.part
		POther Hair
		#Bhvr	Behaviors/TelportationFade.bhvr
		#Lifespan 3
		
	End
	

	Event	
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part1	CustomizeablePowers\Kinetics\KinIDBodyStreaks01.part
		POther LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr
		#Lifespan 3
		
	End

	Event	
		ENAME	RightWepGlow
		Type	Local
		At	WepR
		part1	CustomizeablePowers\Kinetics\KinIDBodyStreaks01.part
		POther LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr
		#Lifespan 3
		
	End

	Event	
		ENAME	LeftWepGlow
		Type	Local
		At	WepL
		part1	CustomizeablePowers\Kinetics\KinIDBodyStreaks01.part
		POther LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		#Lifespan 3
		
	End

	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part1	CustomizeablePowers\Kinetics\KinIDBodyStreaks01.part
		POther LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		#Lifespan 3
		
	End
	

	Event	
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part1	CustomizeablePowers\Kinetics\KinIDBodyStreaks01.part
		POther Ulegl
		#Bhvr	Behaviors/TelportationFade.bhvr
		#Lifespan 3
		
	End


	Event	
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part1	CustomizeablePowers\Kinetics\KinIDBodyStreaks01.part
		POther UlegR
		#Bhvr	Behaviors/TelportationFade.bhvr
		#Lifespan 3
		
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
		At	Chest	
		Bhvr	CustomizeablePowers\Kinetics\KineticScaleSmall.bhvr
		
	End

	Event	
		ENAME	Swirl1
		Type	Local
		At	SpinNode
		Bhvr	CustomizeablePowers\Kinetics\KineticSpin1.bhvr
		
	End

	Event	
		ENAME	Swirl2
		Type	Local
		At	SpinNode
		Bhvr	CustomizeablePowers\Kinetics\KineticSpin2.bhvr
		
	End

	Event	
		ENAME	Swirl3
		Type	Local
		At	SpinNode
		Bhvr	CustomizeablePowers\Kinetics\KineticSpin3.bhvr
		
	End

End

Condition
	On	Cycle
	Time	30

	Event	
		ENAME	Flyaways1
		Type	Posit
		At	Swirl1
		part	CustomizeablePowers\Kinetics\KineticIDSwirlFlyAway01.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 50
		
	End


End	

Condition
	On	Cycle
	Time	25

	
	Event	
		ENAME	Flyaways2
		Type	Posit
		At	Swirl2
		part	CustomizeablePowers\Kinetics\KineticIDSwirlFlyAway01.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 50
		
	End


End	

End		