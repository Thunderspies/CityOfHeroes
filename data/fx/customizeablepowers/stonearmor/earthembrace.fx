#########################################################
## Earth's Embrace
#########################################################

FxInfo
LifeSpan	105

##################################

Condition
	On	Time
	Time 	0

	Event
		Type	Local
		At 	origin
		Sound Stone4 100 100 1.0
	End
End

##################################################################
## Ground FX
##################################################################

Condition
	On	TriggerBits
	TriggerBits	concrete
	TriggerBits	grass		#needs grass clipping fx#
	TriggerBits	gravel		#needs gravel debris fx#
	TriggerBits	Dirt
	#TriggerBits	Wood
	#TriggerBits	Carpet
	#TriggerBits	metal

	Event
		EName 	GroundFX
		Type	Start
		At	root

		ChildFX	:EarthEmbrace_GroundFX.fx
	End
End

#########################################################################
## Heal\Stamina Buff
#########################################################################

Condition
	On	Time
	Time	15

	Event
		EName	Ring
		Type	Local
		At	Root
		part1	CustomizeablePowers\Empathy\StaminaRingUp.part

		Sound heal1 70 70 .8
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

Condition
	On	Time
	Time	28

	Event
		EName	1
		Type	Local
		At	Hips
		BHVR	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\Empathy\Healing01.part
	End
End

Condition
	On	Time
	Time	18

	Event
		EName	2
		Type	Local
		At	FootR
		part1	CustomizeablePowers\Empathy\HealingBodyGlows.part
		part2	CustomizeablePowers\Empathy\HealingBodyGlowsFlat.part
	End

	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	CustomizeablePowers\Empathy\HealingMotionGlows.part
		PMagnet LLEGL

	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	CustomizeablePowers\Empathy\HealingMotionGlows.part
		PMagnet FootL
	End

	Event
		EName	5
		Type	Local
		At	FootL
		part1	CustomizeablePowers\Empathy\HealingBodyGlows.part
		part2	CustomizeablePowers\Empathy\HealingBodyGlowsFlat.part
	End

	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	CustomizeablePowers\Empathy\HealingMotionGlows.part
		PMagnet LLEGR

	End

	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	CustomizeablePowers\Empathy\HealingMotionGlows.part
		PMagnet FootR
	End

	Event
		EName	9
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

#########################################################################
## Destroy!
#########################################################################

End