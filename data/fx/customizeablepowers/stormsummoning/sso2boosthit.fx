#############################################################
## SSO2BoostHit.fx
#############################################################

FxInfo
Lifespan 70

#########################################################

Input
	Inpname	Hips
End

#########################################################


#########################################################
#####################  Mist 1  #########################
#########################################################

Condition

	On 	Time
	Time	0

	Event
		ENAME	CloudSmall
		Type	Posit
		At	Root
		part	CustomizeablePowers\StormSummoning\SSO2BGroundClouds.part
		LifeSpan 30

	End

End

Condition

	On 	Time
	Time	10

	Event
		ENAME	CloudMed
		Type	Posit
		At	Root
		part	CustomizeablePowers\StormSummoning\SSO2BGroundClouds02.part
		PMagnet	Hair
		LifeSpan 30

	End

	Event
		Ename	LevitationForce
		Type	Local
		At	Root

		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		8
			PhysForcePower		35
			PhysForcePowerJitter	5
		End

		Lifespan	30

	End

End

Condition

	On 	Time
	Time	25

	Event
		Ename	GravitationalForce
		Type	Local
		At	Chest

		BhvrOverride
			PhysForceType		In
			PhysForceRadius		10
			PhysForcePower		35
			PhysForcePowerJitter	10
		End

		Lifespan	50

	End

	Event
		ENAME	ChestMist
		Type	Posit
		At	Chest
		part	CustomizeablePowers\StormSummoning\SSO2BBodyMist01.part
		part	CustomizeablePowers\StormSummoning\SSO2BBodyMolecules01.part
		LifeSpan 50
	End

	Event
		ENAME	HairMist
		Type	Posit
		At	Hair
		part	CustomizeablePowers\StormSummoning\SSO2BBodyMist01.part
		part	CustomizeablePowers\StormSummoning\SSO2BBodyMolecules01.part
		LifeSpan 50
	End

	Event
		ENAME	WepLMist
		Type	Posit
		At	WepL
		part	CustomizeablePowers\StormSummoning\SSO2BBodyMist01.part
		part	CustomizeablePowers\StormSummoning\SSO2BBodyMolecules01.part
		LifeSpan 50
	End

	Event
		ENAME	WepRMist
		Type	Posit
		At	WepR
		part	CustomizeablePowers\StormSummoning\SSO2BBodyMist01.part
		part	CustomizeablePowers\StormSummoning\SSO2BBodyMolecules01.part
		LifeSpan 50
	End

	Event
		ENAME	FootLMist
		Type	Posit
		At	FootL
		part	CustomizeablePowers\StormSummoning\SSO2BBodyMist01.part
		part	CustomizeablePowers\StormSummoning\SSO2BBodyMolecules01.part
		LifeSpan 50
	End

	Event
		ENAME	FootRMist
		Type	Posit
		At	FootR
		part	CustomizeablePowers\StormSummoning\SSO2BBodyMist01.part
		part	CustomizeablePowers\StormSummoning\SSO2BBodyMolecules01.part
		LifeSpan 50
	End

End

##################################

Condition
	On	Time
	Time	25

	Event
		EName	Ring
		Type	Local
		At	Root
		part1	CustomizeablePowers/Empathy/StaminaRingUp.part
		Sound heal1 70 70 .8

	End

End

Condition
	On	Time
	Time	30

	Event
		EName	Ring
		Type	Local
		At	Root
		part1	CustomizeablePowers/Empathy/StaminaRingUp.part

	End

End

Condition
	On	Time
	Time	45

	Event
		EName	Ring
		Type	Local
		At	Root
		part1	CustomizeablePowers/Empathy/StaminaRingUp.part

	End

End

Condition
	On	Time
	Time	28

	Event
		EName	1
		Type	Local
		At	Hips
		part1	CustomizeablePowers/Empathy/Healing01.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
	End

	Event
		EName	2
		Type	Local
		At	FootR
		part1	CustomizeablePowers/Empathy/HealingBodyGlows.part
		part2	CustomizeablePowers/Empathy/HealingBodyGlowsFlat.part
	End


#LegL



	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	CustomizeablePowers/Empathy/HealingMotionGlows.part
		PMagnet Knee1

	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	CustomizeablePowers/Empathy/HealingMotionGlows.part
		PMagnet FootL
	End

	Event
		EName	5
		Type	Local
		At	FootL
		part1	CustomizeablePowers/Empathy/HealingBodyGlows.part
		part2	CustomizeablePowers/Empathy/HealingBodyGlowsFlat.part
	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	CustomizeablePowers/Empathy/HealingMotionGlows.part
		PMagnet Kneer

	End


#LegR


	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	CustomizeablePowers/Empathy/HealingMotionGlows.part
		PMagnet FootR
	End

###############################################################################

	Event
		EName	9
		Type	Local
		At	Hair
		part2	CustomizeablePowers/Empathy/HealingHeadGlows.part
	End

##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	CustomizeablePowers/Empathy/HealingArmMotionGlows.part
		part2	CustomizeablePowers/Empathy/HealingArmMotionGlows2.part
		PMagnet ElbowL

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	CustomizeablePowers/Empathy/HealingArmMotionGlows.part
		part2	CustomizeablePowers/Empathy/HealingArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		EName	12
		Type	Local
		At	WepL
		part1	CustomizeablePowers/Empathy/HealingArmGlows.part

	End






	Event
		EName	13
		Type	Local
		At	UArmR
		part1	CustomizeablePowers/Empathy/HealingArmMotionGlows.part
		part2	CustomizeablePowers/Empathy/HealingArmMotionGlows2.part
		PMagnet ElbowL

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	CustomizeablePowers/Empathy/HealingArmMotionGlows.part
		part2	CustomizeablePowers/Empathy/HealingArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		EName	15
		Type	Local
		At	WepR
		part1	CustomizeablePowers/Empathy/HealingArmGlows.part

	End



End

##########################################################################

End