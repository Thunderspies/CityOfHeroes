#############################################################
## FX System Name
#############################################################

FxInfo

LifeSpan	60

#############################################################

#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		EName 	Primea
#		Type	Local 
#		At	Eyes
#		BHVR	Behaviors/OffsetEyes.bhvr
#		Geom	CircleOfThorns
#		sound	peaceeyes1 60 50 .7
#		LifeSpan	70
#	End
#
#	Event
#		Type	Local 
#		At	Primea
#		 AltPiv 1
#		BHVR	Behaviors/GenericparticleFade.bhvr
#		Part	POWERS\CosmicPowers\PeaceBringer\ActivationEyes\CircleOfThornsEyes_21Green.part
#		Part	POWERS\CosmicPowers\PeaceBringer\ActivationEyes\EyesMistLeftBLUE.part
#	
#		LifeSpan	70
#	End
#	
#	Event
#		Type	Local 
#		At	Primea
#		 AltPiv 2
#		BHVR	Behaviors/GenericparticleFade.bhvr
#		Part	POWERS\CosmicPowers\PeaceBringer\ActivationEyes\CircleOfThornsEyes_21Green.part
#		Part	POWERS\CosmicPowers\PeaceBringer\ActivationEyes\EyesMistRightBLUE.part
#		LifeSpan	70
#	End
#End

#############################################################

Condition
	On	Time
	Time	15

	Event
		EName	1
		Type	Local
		At	Hips
		part1	:Healing01.part
		bhvr	Behaviors/GenericParticleFade.bhvr
	End
End

Condition
	On	Time
	Time	20

	Event
		EName	1
		Type	Local
		At	Hips
		part1	:Healing01.part
		bhvr	Behaviors/GenericParticleFade.bhvr
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
		bhvr	Behaviors/GenericParticleFade.bhvr
		Sound heal1 70 70 .95
	End
End

Condition
	On	Time
	Time	20
	
	Event
		EName	Ring
		Type	Local
		At	Root
		part1	Powers/Ability/StaminaRingUp.part
		bhvr	Behaviors/GenericParticleFade.bhvr
	End
End

Condition
	On	Time
	Time	35
	
	Event
		EName	Ring
		Type	Local
		At	Root
		part1	Powers/Ability/StaminaRingUp.part
		bhvr	Behaviors/GenericParticleFade.bhvr
	End
End

#############################################################

Condition
	On	Time
	Time	18
	
	Event
		EName	2
		Type	Local
		At	FootR
		part1	:HealingBodyGlows.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan	40
	End

	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	:HealingMotionGlows.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet Knee1
		Lifespan	40
	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	:HealingMotionGlows.part
		part1	:HealingBodyGlowsDown.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet FootL
		Lifespan	40
	End

	Event
		EName	5		
		Type	Local
		At	FootL
		part1	:HealingBodyGlows.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan	40
	End

	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	:HealingMotionGlows.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet Kneer
		Lifespan	40
	End

	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	:HealingMotionGlows.part
		part1	:HealingBodyGlowsDown.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet FootR
		Lifespan	40
	End

	Event
		EName	9
		Type	Local
		At	Head
		part2	:HealingHeadGlows.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan	40
	End

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	:HealingArmMotionGlows.part
		part2	:HealingArmMotionGlows2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet ElbowL
		Lifespan	40
	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	:HealingArmMotionGlows.part
		part2	:HealingArmMotionGlows2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet HandL
		Lifespan	40
	End

	Event
		EName	12
		Type	Local
		At	WepL
		part1	:HealingArmGlows.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan	40
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part1	:HealingArmMotionGlows.part
		part2	:HealingArmMotionGlows2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet ElbowL
		Lifespan	40
	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	:HealingArmMotionGlows.part
		part2	:HealingArmMotionGlows2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet HandL
		Lifespan	40
	End

	Event
		EName	15
		Type	Local
		At	WepR
		part1	:HealingArmGlows.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan	40
	End
End

#############################################################

End				