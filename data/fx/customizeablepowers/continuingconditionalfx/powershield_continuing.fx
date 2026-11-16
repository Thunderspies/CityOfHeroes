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
		Sound AuraActivate1 80 80 .5
	End
End


Condition
	On	Time
	Time	0
	

	Event
		Type 	Local
		At	Origin
		Sound AuraGlow6_loop 50.0 50.0 .45
		Bhvr	Behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 390
	End

End


#########################################################
#################### BODY GLOW 1 ########################
#########################################################


Condition
	On	Time
	Time	15
	
	Event	
		Type	Local
		At	Chest	
		BHVR	CustomizeablePowers\EnergyAura\OffsetKnees.bhvr
		Part	CustomizeablePowers\EnergyAura\EntropyGlowPOPS.part
		Part	CustomizeablePowers\EnergyAura\EntropyGlowPOPS2.part

	End
End

Condition
	On	Cycle
	Time	15
	Chance	.5

	Event	
		Type	Local
		At	chest	
		BHVR	CustomizeablePowers\EnergyAura\OffsetKnees.bhvr
		Part	CustomizeablePowers\EnergyAura\PowerGlows.part
		LifeSpan	10
	End
End

Condition
	On	Cycle
	Time	5
	Chance	.5
	
	Event	
		Type	Local
		At	chest	
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\EnergyAura\PowerGlows.part
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
		part	CustomizeablePowers\EnergyAura\EnergySwirl.part
		part	CustomizeablePowers\EnergyAura\SwirlEnergy.part

		Bhvr	CustomizeablePowers\EnergyAura\KineticSpin1.bhvr
		#Lifespan 20
		
	End

	Event	
		ENAME	Swirl2
		Type	Local
		At	SpinNode
		part	CustomizeablePowers\EnergyAura\EnergySwirl.part
		part	CustomizeablePowers\EnergyAura\SwirlEnergy.part
		Bhvr	CustomizeablePowers\EnergyAura\KineticSpin2.bhvr
		#Lifespan 20
		
	End

	Event	
		ENAME	Swirl3
		Type	Local
		At	SpinNode
		part	CustomizeablePowers\EnergyAura\EnergySwirl.part
		part	CustomizeablePowers\EnergyAura\SwirlEnergy.part
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
		part	CustomizeablePowers\EnergyAura\EnergySwirl.part
		part	CustomizeablePowers\EnergyAura\SwirlEnergy.part
		Bhvr	CustomizeablePowers\EnergyAura\KineticSpin1.bhvr
		#Lifespan 20
		
	End

	Event	
		ENAME	Swirl2
		Type	Local
		At	SpinNode2
		part	CustomizeablePowers\EnergyAura\EnergySwirl.part
		part	CustomizeablePowers\EnergyAura\SwirlEnergy.part
		Bhvr	CustomizeablePowers\EnergyAura\KineticSpin2.bhvr
		#Lifespan 20
		
	End

	Event	
		ENAME	Swirl3
		Type	Local
		At	SpinNode2
		part	CustomizeablePowers\EnergyAura\EnergySwirl.part
		part	CustomizeablePowers\EnergyAura\SwirlEnergy.part
		Bhvr	CustomizeablePowers\EnergyAura\KineticSpin3.bhvr
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