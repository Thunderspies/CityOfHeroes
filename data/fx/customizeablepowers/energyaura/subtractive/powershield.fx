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
		BHVR	CustomizeablePowers\EnergyAura\OffsetKnees.bhvr
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
		Bhvr	Behaviors\GenericParticleFade.bhvr
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
		Bhvr	CustomizeablePowers\EnergyAura\KineticScale.bhvr

	End

	Event
		ENAME	Swirl1
		Type	Local
		At	SpinNode
		part	:EnergySwirl.part
		part	:SwirlEnergy.part

		Bhvr	CustomizeablePowers\EnergyAura\KineticSpin1.bhvr
		#Lifespan 20

	End

	Event
		ENAME	Swirl2
		Type	Local
		At	SpinNode
		part	:EnergySwirlBright.part
		part	:SwirlEnergyBright.part
		Bhvr	CustomizeablePowers\EnergyAura\KineticSpin2.bhvr
		#Lifespan 20

	End

	Event
		ENAME	Swirl3
		Type	Local
		At	SpinNode
		part	:EnergySwirlBright.part
		part	:SwirlEnergyBright.part
		Bhvr	CustomizeablePowers\EnergyAura\KineticSpin3.bhvr
		#Lifespan 20

	End

####################################################################################################
##Opposite Rings
################################################################################

	Event
		ENAME	SpinNode2
		Type	Local
		At	Hips
		Bhvr	CustomizeablePowers\EnergyAura\KineticScale.bhvr

	End

	Event
		ENAME	Swirl1
		Type	Local
		At	SpinNode2
		part	:EnergySwirlBright.part
		part	:SwirlEnergyBright.part
		Bhvr	CustomizeablePowers\EnergyAura\KineticSpin1.bhvr
		#Lifespan 20

	End

	Event
		ENAME	Swirl2
		Type	Local
		At	SpinNode2
		part	:EnergySwirlBright.part
		part	:SwirlEnergyBright.part
		Bhvr	CustomizeablePowers\EnergyAura\KineticSpin2.bhvr
		#Lifespan 20

	End

	Event
		ENAME	Swirl3
		Type	Local
		At	SpinNode2
		part	:EnergySwirlBright.part
		part	:SwirlEnergyBright.part
		Bhvr	CustomizeablePowers\EnergyAura\KineticSpin3.bhvr
		#Lifespan 20

	End

End


End