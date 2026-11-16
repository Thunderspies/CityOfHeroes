#############################################################
## EmpathyCureWounds.fx
#############################################################

FxInfo

LifeSpan 65

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Left
		Type	Local
		At	WepL
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part1	:Stamina2.part
		Part2	:StaminaSmall2.part
		Part3	:AbilityDots.part
		Part4	:AbilitySparklingDots.part
		Lifespan 45
	End

	Event
		EName	Right
		Type	Local
		At	WepR
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part1	:Stamina2.part
		Part2	:StaminaSmall2.part
		Part3	:AbilityDots.part
		Part4	:AbilitySparklingDots.part
		Lifespan 45
	End
End

#############################################################

Condition
	On 	cycle
	Time 	20
	Chance	1

	Event
		EName	1
		Type	PositOnly
		At	Hips
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:CureWoundsAOERing.part
		part2	:CureWoundsAOERing2.part
		Sound regenaura 60 60 .3
		LifeSpan 20
	End
End

#############################################################

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	Hips
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:Healing01a.part
	End

#	Event
#		Type	Local
#		At	FootR
#		Bhvr	Behaviors\GenericParticleFade.bhvr
#		part1	:HealingBodyGlows.part
#	End
#
	Event
		Type	Local
		At	ULEGL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:HealingMotionGlows.part
		PMagnet Knee1
	End

	Event
		Type	Local
		At	LLEGL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:HealingMotionGlows.part
		PMagnet FootL
	End

#	Event
#		Type	Local
#		At	FootL
#		Bhvr	Behaviors\GenericParticleFade.bhvr
#		part1	:HealingBodyGlows.part
#	End

	Event
		Type	Local
		At	ULEGR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:HealingMotionGlows.part
		PMagnet Kneer
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
		PMagnet ElbowL
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
		PMagnet ElbowL
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
End
###################Sound##########################################
Condition
	On	Time
	Time	15

	Event
		Type	Local
		At	Hips
		sound 	World_Of_Pain_01 50 30 .85
	End
#############################################################
End