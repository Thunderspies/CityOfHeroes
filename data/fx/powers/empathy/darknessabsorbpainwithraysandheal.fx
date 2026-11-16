#############################################################
## DarknessAbsorbPainWithRaysandHeal.fx
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
		Part3	Powers/DarknessControl/DamageHitMist2.part
		Part1	Powers/DarknessControl/SoulDrainHitTendril2.part
		Part2	Powers/DarknessControl/SoulDrainHitPuddle2.part
		Part4	Powers/DarknessControl/RedRays.part
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
		part1	Powers/Healing/Healing02.part


	End

	Event
		EName	2
		Type	Local
		At	T_FootR
		part1	Powers/Healing/HealingBodyGlows.part
		part2	Powers/Healing/HealingBodyGlowsFlat.part

	End


#LegL



	Event
		EName	3
		Type	Local
		At	T_ULEGL
		part1	Powers/Healing/HealingMotionGlows.part
		PMagnet Knee1

	End

	Event
		EName	4
		Type	Local
		At	T_LLEGL
		part1	Powers/Healing/HealingMotionGlows.part
		PMagnet FootL
	End

	Event
		EName	5
		Type	Local
		At	T_FootL
		part1	Powers/Healing/HealingBodyGlows.part
		part2	Powers/Healing/HealingBodyGlowsFlat.part
	End



	Event
		EName	6
		Type	Local
		At	T_ULEGR
		part1	Powers/Healing/HealingMotionGlows.part
		PMagnet Kneer

	End


#LegR


	Event
		EName	7
		Type	Local
		At	T_LLEGR
		part1	Powers/Healing/HealingMotionGlows.part
		PMagnet FootR
	End

###############################################################################

	Event
		EName	9
		Type	Local
		At	T_Hair
		part2	Powers/Healing/HealingHeadGlows.part
	End

##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	T_UArmL
		part1	Powers/Healing/HealingArmMotionGlows.part
		part2	Powers/Healing/HealingArmMotionGlows2.part
		PMagnet ElbowL

	End

	Event
		EName	11
		Type	Local
		At	T_LArmL
		part1	Powers/Healing/HealingArmMotionGlows.part
		part2	Powers/Healing/HealingArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		EName	12
		Type	Local
		At	T_WepL
		part1	Powers/Healing/HealingArmGlows.part

	End

	Event
		EName	13
		Type	Local
		At	T_UArmR
		part1	Powers/Healing/HealingArmMotionGlows.part
		part2	Powers/Healing/HealingArmMotionGlows2.part
		PMagnet ElbowL

	End

	Event
		EName	14
		Type	Local
		At	T_LArmR
		part1	Powers/Healing/HealingArmMotionGlows.part
		part2	Powers/Healing/HealingArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		EName	15
		Type	Local
		At	T_WepR
		part1	Powers/Healing/HealingArmGlows.part

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

