#############################################################
## AOE_Heal_Hit.fx
#############################################################

FxInfo
LifeSpan	100

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
		part1	CustomizeablePowers\Empathy\StaminaRingUp.part
		#Sound	heal1 70 25 .8
	End
End

Condition
	On	Time
	Time	20

	Event
		EName	Ring
		Type	Local
		At	Root
		part1	CustomizeablePowers\Empathy\StaminaRingUp.part
	End
End

Condition
	On	Time
	Time	35

	Event
		EName	Ring
		Type	Local
		At	Root
		part1	CustomizeablePowers\Empathy\StaminaRingUp.part
	End
End

## BODY GLOW ###########################################################

Condition
	On	Time
	Time	18

	Event
		Type	Local
		At	Hair
		part2	CustomizeablePowers\Empathy\HealingHeadGlows.part
	End

	Event
		Type	Local
		At	UArmL
		part1	CustomizeablePowers\Empathy\HealingArmMotionGlows.part
		part2	CustomizeablePowers\Empathy\HealingArmMotionGlows2.part
		PMagnet LArmL
	End

	Event
		Type	Local
		At	LArmL
		part1	CustomizeablePowers\Empathy\HealingArmMotionGlows.part
		part2	CustomizeablePowers\Empathy\HealingArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		Type	Local
		At	WepL
		part1	CustomizeablePowers\Empathy\HealingArmGlows.part
	End

	Event
		Type	Local
		At	UArmR
		part1	CustomizeablePowers\Empathy\HealingArmMotionGlows.part
		part2	CustomizeablePowers\Empathy\HealingArmMotionGlows2.part
		PMagnet LArmR
	End

	Event
		Type	Local
		At	LArmR
		part1	CustomizeablePowers\Empathy\HealingArmMotionGlows.part
		part2	CustomizeablePowers\Empathy\HealingArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		Type	Local
		At	WepR
		part1	CustomizeablePowers\Empathy\HealingArmGlows.part
	End

	Event
		Type	Local
		At	Hips
		part1	CustomizeablePowers\Empathy\Healing01.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
	End

	Event
		Type	Local
		At	ULEGR
		part1	CustomizeablePowers\Empathy\HealingMotionGlows.part
		PMagnet LLEGR
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
		part2	CustomizeablePowers\Empathy\HealingBodyGlowsFlat.part
	End

	Event
		Type	Local
		At	LLEGL
		part1	CustomizeablePowers\Empathy\HealingMotionGlows.part
		PMagnet FootL
	End

	Event
		Type	Local
		At	ULEGL
		part1	CustomizeablePowers\Empathy\HealingMotionGlows.part
		PMagnet FootL
	End

	Event
		Type	Local
		At	FootL
		part1	CustomizeablePowers\Empathy\HealingBodyGlows.part
		part2	CustomizeablePowers\Empathy\HealingBodyGlowsFlat.part
	End
End

#############################################################

Condition
	On	Time
	Time	58

	Event
		EName	all
		Type	Destroy

	End
End

#############################################################

End