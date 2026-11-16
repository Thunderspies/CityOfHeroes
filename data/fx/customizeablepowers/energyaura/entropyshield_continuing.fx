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
		Sound AuraActivate5 80 80 .33
	End
End

Condition
	On	Time
	Time	0
	

	Event
		Type 	Local
		At	Origin
		Sound entropy3_loop 50.0 50.0 .55
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
	Time	0

	Event	
		Type	Local
		At	Hips	
#		BHVR	:OffsetKnees.bhvr
		Part	CustomizeablePowers\EnergyAura\EntropyGlow.part
		Part	CustomizeablePowers\EnergyAura\EntropyGlowPOPS.part
		
	End
	
	Event	
		Type	Local
		At	Hips	
		#BHVR	:OffsetKnees.bhvr
		Part	CustomizeablePowers\EnergyAura\EntropyGlow.part
		Part	CustomizeablePowers\EnergyAura\EntropyGlowPOPS.part
		Part	CustomizeablePowers\EnergyAura\EntropyGlowPOPS2.part
		
	End

	Event	
		Type	Local
		At	Hips	
		BHVR	CustomizeablePowers\EnergyAura\OffsetKnees.bhvr
		Part	CustomizeablePowers\EnergyAura\PowerGlow.part
		
	End
	
	Event	
		Type	Local
		At	Hips	
		#BHVR	:OffsetKnees.bhvr
		Part	CustomizeablePowers\EnergyAura\PowerGlow.part
		
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