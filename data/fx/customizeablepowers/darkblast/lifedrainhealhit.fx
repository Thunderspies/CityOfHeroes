#############################################################
## LifeDrainHealHit.fx
#############################################################

FxInfo
LifeSpan	55

########################################################

Input
	Inpname	Hips
End

#########################################################



Condition
	On	Time
	Time	0

	Event
		EName	Ring
		Type	Local
		At	origin

		ChildFx	:DarknessAbsorbPain.fx
		LifeSpan	100
	End

	Event
		EName	Ring
		Type	Local
		At	T_Root
		Part	CustomizeablePowers\Empathy\StaminaRingUp.part
		Sound heal1 70 70 .88
	End
End

Condition
	On	Time
	Time	5

	Event
		EName	Ring
		Type	Local
		At	T_Root
		Part	CustomizeablePowers\Empathy\StaminaRingUp.part
	End
End

Condition
	On	Time
	Time	15

	Event
		EName	Ring
		Type	Local
		At	T_Root
		Part	CustomizeablePowers\Empathy\StaminaRingUp.part
	End
End

Condition
	On	Time
	Time	3

	Event
		EName	1
		Type	Local
		At	T_Hips
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\Empathy\Healing01.part
		Sound dclifedrain2 80 80 1.0
		Lifespan 50

	End

	Event
		EName	2
		Type	Local
		At	T_FootR
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\Empathy\HealingBodyGlows.part
		Part	CustomizeablePowers\Empathy\HealingBodyGlowsFlat.part
		Lifespan 50

	End

	Event
		EName	3
		Type	Local
		At	T_ULEGL
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\Empathy\HealingMotionGlows.part
		PMagnet Knee1
		Lifespan 50

	End

	Event
		EName	4
		Type	Local
		At	T_LLEGL
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\Empathy\HealingMotionGlows.part
		PMagnet FootL
		Lifespan 50

	End

	Event
		EName	5
		Type	Local
		At	T_FootL
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\Empathy\HealingBodyGlows.part
		Part	CustomizeablePowers\Empathy\HealingBodyGlowsFlat.part
	End



	Event
		EName	6
		Type	Local
		At	T_ULEGR
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\Empathy\HealingMotionGlows.part
		PMagnet Kneer
		Lifespan 50

	End

	Event
		EName	7
		Type	Local
		At	T_LLEGR
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\Empathy\HealingMotionGlows.part
		PMagnet FootR
		Lifespan 50

	End

##############################################################################

	Event
		EName	9
		Type	Local
		At	T_Hair
		Bhvr	behaviors\GenericParticleFade.bhvr
		part2	CustomizeablePowers\Empathy\HealingHeadGlows.part
		Lifespan 50
	End

##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	T_UArmL
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\Empathy\HealingArmMotionGlows.part
		Part	CustomizeablePowers\Empathy\HealingArmMotionGlows2.part
		PMagnet ElbowL
		Lifespan 50

	End

	Event
		EName	11
		Type	Local
		At	T_LArmL
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\Empathy\HealingArmMotionGlows.part
		Part	CustomizeablePowers\Empathy\HealingArmMotionGlows2.part
		PMagnet HandL
		Lifespan 50

	End

	Event
		EName	12
		Type	Local
		At	T_WepL
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\Empathy\HealingArmGlows.part
		Lifespan 50

	End

	Event
		EName	13
		Type	Local
		At	T_UArmR
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\Empathy\HealingArmMotionGlows.part
		Part	CustomizeablePowers\Empathy\HealingArmMotionGlows2.part
		PMagnet ElbowL
		Lifespan 50

	End

	Event
		EName	14
		Type	Local
		At	T_LArmR
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\Empathy\HealingArmMotionGlows.part
		Part	CustomizeablePowers\Empathy\HealingArmMotionGlows2.part
		PMagnet HandL
		Lifespan 50

	End

	Event
		EName	15
		Type	Local
		At	T_WepR
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\Empathy\HealingArmGlows.part
		Lifespan 50

	End

End

##################################################################

End