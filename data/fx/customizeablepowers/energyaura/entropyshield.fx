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
	Time	0

	Event	
		Type	Local
		At	Hips	
#		BHVR	:OffsetKnees.bhvr
		Part	CustomizeablePowers\EnergyAura\EntropyGlowBright.part
		Part	CustomizeablePowers\EnergyAura\EntropyGlowPOPS.part
		
	End
	
	Event	
		Type	Local
		At	Hips	
		#BHVR	:OffsetKnees.bhvr
		Part	CustomizeablePowers\EnergyAura\EntropyGlowBright.part
		Part	CustomizeablePowers\EnergyAura\EntropyGlowPOPS.part
		Part	CustomizeablePowers\EnergyAura\EntropyGlowPOPS2.part
		
	End

	Event	
		Type	Local
		At	Hips	
		BHVR	CustomizeablePowers\EnergyAura\OffsetKnees.bhvr
		Part	CustomizeablePowers\EnergyAura\PowerGlowBright.part
		
	End
	
	Event	
		Type	Local
		At	Hips	
		#BHVR	:OffsetKnees.bhvr
		Part	CustomizeablePowers\EnergyAura\PowerGlowBright.part
		
	End

#	Event	
#		ENAME	SpinNode
#		Type	Local
#		At	Hips	
#		Bhvr	Powers/Kinetics/KineticScale.bhvr
#		
#	End
#
#	Event	
#		ENAME	Glows1
#		Type	Local
#		At	SpinNode
#		part	:KineticExpandingRings02.part
#		Bhvr	Behaviors/GenericParticleFade.bhvr
#		#Lifespan 20
#		
#	End

End


End