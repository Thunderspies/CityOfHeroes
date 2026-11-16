#############################################################
## HealingDrainBall.fx
#############################################################

FxInfo

LifeSpan 65

#############################################################

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
		part1	CustomizeablePowers\Empathy\Healing01a.part
		sound 	suckup2 80 30 .8
	End

	Event
		Type	Local
		At	ULEGL
		part1	CustomizeablePowers\Empathy\HealingMotionGlows.part
		PMagnet Knee1
	End

	Event
		Type	Local
		At	LLEGL
		part1	CustomizeablePowers\Empathy\HealingMotionGlows.part
		PMagnet FootL
	End

	Event
		Type	Local
		At	FootL
		part1	CustomizeablePowers\Empathy\HealingBodyGlows.part
	End

	Event
		Type	Local
		At	ULEGR
		part1	CustomizeablePowers\Empathy\HealingMotionGlows.part
		PMagnet Kneer
	End

	Event
		Type	Local
		At	LLEGR
		part1	CustomizeablePowers\Empathy\HealingMotionGlows.part
		PMagnet FootR
	End

	Event
		Type	Local
		At	FootR
		part1	CustomizeablePowers\Empathy\HealingBodyGlows.part
	End
End

Condition
	On	Time
	Time	20

	Event
		Type	Local
		At	Hair
		part2	CustomizeablePowers\Empathy\HealingHeadGlows.part
	End

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	CustomizeablePowers\Empathy\HealingArmMotionGlows.part
		part2	CustomizeablePowers\Empathy\HealingArmMotionGlows2.part
		PMagnet ElbowL
	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	CustomizeablePowers\Empathy\HealingArmMotionGlows.part
		part2	CustomizeablePowers\Empathy\HealingArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		EName	12
		Type	Local
		At	WepL
		part1	CustomizeablePowers\Empathy\HealingArmGlows.part
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part1	CustomizeablePowers\Empathy\HealingArmMotionGlows.part
		part2	CustomizeablePowers\Empathy\HealingArmMotionGlows2.part
		PMagnet ElbowL
	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	CustomizeablePowers\Empathy\HealingArmMotionGlows.part
		part2	CustomizeablePowers\Empathy\HealingArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		EName	15
		Type	Local
		At	WepR
		part1	CustomizeablePowers\Empathy\HealingArmGlows.part
	End
End

#############################################################

End