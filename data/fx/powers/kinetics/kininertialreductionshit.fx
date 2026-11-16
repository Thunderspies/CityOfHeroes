#########################################################
##	template

FxInfo

Input
	InpName Origin
End
Input
	InpName Target
End

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






Lifespan 200

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
		Bhvr	:KineticScale.bhvr
		
	End

	Event	
		ENAME	Swirl1
		Type	Local
		At	SpinNode
		part	:KinecTestSwirl02.part
		part	:KinecTestSwirl01.part
		Bhvr	:KineticSpin1.bhvr
		Lifespan 20
		
	End

	Event	
		ENAME	Swirl2
		Type	Local
		At	SpinNode
		part	:KinecTestSwirl02.part
		part	:KinecTestSwirl01.part
		Bhvr	:KineticSpin2.bhvr
		Lifespan 20
		
	End

	Event	
		ENAME	Swirl3
		Type	Local
		At	SpinNode
		part	:KinecTestSwirl02.part
		part	:KinecTestSwirl01.part
		Bhvr	:KineticSpin3.bhvr
		Lifespan 20
		
	End

	Event	
		ENAME	Glows1
		Type	Local
		At	SpinNode
		part	:KineticExpandingGlows01.part
		part	:KineticExpandingRings01.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 20
		
	End

	Event	
		ENAME	Flyaways1
		Type	Start
		At	Swirl1
		part	:KineticSwirlFlyAway01.part
		#Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 20
		
	End

	Event	
		ENAME	Flyaways2
		Type	Start
		At	Swirl2
		part	:KineticSwirlFlyAway02.part
		#Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 20
		
	End


End

##########################################################
##################### BODY GLOW 2 ########################
##########################################################
#
#
#
#Condition
#	On	Time
#	Time	0
#	
#
#	Event	
#		ENAME	ChestGlow
#		Type	Local
#		At	Hips
#		part1	:KineticSpeedBodyGlowsCyan01.part
#		POther Neck
#		#Bhvr	Behaviors/TelportationFade.bhvr
#		Lifespan 45
#	
#	End
#
#	Event	
#		ENAME	HeadGlow
#		Type	Local
#		At	Hair
#		part1	:KineticSpeedBodyGlowsCyan01.part
#		POther Chest
#		#Bhvr	Behaviors/TelportationFade.bhvr
#		Lifespan 40
#		
#	End
#	
#
#	Event	
#		ENAME	RightUarmGlow
#		Type	Local
#		At	SpadR
#		part1	:KineticSpeedBodyGlowsCyan01.part
#		POther LarmR
#		#Bhvr	Behaviors/TelportationFade.bhvr
#		Lifespan 40
#		
#	End
#
#	
#	Event	
#		ENAME	RightLarmGlow
#		Type	Local
#		At	WepR
#		part1	:KineticSpeedBodyGlowsCyan01.part
#		POther LarmR
#		#Bhvr	Behaviors/TelportationFade.bhvr
#		Lifespan 35
#		
#	End
#
#
#	Event	
#		ENAME	LeftUarmGlow
#		Type	Local
#		At	SpadL
#		part1	:KineticSpeedBodyGlowsCyan01.part
#		POther LarmL
#		#Bhvr	Behaviors/TelportationFade.bhvr
#		Lifespan 40
#		
#	End
#	
#	Event	
#		ENAME	LeftLarmGlow
#		Type	Local
#		At	WepL
#		part1	:KineticSpeedBodyGlowsCyan01.part
#		POther LarmL
#		#Bhvr	Behaviors/TelportationFade.bhvr
#		Lifespan 35
#		
#	End
#
#
#	Event	
#		ENAME	LeftULegGlow
#		Type	Local
#		At	LlegL
#		part1	:KineticSpeedBodyGlowsCyan01.part
#		POther Ulegl
#		#Bhvr	Behaviors/TelportationFade.bhvr
#		Lifespan 50
#		
#	End
#
#	Event	
#		ENAME	LeftLLegGlow
#		Type	Local
#		At	FootL
#		part1	:KineticSpeedBodyGlowsCyan01.part
#		POther Llegl
#		#Bhvr	Behaviors/TelportationFade.bhvr
#		Lifespan 55
#		
#	End
#
#	Event	
#		ENAME	RightULegGlow
#		Type	Local
#		At	LlegR
#		part1	:KineticSpeedBodyGlowsCyan01.part
#		POther UlegR
#		#Bhvr	Behaviors/TelportationFade.bhvr
#		Lifespan 50
#		
#	End
#
#	Event	
#		ENAME	RightLLegGlow
#		Type	Local
#		At	FootR
#		part1	:KineticSpeedBodyGlowsCyan01.part
#		POther LlegR
#		#Bhvr	Behaviors/TelportationFade.bhvr
#		Lifespan 55
#		
#	End
#
#
#End


End