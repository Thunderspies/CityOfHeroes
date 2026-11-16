#############################################################
## KineticShield_Continuing.FX
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
		Sound AuraActivate4 70 70 .4
	End
End


Condition
	On	Time
	Time	0


	Event
		Type 	Local
		At	Origin
		Sound AuraGlow9_loop 50.0 50.0 .5
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
		Part	:KineticLightRays.part

	End

	Event
		ENAME	SpinNode
		Type	Local
		At	Hips
		Bhvr	CustomizeablePowers\EnergyAura\KineticScale.bhvr

	End

	Event
		ENAME	Glows1
		Type	Local
		At	SpinNode
		part	:KineticExpandingRings02.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		#Lifespan 20

	End



End

#############################################################

End