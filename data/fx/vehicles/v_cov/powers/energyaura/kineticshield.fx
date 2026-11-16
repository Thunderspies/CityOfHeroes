#########################################################
##	template

FxInfo

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
		Sound kinetic4 80 80 .84
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
		Bhvr	Powers/Kinetics/KineticScale.bhvr
		
	End

	Event	
		ENAME	Swirl1
		Type	Local
		At	SpinNode
		part	:FaintKinecTestSwirl02.part
		part	:FaintKinecTestSwirl01.part
		Bhvr	Powers/Kinetics/KineticSpin1.bhvr
		#Lifespan 20
		
	End

	Event	
		ENAME	Swirl2
		Type	Local
		At	SpinNode
		part	:FaintKinecTestSwirl02.part
		part	:FaintKinecTestSwirl01.part
		Bhvr	Powers/Kinetics/KineticSpin2.bhvr
		#Lifespan 20
		
	End

	Event	
		ENAME	Swirl3
		Type	Local
		At	SpinNode
		part	:FaintKinecTestSwirl02.part
		part	:FaintKinecTestSwirl01.part
		Bhvr	Powers/Kinetics/KineticSpin3.bhvr
		#Lifespan 20
		
	End

####################################################################################################
##Opposite Rings
################################################################################
	
	Event	
		ENAME	SpinNode2
		Type	Local
		At	Hips	
		Bhvr	:KineticScale.bhvr
		
	End

	Event	
		ENAME	Swirl1
		Type	Local
		At	SpinNode2
		part	:FaintKinecTestSwirl02.part
		part	:FaintKinecTestSwirl01.part
		Bhvr	:KineticSpin1.bhvr
		#Lifespan 20
		
	End

	Event	
		ENAME	Swirl2
		Type	Local
		At	SpinNode2
		part	:FaintKinecTestSwirl02.part
		part	:FaintKinecTestSwirl01.part
		Bhvr	:KineticSpin2.bhvr
		#Lifespan 20
		
	End

	Event	
		ENAME	Swirl3
		Type	Local
		At	SpinNode2
		part	:FaintKinecTestSwirl02.part
		part	:FaintKinecTestSwirl01.part
		Bhvr	:KineticSpin3.bhvr
		#Lifespan 20
		
	End

##############################################################################################

	Event	
		ENAME	Glows1
		Type	Local
		At	SpinNode
		part	:KineticExpandingRings01.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		#Lifespan 20
		
	End



End


End