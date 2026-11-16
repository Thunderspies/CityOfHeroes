#############################################################
## FX System Name
#############################################################

FxInfo

LifeSpan	100

#############################################################

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
		part2	:HealingHeadGlows.part
	End

	Event
		Type	Local
		At	UArmL
		part1	:HealingArmMotionGlows.part
		part2	:HealingArmMotionGlows2.part
		PMagnet LArmL
	End

	Event
		Type	Local
		At	LArmL
		part1	:HealingArmMotionGlows.part
		part2	:HealingArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		Type	Local
		At	WepL
		part1	:HealingArmGlows.part
	End

	Event
		Type	Local
		At	UArmR
		part1	:HealingArmMotionGlows.part
		part2	:HealingArmMotionGlows2.part
		PMagnet LArmR
	End

	Event
		Type	Local
		At	LArmR
		part1	:HealingArmMotionGlows.part
		part2	:HealingArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		Type	Local
		At	WepR
		part1	:HealingArmGlows.part
	End

	Event
		Type	Local
		At	Hips
		part1	:Healing01.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
	End

	Event
		Type	Local
		At	ULEGR
		part1	:HealingMotionGlows.part
		PMagnet LLEGR
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
		part2	:HealingBodyGlowsFlat.part
	End

	Event
		Type	Local
		At	LLEGL
		part1	:HealingMotionGlows.part
		PMagnet FootL
	End

	Event
		Type	Local
		At	ULEGL
		part1	:HealingMotionGlows.part
		PMagnet FootL
	End

	Event
		Type	Local
		At	FootL
		part1	:HealingBodyGlows.part
		part2	:HealingBodyGlowsFlat.part
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