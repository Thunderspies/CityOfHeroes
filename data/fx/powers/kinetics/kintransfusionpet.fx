#########################################################
##	template

FxInfo

Input
	InpName Origin
End

Input
	InpName Root
End

Input  
	InpName	Target
End

Input  
	InpName	Hair
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


#Lifespan 300

#########################################################
####################### AUDIO ###########################
#########################################################


Condition
	On	Time
	Time	0
	

	Event
		Type	Local
		At 	Origin
		Sound blind 80 80 .8
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
		At	Root
		Geom	OverheadDummy
		#Bhvr	:KineticScaleSmall.bhvr
		
	End

	Event	
		ENAME	Swirl1
		Type	Local
		At	SpinNode
		AltPiv	1
		part	:KinecTestSwirl02.part
		part	:KinecTestSwirl01.part
		Bhvr	:KineticSpin1.bhvr
		#Lifespan 50
		
	End

	Event	
		ENAME	Swirl2
		Type	Local
		At	SpinNode
		AltPiv	1
		part	:KinecTestSwirl02.part
		part	:KinecTestSwirl01.part
		Bhvr	:KineticSpin2.bhvr
		#Lifespan 50
		
	End

	Event	
		ENAME	Swirl3
		Type	Local
		At	SpinNode
		AltPiv	1
		part	:KinecTestSwirl02.part
		part	:KinecTestSwirl01.part
		Bhvr	:KineticSpin3.bhvr
		#Lifespan 50
		
	End

	Event	
		ENAME	Glows1
		Type	Local
		At	SpinNode
		AltPiv	1
		part	:KineticExpandingGlows01.part
		part	:KineticExpandingRings01.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		#Lifespan 50
		
	End

	Event	
		ENAME	Flyaways1
		Type	Start
		At	Swirl1
		part	:KineticSwirlFlyAway01.part
		#Bhvr	Behaviors/GenericParticleFade.bhvr
		#Lifespan 50
		
	End

	Event	
		ENAME	Flyaways2
		Type	Start
		At	Swirl2
		part	:KineticSwirlFlyAway02.part
		#Bhvr	Behaviors/GenericParticleFade.bhvr
		#Lifespan 50
		
	End


End



End