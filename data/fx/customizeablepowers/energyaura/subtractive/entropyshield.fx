#############################################################
## EntropyShield.fx
#############################################################

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
		Part	:EntropyGlowBright.part
		Part	:EntropyGlowPOPS.part

	End

	Event
		Type	Local
		At	Hips
		#BHVR	:OffsetKnees.bhvr
		Part	:EntropyGlowBright.part
		Part	:EntropyGlowPOPS.part
		Part	:EntropyGlowPOPS2.part

	End

	Event
		Type	Local
		At	Hips
		BHVR	CustomizeablePowers\EnergyAura\OffsetKnees.bhvr
		Part	:PowerGlowBright.part

	End

	Event
		Type	Local
		At	Hips
		#BHVR	:OffsetKnees.bhvr
		Part	:PowerGlowBright.part

	End


End


End