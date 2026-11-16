#########################################################
##	Revive Healing
FxInfo

LifeSpan	100

Input
	InpName	Hips
End

Input
	InpName	FootL
End

Input
	InpName	FootR
End

Input
	InpName	ULEGL
End

Input
	InpName	ULEGR
End

Input
	InpName	LLEGL
End

Input
	InpName	LLEGR
End

Input
	InpName	wepR
End

Input
	InpName	wepL
End

Input
	InpName	HIPS
End

Input
	InpName	CHEST
End

Input
	InpName	Hair
End

Input
	InpName	UARMR
End

Input
	InpName	UARML
End

Input
	InpName	LARML
End

Input
	InpName	LARMR
End

Input
	InpName	Root
End

##################################

Condition
	On	Time
	Time	0

	Event
		EName	Ring
		Type	Local
		At	Root
		part1	Powers/Ability/StaminaRingUp.part
		Sound heal1 70 70 .8

	End

End

Condition
	On	Time
	Time	5

	Event
		EName	Ring
		Type	Local
		At	Root
		part1	Powers/Ability/StaminaRingUp.part

	End

End

Condition
	On	Time
	Time	15

	Event
		EName	Ring
		Type	Local
		At	Root
		part1	Powers/Ability/StaminaRingUp.part

	End

End

Condition
	On	Time
	Time	0

	Event
		EName	1
		Type	Local
		At	Hips
		part1	Powers/Healing/Healing01.part
		bhvr	Behaviors/GenericParticleFade.bhvr
	End

	Event
		EName	2
		Type	Local
		At	FootR
		part1	Powers/Healing/HealingBodyGlows.part
		part2	Powers/Healing/HealingBodyGlowsFlat.part
	End


#LegL



	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	Powers/Healing/HealingMotionGlows.part
		PMagnet Knee1

	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	Powers/Healing/HealingMotionGlows.part
		PMagnet FootL
	End

	Event
		EName	5
		Type	Local
		At	FootL
		part1	Powers/Healing/HealingBodyGlows.part
		part2	Powers/Healing/HealingBodyGlowsFlat.part
	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	Powers/Healing/HealingMotionGlows.part
		PMagnet Kneer

	End


#LegR


	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	Powers/Healing/HealingMotionGlows.part
		PMagnet FootR
	End

###############################################################################

	Event
		EName	9
		Type	Local
		At	Hair
		part2	Powers/Healing/HealingHeadGlows.part
	End

##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	Powers/Healing/HealingArmMotionGlows.part
		part2	Powers/Healing/HealingArmMotionGlows2.part
		PMagnet ElbowL

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	Powers/Healing/HealingArmMotionGlows.part
		part2	Powers/Healing/HealingArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		EName	12
		Type	Local
		At	WepL
		part1	Powers/Healing/HealingArmGlows.part

	End






	Event
		EName	13
		Type	Local
		At	UArmR
		part1	Powers/Healing/HealingArmMotionGlows.part
		part2	Powers/Healing/HealingArmMotionGlows2.part
		PMagnet ElbowL

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	Powers/Healing/HealingArmMotionGlows.part
		part2	Powers/Healing/HealingArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		EName	15
		Type	Local
		At	WepR
		part1	Powers/Healing/HealingArmGlows.part

	End



End



Condition
	On	Time
	Time	98

	Event
		EName	1
		Type	Destroy

	End

	Event
		EName	2
		Type	Destroy

	End

	Event
		EName	3
		Type	Destroy

	End

	Event
		EName	4
		Type	Destroy

	End

	Event
		EName	5
		Type	Destroy

	End

	Event
		EName	6
		Type	Destroy

	End

	Event
		EName	7
		Type	Destroy

	End

	Event
		EName	9
		Type	Destroy

	End

	Event
		EName	10
		Type	Destroy

	End

	Event
		EName	11
		Type	Destroy

	End

	Event
		EName	12
		Type	Destroy

	End

	Event
		EName	13
		Type	Destroy

	End

	Event
		EName	14
		Type	Destroy

	End

	Event
		EName	15
		Type	Destroy

	End

End



End