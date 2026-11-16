#############################################################
## Omega_Eyes_Huge.fx
#############################################################

FxInfo

Flags NotCompatibleWithAnimalHead

#########################################################
#################### BODY GLOW 1 ########################
#########################################################

Condition
	On	Time
	Time	0

	Event
		EName 	EyeAnchor
		Type	Local
		At	Eyes
		Bhvr	behaviors/GenericParticleFade.bhvr
		Geom	CircleOfThorns
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	1
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Plasma_Core_EyeL.part
		Part	:Smoke_Long_EyeL.part
		Part	:Eye_GlowL.part
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	2
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Plasma_Core_EyeR.part
		Part	:Smoke_Long_EyeR.part
		Part	:Eye_GlowR.part
	End
End

#########################################################

End