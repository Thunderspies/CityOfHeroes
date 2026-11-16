#############################################################
## DarknessAbsorbPainWithRaysAndHeal.fx
#############################################################

FxInfo
Lifespan	50

########################################################

Input
	Inpname	Hips
End

########################################################

Condition
	On 	Time
	Time 	0

	Event
		ENAME   Tendrills
		Type	Local
		At	chest
		Part3	CustomizeablePowers\DarkMelee\DamageHitMist2.part
		Part1	CustomizeablePowers\DarkMelee\SoulDrainHitTendril2.part
		Part2	CustomizeablePowers\DarkMelee\SoulDrainHitPuddle2.part
		Part4	CustomizeablePowers\DarkMelee\RedRays.part
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
		Bhvr 	behaviors/GenericParticleFade.bhvr
		part1	:Healing02.part


	End

	Event
		EName	2
		Type	Local
		At	T_FootR
		Bhvr 	behaviors/GenericParticleFade.bhvr
		part1	CustomizeablePowers\Empathy\HealingBodyGlows.part
		part2	CustomizeablePowers\Empathy\HealingBodyGlowsFlat.part

	End


#LegL



	Event
		EName	3
		Type	Local
		At	T_ULEGL
		Bhvr 	behaviors/GenericParticleFade.bhvr
		part1	CustomizeablePowers\Empathy\HealingMotionGlows.part
		PMagnet Knee1

	End

	Event
		EName	4
		Type	Local
		At	T_LLEGL
		Bhvr 	behaviors/GenericParticleFade.bhvr
		part1	CustomizeablePowers\Empathy\HealingMotionGlows.part
		PMagnet FootL
	End

	Event
		EName	5
		Type	Local
		At	T_FootL
		Bhvr 	behaviors/GenericParticleFade.bhvr
		part1	CustomizeablePowers\Empathy\HealingBodyGlows.part
		part2	CustomizeablePowers\Empathy\HealingBodyGlowsFlat.part
	End



	Event
		EName	6
		Type	Local
		At	T_ULEGR
		Bhvr 	behaviors/GenericParticleFade.bhvr
		part1	CustomizeablePowers\Empathy\HealingMotionGlows.part
		PMagnet Kneer

	End


	Event
		EName	7
		Type	Local
		At	T_LLEGR
		Bhvr 	behaviors/GenericParticleFade.bhvr
		part1	CustomizeablePowers\Empathy\HealingMotionGlows.part
		PMagnet FootR
	End


	Event
		EName	9
		Type	Local
		At	T_Hair
		Bhvr 	behaviors/GenericParticleFade.bhvr
		part2	CustomizeablePowers\Empathy\HealingHeadGlows.part
	End

##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	T_UArmL
		Bhvr 	behaviors/GenericParticleFade.bhvr
		part1	CustomizeablePowers\Empathy\HealingArmMotionGlows.part
		part2	CustomizeablePowers\Empathy\HealingArmMotionGlows2.part
		PMagnet ElbowL

	End

	Event
		EName	11
		Type	Local
		At	T_LArmL
		Bhvr 	behaviors/GenericParticleFade.bhvr
		part1	CustomizeablePowers\Empathy\HealingArmMotionGlows.part
		part2	CustomizeablePowers\Empathy\HealingArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		EName	12
		Type	Local
		At	T_WepL
		Bhvr 	behaviors/GenericParticleFade.bhvr
		part1	CustomizeablePowers\Empathy\HealingArmGlows.part

	End

	Event
		EName	13
		Type	Local
		At	T_UArmR
		Bhvr 	behaviors/GenericParticleFade.bhvr
		part1	CustomizeablePowers\Empathy\HealingArmMotionGlows.part
		part2	CustomizeablePowers\Empathy\HealingArmMotionGlows2.part
		PMagnet ElbowL

	End

	Event
		EName	14
		Type	Local
		At	T_LArmR
		Bhvr 	behaviors/GenericParticleFade.bhvr
		part1	CustomizeablePowers\Empathy\HealingArmMotionGlows.part
		part2	CustomizeablePowers\Empathy\HealingArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		EName	15
		Type	Local
		At	T_WepR
		Bhvr 	behaviors/GenericParticleFade.bhvr
		part1	CustomizeablePowers\Empathy\HealingArmGlows.part

	End

End


Condition
	On	Time
	Time	40

	Event
		EName	all
		Type	Destroy

	End

End

End
##################################

