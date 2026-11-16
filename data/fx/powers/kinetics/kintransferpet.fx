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
		Sound blind 80 80 .8
	End
End


#########################################################
#################### BODY GLOW 1 ########################
#########################################################


Condition
	On	Cycle 
	Time	60
	
	Event	
		ENAME	SpinNode
		Type	Local
		At	Hips	
		Bhvr	:KineticScale.bhvr
		Lifespan 50
		
	End

	Event	
		ENAME	Swirl1
		Type	Local
		At	SpinNode
		part	:KinecTestSwirl02.part
		part	:KinecTestSwirl01.part
		Bhvr	:KineticSpin1.bhvr
		Lifespan 50
		
	End

	Event	
		ENAME	Swirl2
		Type	Local
		At	SpinNode
		part	:KinecTestSwirl02.part
		part	:KinecTestSwirl01.part
		Bhvr	:KineticSpin2.bhvr
		Lifespan 50
		
	End

	Event	
		ENAME	Swirl3
		Type	Local
		At	SpinNode
		part	:KinecTestSwirl02.part
		part	:KinecTestSwirl01.part
		Bhvr	:KineticSpin3.bhvr
		Lifespan 50
		
	End

	Event	
		ENAME	Glows1
		Type	Local
		At	SpinNode
		part	:KineticExpandingGlows01.part
		part	:KineticExpandingRings01.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 50
		
	End

	Event	
		ENAME	Flyaways1
		Type	Start
		At	Swirl1
		part	:KineticSwirlFlyAway01.part
		#Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 50
		
	End

	Event	
		ENAME	Flyaways2
		Type	Start
		At	Swirl2
		part	:KineticSwirlFlyAway02.part
		#Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 50
		
	End


End	


End