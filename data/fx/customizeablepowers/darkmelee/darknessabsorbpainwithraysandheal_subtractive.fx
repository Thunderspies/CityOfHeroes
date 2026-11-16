#############################################################
## DarknessAbsorbPainWithRaysandHeal_Subtractive.fx
#############################################################

FxInfo
Lifespan	40

########################################################



Condition
	On 	Time
	Time 	0

	Event
		ENAME   Tendrills
		Type	Local
		At	chest
		Part	CustomizeablePowers\DarkMelee\SoulDrainHitTendril2_Subtractive.part
		Part	CustomizeablePowers\DarkMelee\SoulDrainHitPuddle2_Subtractive.Part
		Part	CustomizeablePowers\DarkMelee\DamageHitMist2.part
		Part	CustomizeablePowers\DarkMelee\SoulDrainHitPuddle2.part
		Part	CustomizeablePowers\DarkMelee\RedRays.part
		SOund	dclifedrain2 80 33 1.0
		LifeSpan	30
	End

End

Condition
	On	Time
	Time	10

	Event
		EName	Ring
		Type	Local
		At	T_Root

		Sound heal1 70 70 .88

	End


	Event
		EName	1
		Type	Local
		At	T_Hips
		part	CustomizeablePowers\Empathy\Healing02.part


	End

	Event
		EName	2
		Type	Local
		At	T_FootR
		part	CustomizeablePowers\Empathy\HealingBodyGlows.part
		part	CustomizeablePowers\Empathy\HealingBodyGlowsFlat.part

	End

	Event
		EName	3
		Type	Local
		At	T_ULEGL
		part	CustomizeablePowers\Empathy\HealingMotionGlows.part
		PMagnet Knee1

	End

	Event
		EName	4
		Type	Local
		At	T_LLEGL
		part	CustomizeablePowers\Empathy\HealingMotionGlows.part
		PMagnet FootL
	End

	Event
		EName	5
		Type	Local
		At	T_FootL
		part	CustomizeablePowers\Empathy\HealingBodyGlows.part
		part	CustomizeablePowers\Empathy\HealingBodyGlowsFlat.part
	End



	Event
		EName	6
		Type	Local
		At	T_ULEGR
		part	CustomizeablePowers\Empathy\HealingMotionGlows.part
		PMagnet Kneer

	End

	Event
		EName	7
		Type	Local
		At	T_LLEGR
		part	CustomizeablePowers\Empathy\HealingMotionGlows.part
		PMagnet FootR
	End

###############################################################################

	Event
		EName	9
		Type	Local
		At	T_Hair
		part	CustomizeablePowers\Empathy\HealingHeadGlows.part
	End

##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	T_UArmL
		part	CustomizeablePowers\Empathy\HealingArmMotionGlows.part
		part	CustomizeablePowers\Empathy\HealingArmMotionGlows2.part
		PMagnet ElbowL

	End

	Event
		EName	11
		Type	Local
		At	T_LArmL
		part	CustomizeablePowers\Empathy\HealingArmMotionGlows.part
		part	CustomizeablePowers\Empathy\HealingArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		EName	12
		Type	Local
		At	T_WepL
		part	CustomizeablePowers\Empathy\HealingArmGlows.part

	End

	Event
		EName	13
		Type	Local
		At	T_UArmR
		part	CustomizeablePowers\Empathy\HealingArmMotionGlows.part
		part	CustomizeablePowers\Empathy\HealingArmMotionGlows2.part
		PMagnet ElbowL

	End

	Event
		EName	14
		Type	Local
		At	T_LArmR
		part	CustomizeablePowers\Empathy\HealingArmMotionGlows.part
		part	CustomizeablePowers\Empathy\HealingArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		EName	15
		Type	Local
		At	T_WepR
		part	CustomizeablePowers\Empathy\HealingArmGlows.part

	End

End

##################################

End

