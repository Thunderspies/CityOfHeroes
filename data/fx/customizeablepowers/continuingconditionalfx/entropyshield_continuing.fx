#############################################################
## EntropyShield_Continuing.FX
#############################################################

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
		At	:EntropyGlow.part
		Part	:EntropyGlowPOPS.part

	End

	Event
		Type	Local
		At	Hips
		Part	:EntropyGlow.part
		Part	:EntropyGlowPOPS.part
		Part	:EntropyGlowPOPS2.part

	End

	Event
		Type	Local
		At	Hips
		BHVR	CustomizeablePowers\EnergyAura\OffsetKnees.bhvr
		Part	:PowerGlow.part

	End

	Event
		Type	Local
		At	Hips
		Part	:PowerGlow.part

	End

End

#############################################################

End