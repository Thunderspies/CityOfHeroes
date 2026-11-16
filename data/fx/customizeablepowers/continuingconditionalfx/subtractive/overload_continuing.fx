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
		#Sound 	OverloadActivate 80 60 .5
	End
End

Condition
	On	Time
	Time	0


	Event
		Type 	Local
		At	Origin
		Sound Overload1_loop 50.0 50.0 .55
		Bhvr	Behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 390
	End

End


Condition
	On	Time
	Time	0


	Event
		Type 	Local
		At	Origin
		Sound Radiation5_loop 50.0 50.0 .66
		Bhvr	Behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 390
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
		Part	:PowerGlowsCont.part
		Part	:PowerGlowsCont.part
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
		Part	:PowerGlowsCont.part
		Part	:PowerGlowsCont.part
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
		part	:EnergySwirlBright2Cont.part
		part	:SwirlEnergyBright2Cont.part

		Bhvr	CustomizeablePowers\EnergyAura\KineticSpin1a.bhvr
		#Lifespan 20

	End

	Event
		ENAME	Swirl2
		Type	Local
		At	SpinNode
		part	:EnergySwirlBright2Cont.part
		part	:SwirlEnergyBright2Cont.part
		Bhvr	CustomizeablePowers\EnergyAura\KineticSpin2a.bhvr
		#Lifespan 20

	End

	Event
		ENAME	Swirl3
		Type	Local
		At	SpinNode
		part	:EnergySwirlBright2Cont.part
		part	:SwirlEnergyBright2Cont.part
		Bhvr	CustomizeablePowers\EnergyAura\KineticSpin3a.bhvr
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
		part	:EnergySwirlBright2Cont.part
		part	:SwirlEnergyBright2Cont.part
		Bhvr	CustomizeablePowers\EnergyAura\KineticSpin1a.bhvr
		#Lifespan 20

	End

	Event
		ENAME	Swirl2
		Type	Local
		At	SpinNode2
		part	:EnergySwirlBright2Cont.part
		part	:SwirlEnergyBright2Cont.part
		Bhvr	CustomizeablePowers\EnergyAura\KineticSpin2a.bhvr
		#Lifespan 20

	End

	Event
		ENAME	Swirl3
		Type	Local
		At	SpinNode2
		part	:EnergySwirlBright2Cont.part
		part	:SwirlEnergyBright2Cont.part
		Bhvr	CustomizeablePowers\EnergyAura\KineticSpin3a.bhvr
		#Lifespan 20

	End

#########################################################################################################


	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	:EnergyCloakStealthBodyGlow01Cont.part
		part	:EnergyCloakStealthBodyGlow02Cont.part
		part	:EnergyCloakStealthBodyGlowBlue01Cont.part
		part	:EnergyCloakStealthBodyGlowBlue02Cont.part
		POther Neck
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0

	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part	:EnergyCloakStealthBodyGlow01Cont.part
		part	:EnergyCloakStealthBodyGlow02Cont.part
		part	:EnergyCloakStealthBodyGlowBlue01Cont.part
		part	:EnergyCloakStealthBodyGlowBlue02Cont.part
		POther Chest
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0

	End


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part	:EnergyCloakStealthBodyGlow01Cont.part
		part	:EnergyCloakStealthBodyGlow02Cont.part
		part	:EnergyCloakStealthBodyGlowBlue01Cont.part
		part	:EnergyCloakStealthBodyGlowBlue02Cont.part
		POther LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0

	End


	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:EnergyCloakStealthBodyGlow01Cont.part
		part	:EnergyCloakStealthBodyGlow02Cont.part
		part	:EnergyCloakStealthBodyGlowBlue01Cont.part
		part	:EnergyCloakStealthBodyGlowBlue02Cont.part
		POther LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0

	End


	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part	:EnergyCloakStealthBodyGlow01Cont.part
		part	:EnergyCloakStealthBodyGlow02Cont.part
		part	:EnergyCloakStealthBodyGlowBlue01Cont.part
		part	:EnergyCloakStealthBodyGlowBlue02Cont.part
		POther LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0

	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:EnergyCloakStealthBodyGlow01Cont.part
		part	:EnergyCloakStealthBodyGlow02Cont.part
		part	:EnergyCloakStealthBodyGlowBlue01Cont.part
		part	:EnergyCloakStealthBodyGlowBlue02Cont.part
		POther LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0

	End


	Event
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part	:EnergyCloakStealthBodyGlow01Cont.part
		part	:EnergyCloakStealthBodyGlow02Cont.part
		part	:EnergyCloakStealthBodyGlowBlue01Cont.part
		part	:EnergyCloakStealthBodyGlowBlue02Cont.part
		POther Ulegl
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0

	End

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part	:EnergyCloakStealthBodyGlow01Cont.part
		part	:EnergyCloakStealthBodyGlow02Cont.part
		part	:EnergyCloakStealthBodyGlowBlue01Cont.part
		part	:EnergyCloakStealthBodyGlowBlue02Cont.part
		POther Llegl
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0

	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part	:EnergyCloakStealthBodyGlow01Cont.part
		part	:EnergyCloakStealthBodyGlow02Cont.part
		part	:EnergyCloakStealthBodyGlowBlue01Cont.part
		part	:EnergyCloakStealthBodyGlowBlue02Cont.part
		POther UlegR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0

	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part	:EnergyCloakStealthBodyGlow01Cont.part
		part	:EnergyCloakStealthBodyGlow02Cont.part
		part	:EnergyCloakStealthBodyGlowBlue01Cont.part
		part	:EnergyCloakStealthBodyGlowBlue02Cont.part

		POther LlegR
		#Bhvr	Behaviors/TelportationFade.bhvr
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
		#BHVR	:OffsetKnees.bhvr
		Part	:EntropyGlow.part
		Part	:EntropyGlowPOPS.part
		Part	:EntropyGlowPOPS2.part

	End

	Event
		Type	Local
		At	neck
		#BHVR	:OffsetKnees.bhvr
		Part	:PowerGlowsCont.part

	End

	Event
		Type	Local
		At	neck
		#BHVR	:OffsetKnees.bhvr
		Part	:PowerGlowsCont.part

	End

###########################################################################################

	Event
		Type	Local
		At	chest
		Part	:KineticLightRaysBright2Cont.part

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
		part	:KineticExpandingRings02Cont.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		#Lifespan 20

	End

###################################################################################

End


End