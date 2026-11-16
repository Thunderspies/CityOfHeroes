#############################################################
## FX System Name
#############################################################

FxInfo

LifeSpan 58

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
		part1	CustomizeablePowers\PainDomination\StaminaRingUp.part
		Sound heal1 70 70 0.5
	End
End

Condition
	On	Time
	Time	20

	Event
		EName	Ring
		Type	Local
		At	Root
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\PainDomination\StaminaRingUp.part
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
		part1	CustomizeablePowers\PainDomination\StaminaRingUp.part
	End
End

## LARGE GLOW ###########################################################

Condition
	On	Time
	Time	18

	Event
		Type	Local
		At	Hips
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\PainDomination\Healing01.part
	End
End

## BODY GLOW ###########################################################

Condition
	On	Time
	Time	18

	Event
		Type	Local
		At	Hair
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part2	CustomizeablePowers\PainDomination\HealingHeadGlows.part
	End

	Event
		Type	Local
		At	UArmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\PainDomination\HealingArmMotionGlows.part
		part2	CustomizeablePowers\PainDomination\HealingArmMotionGlows2.part
		PMagnet LArmL
	End

	Event
		Type	Local
		At	LArmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\PainDomination\HealingArmMotionGlows.part
		part2	CustomizeablePowers\PainDomination\HealingArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		Type	Local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\PainDomination\HealingArmGlows.part
	End

	Event
		Type	Local
		At	UArmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\PainDomination\HealingArmMotionGlows.part
		part2	CustomizeablePowers\PainDomination\HealingArmMotionGlows2.part
		PMagnet LArmR
	End

	Event
		Type	Local
		At	LArmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\PainDomination\HealingArmMotionGlows.part
		part2	CustomizeablePowers\PainDomination\HealingArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		Type	Local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\PainDomination\HealingArmGlows.part
	End

	Event
		Type	Local
		At	ULEGR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\PainDomination\HealingMotionGlows.part
		PMagnet LLEGR
	End

	Event
		Type	Local
		At	LLEGR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\PainDomination\HealingMotionGlows.part
		PMagnet FootR
	End

	Event
		Type	Local
		At	FootR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\PainDomination\HealingBodyGlows.part
	End

	Event
		Type	Local
		At	LLEGL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\PainDomination\HealingMotionGlows.part
		PMagnet FootL
	End

	Event
		Type	Local
		At	ULEGL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\PainDomination\HealingMotionGlows.part
		PMagnet FootL
	End

	Event
		Type	Local
		At	FootL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\PainDomination\HealingBodyGlows.part
	End
End

#############################################################

End