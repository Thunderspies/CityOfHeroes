#########################################################
##	template

FxInfo

LifeSpan	20

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
	Time	5
#	Chance	.5

	Event	
		Type	Local
		At	Hips	
		BHVR	:OffsetKnees.bhvr
		Part	:PowerGlowBright.part
		LifeSpan	10
	End
End

Condition
	On	Time
	Time	0
#	Chance	.5
	
	Event	
		Type	Local
		At	Hips	
		BHVR	Behaviors/GenericParticleFade.bhvr
		Part	:PowerGlowBright.part
		LifeSpan	10
		
	End

End

##################################################################################################

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
		part	:EnergySwirl.part
		part	:SwirlEnergy.part

		Bhvr	Powers/Kinetics/KineticSpin1.bhvr
		#Lifespan 20
		
	End

	Event	
		ENAME	Swirl2
		Type	Local
		At	SpinNode
		part	:EnergySwirlBright.part
		part	:SwirlEnergyBright.part
		Bhvr	Powers/Kinetics/KineticSpin2.bhvr
		#Lifespan 20
		
	End

	Event	
		ENAME	Swirl3
		Type	Local
		At	SpinNode
		part	:EnergySwirlBright.part
		part	:SwirlEnergyBright.part
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
		part	:EnergySwirlBright.part
		part	:SwirlEnergyBright.part
		Bhvr	:KineticSpin1.bhvr
		#Lifespan 20
		
	End

	Event	
		ENAME	Swirl2
		Type	Local
		At	SpinNode2
		part	:EnergySwirlBright.part
		part	:SwirlEnergyBright.part
		Bhvr	:KineticSpin2.bhvr
		#Lifespan 20
		
	End

	Event	
		ENAME	Swirl3
		Type	Local
		At	SpinNode2
		part	:EnergySwirlBright.part
		part	:SwirlEnergyBright.part
		Bhvr	:KineticSpin3.bhvr
		#Lifespan 20
		
	End

#	Event	
#		Type	Local
#		At	Chest	
#		BHVR	:OffsetFront.bhvr
#		Geom	PowerShield01
#		
#	End
#	
#	Event	
#		Type	Local
#		At	Chest	
#		BHVR	:OffsetBack.bhvr
#		Geom	PowerShield02
#	
#	End
#
End


End