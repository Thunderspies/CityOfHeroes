#############################################################
## HealingDrainBall.fx
#############################################################

FxInfo

LifeSpan 65

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On	Time
	Time	20

	Event
		Type	Local
		At	Root
		ChildFx POWERS\Ability\StaminaOtherRings.fx
	End

	Event
		Type	Local
		At	Hips
		part1	POWERS\Healing\Healing01a.part
		sound 	suckup2 80 30 .8
	End

	Event
		Type	Local
		At	ULEGL
		part1	POWERS\Healing\HealingMotionGlows.part
		PMagnet Knee1
	End

	Event
		Type	Local
		At	LLEGL
		part1	POWERS\Healing\HealingMotionGlows.part
		PMagnet FootL
	End

	Event
		Type	Local
		At	FootL
		part1	POWERS\Healing\HealingBodyGlows.part
	End

	Event
		Type	Local
		At	ULEGR
		part1	POWERS\Healing\HealingMotionGlows.part
		PMagnet Kneer
	End

	Event
		Type	Local
		At	LLEGR
		part1	POWERS\Healing\HealingMotionGlows.part
		PMagnet FootR
	End

	Event
		Type	Local
		At	FootR
		part1	POWERS\Healing\HealingBodyGlows.part
	End
End

Condition
	On	Time
	Time	20

	Event
		Type	Local
		At	Hair
		part2	POWERS\Healing\HealingHeadGlows.part
	End

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	POWERS\Healing\HealingArmMotionGlows.part
		part2	POWERS\Healing\HealingArmMotionGlows2.part
		PMagnet ElbowL
	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	POWERS\Healing\HealingArmMotionGlows.part
		part2	POWERS\Healing\HealingArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		EName	12
		Type	Local
		At	WepL
		part1	POWERS\Healing\HealingArmGlows.part
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part1	POWERS\Healing\HealingArmMotionGlows.part
		part2	POWERS\Healing\HealingArmMotionGlows2.part
		PMagnet ElbowL
	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	POWERS\Healing\HealingArmMotionGlows.part
		part2	POWERS\Healing\HealingArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		EName	15
		Type	Local
		At	WepR
		part1	POWERS\Healing\HealingArmGlows.part
	End
End

#############################################################

End