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
	Time	15

	Event
		EName	Ring
		Type	Local
		At	Root
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:StaminaRingUp.part
		Sound heal1 70 70 .25
	End
End

Condition
	On	Time
	Time	20

	Event
		Type	Local
		At	Root
		part1	:StaminaRingUp.part
	End

	Event
		Type	Local
		At	Hips
		part1	:Healing01a.part
	End
End

Condition
	On	Time
	Time	35

	Event
		EName	Ring
		Type	Local
		At	Root
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:StaminaRingUp.part
	End
End

#############################################################

Condition
	On	Time
	Time	20

	Event
		Type	Local
		At	ULEGL
		part1	:HealingMotionGlows.part
		PMagnet Knee1
	End

	Event
		Type	Local
		At	LLEGL
		part1	:HealingMotionGlows.part
		PMagnet FootL
	End

	Event
		Type	Local
		At	FootL
		part1	:HealingBodyGlows.part
	End

	Event
		Type	Local
		At	ULEGR
		part1	:HealingMotionGlows.part
		PMagnet Kneer
	End

	Event
		Type	Local
		At	LLEGR
		part1	:HealingMotionGlows.part
		PMagnet FootR
	End

	Event
		Type	Local
		At	FootR
		part1	:HealingBodyGlows.part
	End
End

Condition
	On	Time
	Time	20

	Event
		Type	Local
		At	Hair
		part2	:HealingHeadGlows.part
	End

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	:HealingArmMotionGlows.part
		part2	:HealingArmMotionGlows2.part
		PMagnet ElbowL
	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	:HealingArmMotionGlows.part
		part2	:HealingArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		EName	12
		Type	Local
		At	WepL
		part1	:HealingArmGlows.part
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part1	:HealingArmMotionGlows.part
		part2	:HealingArmMotionGlows2.part
		PMagnet ElbowL
	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	:HealingArmMotionGlows.part
		part2	:HealingArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		EName	15
		Type	Local
		At	WepR
		part1	:HealingArmGlows.part
	End
End

#############################################################

End