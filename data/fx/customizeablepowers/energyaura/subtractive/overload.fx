#############################################################
## OverLoad.fx
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
		Sound rampup 80 80 .86
	End
End


#########################################################
#################### BODY GLOW 1 ########################
#########################################################


Condition
	On	Cycle
	Time	15
	Chance	.5

	Event
		Type	Local
		At	Hips
		BHVR	CustomizeablePowers\EnergyAura\OffsetKnees.bhvr
		Part	:PowerGlows.part
		Part	:PowerGlows.part
		LifeSpan	10
	End
End

Condition
	On	Cycle
	Time	5
	Chance	.5

	Event
		Type	Local
		At	Hips
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:PowerGlows.part
		Part	:PowerGlows.part
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
		part	:EnergySwirlBright2.part
		part	:SwirlEnergyBright2.part

		Bhvr	CustomizeablePowers\EnergyAura\KineticSpin1a.bhvr

	End

	Event
		ENAME	Swirl2
		Type	Local
		At	SpinNode
		part	:EnergySwirlBright2.part
		part	:SwirlEnergyBright2.part
		Bhvr	CustomizeablePowers\EnergyAura\KineticSpin2a.bhvr

	End

	Event
		ENAME	Swirl3
		Type	Local
		At	SpinNode
		part	:EnergySwirlBright2.part
		part	:SwirlEnergyBright2.part
		Bhvr	CustomizeablePowers\EnergyAura\KineticSpin3a.bhvr

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
		part	:EnergySwirlBright2.part
		part	:SwirlEnergyBright2.part
		Bhvr	CustomizeablePowers\EnergyAura\KineticSpin1a.bhvr

	End

	Event
		ENAME	Swirl2
		Type	Local
		At	SpinNode2
		part	:EnergySwirlBright2.part
		part	:SwirlEnergyBright2.part
		Bhvr	CustomizeablePowers\EnergyAura\KineticSpin2a.bhvr

	End

	Event
		ENAME	Swirl3
		Type	Local
		At	SpinNode2
		part	:EnergySwirlBright2.part
		part	:SwirlEnergyBright2.part
		Bhvr	CustomizeablePowers\EnergyAura\KineticSpin3a.bhvr

	End

#########################################################################################################


	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	:EnergyCloakStealthBodyGlow01.part
		part	:EnergyCloakStealthBodyGlow02.part
		part	:EnergyCloakStealthBodyGlowBlue01.part
		part	:EnergyCloakStealthBodyGlowBlue02.part
		POther Neck
		Lifespan 0

	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part	:EnergyCloakStealthBodyGlow01.part
		part	:EnergyCloakStealthBodyGlow02.part
		part	:EnergyCloakStealthBodyGlowBlue01.part
		part	:EnergyCloakStealthBodyGlowBlue02.part
		POther Chest
		Lifespan 0

	End


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part	:EnergyCloakStealthBodyGlow01.part
		part	:EnergyCloakStealthBodyGlow02.part
		part	:EnergyCloakStealthBodyGlowBlue01.part
		part	:EnergyCloakStealthBodyGlowBlue02.part
		POther LarmR
		Lifespan 0

	End


	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:EnergyCloakStealthBodyGlow01.part
		part	:EnergyCloakStealthBodyGlow02.part
		part	:EnergyCloakStealthBodyGlowBlue01.part
		part	:EnergyCloakStealthBodyGlowBlue02.part
		POther LarmR
		Lifespan 0

	End


	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part	:EnergyCloakStealthBodyGlow01.part
		part	:EnergyCloakStealthBodyGlow02.part
		part	:EnergyCloakStealthBodyGlowBlue01.part
		part	:EnergyCloakStealthBodyGlowBlue02.part
		POther LarmL
		Lifespan 0

	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:EnergyCloakStealthBodyGlow01.part
		part	:EnergyCloakStealthBodyGlow02.part
		part	:EnergyCloakStealthBodyGlowBlue01.part
		part	:EnergyCloakStealthBodyGlowBlue02.part
		POther LarmL
		Lifespan 0

	End


	Event
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part	:EnergyCloakStealthBodyGlow01.part
		part	:EnergyCloakStealthBodyGlow02.part
		part	:EnergyCloakStealthBodyGlowBlue01.part
		part	:EnergyCloakStealthBodyGlowBlue02.part
		POther Ulegl
		Lifespan 0

	End

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part	:EnergyCloakStealthBodyGlow01.part
		part	:EnergyCloakStealthBodyGlow02.part
		part	:EnergyCloakStealthBodyGlowBlue01.part
		part	:EnergyCloakStealthBodyGlowBlue02.part
		POther Llegl
		Lifespan 0

	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part	:EnergyCloakStealthBodyGlow01.part
		part	:EnergyCloakStealthBodyGlow02.part
		part	:EnergyCloakStealthBodyGlowBlue01.part
		part	:EnergyCloakStealthBodyGlowBlue02.part
		POther UlegR
		Lifespan 0

	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part	:EnergyCloakStealthBodyGlow01.part
		part	:EnergyCloakStealthBodyGlow02.part
		part	:EnergyCloakStealthBodyGlowBlue01.part
		part	:EnergyCloakStealthBodyGlowBlue02.part

		POther LlegR
		Lifespan 0

	End

############################################################################################################


	Event
		Type	Local
		At	Hips
		BHVR	CustomizeablePowers\EnergyAura\OffsetKnees.bhvr
		Part	:EntropyGlow.part
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

###########################################################################################

	Event
		Type	Local
		At	Hips
		Part	:KineticLightRaysBright2.part

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

	End
End

###################################################################################

End