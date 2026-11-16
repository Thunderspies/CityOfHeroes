#############################################################
## Endurance.fx
#############################################################

FxInfo

LifeSpan 60

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
		#part1	Powers\Ability\IntelligenceRingUp.part
		sound	heal1 80 20 .4
	End
End

Condition
	On	Time
	Time	20

	Event
		Type	Local
		At	Root
		Bhvr	behaviors\GenericParticleFade.bhvr
		part1	:StrengthRingUp.part
		Lifespan 20
	End

#	Event
#		EName	Ring
#		Type	Local
#		At	Root
#		part1	Powers\Ability\IntelligenceRingUp.part
#	End
End

Condition
	On	Time
	Time	35

	Event
		Type	Local
		At	Root
		Bhvr	behaviors\GenericParticleFade.bhvr
		part1	:StrengthRingUp.part
		Lifespan 20
	End

#	Event
#		EName	Ring
#		Type	Local
#		At	Root
#		part1	Powers\Ability\IntelligenceRingUp.part
#	End
End

#############################################################

Condition
	On	Time
	Time	18

	Event
		Type	Local
		At	Hips
		bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:Healing01.part
		Lifespan 40
	End
End

#############################################################

Condition
	On	Time
	Time	18

	Event
		Type	Local
		At	Hair
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part2	:HealingHeadGlows.part
	End

	Event
		Type	Local
		At	UArmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:HealingArmMotionGlows.part
		part2	:HealingArmMotionGlows2.part
		PMagnet LArmL
	End

	Event
		Type	Local
		At	LArmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:HealingArmMotionGlows.part
		part2	:HealingArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		Type	Local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:HealingArmGlows.part
	End

	Event
		Type	Local
		At	UArmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:HealingArmMotionGlows.part
		part2	:HealingArmMotionGlows2.part
		PMagnet LArmR
	End

	Event
		Type	Local
		At	LArmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:HealingArmMotionGlows.part
		part2	:HealingArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		Type	Local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:HealingArmGlows.part
	End

	Event
		Type	Local
		At	ULEGR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:HealingMotionGlows.part
		PMagnet LLEGR
	End

	Event
		Type	Local
		At	LLEGR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:HealingMotionGlows.part
		PMagnet FootR
	End

	Event
		Type	Local
		At	FootR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:HealingBodyGlows.part
	End

	Event
		Type	Local
		At	LLEGL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:HealingMotionGlows.part
		PMagnet FootL
	End

	Event
		Type	Local
		At	ULEGL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:HealingMotionGlows.part
		PMagnet FootL
	End

	Event
		Type	Local
		At	FootL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:HealingBodyGlows.part
	End
End

#############################################################

End