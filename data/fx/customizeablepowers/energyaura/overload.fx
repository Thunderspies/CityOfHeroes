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
		Part	CustomizeablePowers\EnergyAura\PowerGlows.part
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
		At	Hips	
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\EnergyAura\PowerGlows.part
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
		part	CustomizeablePowers\EnergyAura\EnergySwirlBright2.part
		part	CustomizeablePowers\EnergyAura\SwirlEnergyBright2.part

		Bhvr	CustomizeablePowers\EnergyAura\KineticSpin1a.bhvr
		#Lifespan 20
		
	End

	Event	
		ENAME	Swirl2
		Type	Local
		At	SpinNode
		part	CustomizeablePowers\EnergyAura\EnergySwirlBright2.part
		part	CustomizeablePowers\EnergyAura\SwirlEnergyBright2.part
		Bhvr	CustomizeablePowers\EnergyAura\KineticSpin2a.bhvr
		#Lifespan 20
		
	End

	Event	
		ENAME	Swirl3
		Type	Local
		At	SpinNode
		part	CustomizeablePowers\EnergyAura\EnergySwirlBright2.part
		part	CustomizeablePowers\EnergyAura\SwirlEnergyBright2.part
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
		part	CustomizeablePowers\EnergyAura\EnergySwirlBright2.part
		part	CustomizeablePowers\EnergyAura\SwirlEnergyBright2.part
		Bhvr	CustomizeablePowers\EnergyAura\KineticSpin1a.bhvr
		#Lifespan 20
		
	End

	Event	
		ENAME	Swirl2
		Type	Local
		At	SpinNode2
		part	CustomizeablePowers\EnergyAura\EnergySwirlBright2.part
		part	CustomizeablePowers\EnergyAura\SwirlEnergyBright2.part
		Bhvr	CustomizeablePowers\EnergyAura\KineticSpin2a.bhvr
		#Lifespan 20
		
	End

	Event	
		ENAME	Swirl3
		Type	Local
		At	SpinNode2
		part	CustomizeablePowers\EnergyAura\EnergySwirlBright2.part
		part	CustomizeablePowers\EnergyAura\SwirlEnergyBright2.part
		Bhvr	CustomizeablePowers\EnergyAura\KineticSpin3a.bhvr
		#Lifespan 20
		
	End

#########################################################################################################


	Event	
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	CustomizeablePowers\EnergyAura\EnergyCloakStealthBodyGlow01.part
		part	CustomizeablePowers\EnergyAura\EnergyCloakStealthBodyGlow02.part
		part	CustomizeablePowers\EnergyAura\EnergyCloakStealthBodyGlowBlue01.part
		part	CustomizeablePowers\EnergyAura\EnergyCloakStealthBodyGlowBlue02.part
		POther Neck
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
	
	End

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part	CustomizeablePowers\EnergyAura\EnergyCloakStealthBodyGlow01.part
		part	CustomizeablePowers\EnergyAura\EnergyCloakStealthBodyGlow02.part
		part	CustomizeablePowers\EnergyAura\EnergyCloakStealthBodyGlowBlue01.part
		part	CustomizeablePowers\EnergyAura\EnergyCloakStealthBodyGlowBlue02.part
		POther Chest
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End
	

	Event	
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part	CustomizeablePowers\EnergyAura\EnergyCloakStealthBodyGlow01.part
		part	CustomizeablePowers\EnergyAura\EnergyCloakStealthBodyGlow02.part
		part	CustomizeablePowers\EnergyAura\EnergyCloakStealthBodyGlowBlue01.part
		part	CustomizeablePowers\EnergyAura\EnergyCloakStealthBodyGlowBlue02.part
		POther LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	CustomizeablePowers\EnergyAura\EnergyCloakStealthBodyGlow01.part
		part	CustomizeablePowers\EnergyAura\EnergyCloakStealthBodyGlow02.part
		part	CustomizeablePowers\EnergyAura\EnergyCloakStealthBodyGlowBlue01.part
		part	CustomizeablePowers\EnergyAura\EnergyCloakStealthBodyGlowBlue02.part
		POther LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End


	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part	CustomizeablePowers\EnergyAura\EnergyCloakStealthBodyGlow01.part
		part	CustomizeablePowers\EnergyAura\EnergyCloakStealthBodyGlow02.part
		part	CustomizeablePowers\EnergyAura\EnergyCloakStealthBodyGlowBlue01.part
		part	CustomizeablePowers\EnergyAura\EnergyCloakStealthBodyGlowBlue02.part
		POther LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End
	
	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	CustomizeablePowers\EnergyAura\EnergyCloakStealthBodyGlow01.part
		part	CustomizeablePowers\EnergyAura\EnergyCloakStealthBodyGlow02.part
		part	CustomizeablePowers\EnergyAura\EnergyCloakStealthBodyGlowBlue01.part
		part	CustomizeablePowers\EnergyAura\EnergyCloakStealthBodyGlowBlue02.part
		POther LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End


	Event	
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part	CustomizeablePowers\EnergyAura\EnergyCloakStealthBodyGlow01.part
		part	CustomizeablePowers\EnergyAura\EnergyCloakStealthBodyGlow02.part
		part	CustomizeablePowers\EnergyAura\EnergyCloakStealthBodyGlowBlue01.part
		part	CustomizeablePowers\EnergyAura\EnergyCloakStealthBodyGlowBlue02.part
		POther Ulegl
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part	CustomizeablePowers\EnergyAura\EnergyCloakStealthBodyGlow01.part
		part	CustomizeablePowers\EnergyAura\EnergyCloakStealthBodyGlow02.part
		part	CustomizeablePowers\EnergyAura\EnergyCloakStealthBodyGlowBlue01.part
		part	CustomizeablePowers\EnergyAura\EnergyCloakStealthBodyGlowBlue02.part
		POther Llegl
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part	CustomizeablePowers\EnergyAura\EnergyCloakStealthBodyGlow01.part
		part	CustomizeablePowers\EnergyAura\EnergyCloakStealthBodyGlow02.part
		part	CustomizeablePowers\EnergyAura\EnergyCloakStealthBodyGlowBlue01.part
		part	CustomizeablePowers\EnergyAura\EnergyCloakStealthBodyGlowBlue02.part
		POther UlegR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part	CustomizeablePowers\EnergyAura\EnergyCloakStealthBodyGlow01.part
		part	CustomizeablePowers\EnergyAura\EnergyCloakStealthBodyGlow02.part
		part	CustomizeablePowers\EnergyAura\EnergyCloakStealthBodyGlowBlue01.part
		part	CustomizeablePowers\EnergyAura\EnergyCloakStealthBodyGlowBlue02.part
		
		POther LlegR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

############################################################################################################


	Event	
		Type	Local
		At	Hips	
		BHVR	CustomizeablePowers\EnergyAura\OffsetKnees.bhvr
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

###########################################################################################

	Event	
		Type	Local
		At	Hips	
		Part	CustomizeablePowers\EnergyAura\KineticLightRaysBright2.part
		
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
		part	CustomizeablePowers\EnergyAura\KineticExpandingRings02.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		#Lifespan 20
		
	End

###################################################################################


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