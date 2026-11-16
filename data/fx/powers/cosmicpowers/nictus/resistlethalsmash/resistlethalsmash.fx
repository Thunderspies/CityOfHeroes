#########################################################
## Umbral Aura - Penumbral Shield
#########################################################

FxInfo

## SOUND FX ######################################################

Condition
	On 	Time
	Time 	0
	Event

		Type	Local
		At	Origin
		Sound WarshadeBuild 70 70 .76
	End
End

Condition
	On 	Time
	Time 	12
	Event

		Type	Local
		At	Origin
		Sound glow13_loop 60 60 .2
	End
End

## EYE FX ######################################################

#Condition
#	On	Time
#	Time	0
#
#	Event
#		EName 	Eyes
#		Type	Local 
#		At	Root
#		ChildFX POWERS\CosmicPowers\Nictus\ActivationEyes\ActivationEyesContinuingFX.fx
#	End
#End

## RICOCHET FX ######################################################

##Condition
##	On 	cycle
##	Time 	20
##	Chance	0.2
##	
##	Event
##		EName	3
##		Type	Local
##		At	LarmL
##		part1	:2RaysSmall.part
##		part2	:2Ring2.part
##		part3	:2Ring.part
##
##		LifeSpan	25
##	End
##End
##
##Condition
##	On 	cycle
##	Time 	19
##	Chance	0.2
##	
##	Event
##		EName	3
##		Type	Local
##		At	LarmR
##		part1	:2RaysSmall.part
##		part2	:2Ring2.part
##		part3	:2Ring.part
##
##		LifeSpan	25
##	End
##	
##End
##
##Condition
##	On 	cycle
##	Time 	21
##	Chance	0.2
##	
##	Event
##		EName	3
##		Type	Local
##		At	UarmL
##		part1	:2RaysSmall.part
##		part2	:2Ring2.part
##		part3	:2Ring.part
##		
##		LifeSpan	25
##	End
##End
##
##Condition
##	On 	cycle
##	Time 	22
##	Chance	0.2
##	
##	Event
##		EName	3
##		Type	Local
##		At	UarmR
##		part1	:2RaysSmall.part
##		part2	:2Ring2.part
##		part3	:2Ring.part
##		
##		LifeSpan	25
##	End
##End
##
##Condition
##	On 	cycle
##	Time 	17
##	Chance	0.2
##
##	Event
##		EName	3
##		Type	Local
##		At	LlegL
##		part1	:2RaysSmall.part
##		part2	:2Ring2.part
##		part3	:2Ring.part
##		
##		LifeSpan	25
##	End
##End
##
##Condition
##	On 	cycle
##	Time 	18
##	Chance	0.2
##
##	Event
##		EName	3
##		Type	Local
##		At	LlegR
##		part1	:2RaysSmall.part
##		part2	:2Ring2.part
##		part3	:2Ring.part
##		
##		LifeSpan	25
##	End
##End

## AURA FX ######################################################

Condition
	On	Time
	Time	18

#	Event
#		EName	9
#		Type	Local
#		At	Head
#		part	:HeadGlows.part
#		part	:PurpleHeadGlows.part
#		part	:HeadGlowsBLK.part
#		bhvr	Behaviors/GenericParticleFade.bhvr
#	End
#
	Event
		EName	10
		Type	Local
		At	UArmL
		##part	:ArmMotionGlows.part
		part	:ArmMotionGlows2.part
		##part	:PurpleArmMotionGlows.part
		part	:PurpleArmMotionGlows2.part
		##part	:ArmMotionGlowsBLK.part
		part	:ArmMotionGlows2BLK.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet ElbowL
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		##part	:ArmMotionGlows.part
		part	:ArmMotionGlows2.part
		##part	:PurpleArmMotionGlows.part
		part	:PurpleArmMotionGlows2.part
		##part	:ArmMotionGlowsBLK.part
		part	:ArmMotionGlows2BLK.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet ElbowL
	End

	Event
		EName	11
		Type	Local
		At	LArmL
		##part	:ArmMotionGlows.part
		part	:ArmMotionGlows2.part
		##part	:PurpleArmMotionGlows.part
		part	:PurpleArmMotionGlows2.part
		##part	:ArmMotionGlowsBLK.part
		part	:ArmMotionGlows2BLK.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet WepL
	End

	Event
		EName	14
		Type	Local
		At	LArmR
		##part	:ArmMotionGlows.part
		part	:ArmMotionGlows2.part
		##part	:PurpleArmMotionGlows.part
		part	:PurpleArmMotionGlows2.part
		##part	:ArmMotionGlowsBLK.part
		part	:ArmMotionGlows2BLK.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet WepR
	End

##	Event
##		EName	12
##		Type	Local
##		At	WepL
##		part	:ArmGlows.part
##		part	:PurpleArmGlows.part
##		part	:ArmGlowsBLK.part
##		
##		bhvr	Behaviors/GenericParticleFade.bhvr
##	End
##
##	Event
##		EName	15
##		Type	Local
##		At	WepR
##		part	:ArmGlows.part
##		part	:PurpleArmGlows.part
##		part	:ArmGlowsBLK.part
##		bhvr	Behaviors/GenericParticleFade.bhvr
##	End

	Event
		EName	3
		Type	Local
		At	Hips
		part	:MotionGlows.part
		part	:BodyGlowsDown.part
		part	:PurpleMotionGlows.part
		part	:PurpleBodyGlowsDown.part
		part	:MotionGlowsBLK.part
		part	:BodyGlowsDownBLK.part
		PMagnet Waist
	End

##	Event
##		EName	3
##		Type	Local
##		At	ULEGL
##		part	:MotionGlows.part
##		part	:BodyGlowsDown.part
##		part	:PurpleMotionGlows.part
##		part	:PurpleBodyGlowsDown.part
##		part	:MotionGlowsBLK.part
##		part	:BodyGlowsDownBLK.part
##		bhvr	Behaviors/GenericParticleFade.bhvr
##		PMagnet KneeL
##	End
##
##	Event
##		EName	6
##		Type	Local
##		At	ULEGR
##		part	:MotionGlows.part
##		part	:BodyGlowsDown.part
##		part	:PurpleMotionGlows.part
##		part	:PurpleBodyGlowsDown.part
##		part	:MotionGlowsBLK.part
##		part	:BodyGlowsDownBLK.part
##		bhvr	Behaviors/GenericParticleFade.bhvr
##		PMagnet KneeR
##	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part	:MotionGlows.part
		part	:BodyGlowsDown.part
		part	:PurpleMotionGlows.part
		part	:PurpleBodyGlowsDown.part
		part	:MotionGlowsBLK.part
		part	:BodyGlowsDownBLK.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet ULEGL
	End

	Event
		EName	7
		Type	Local
		At	LLEGR
		part	:MotionGlows.part
		part	:BodyGlowsDown.part
		part	:PurpleMotionGlows.part
		part	:PurpleBodyGlowsDown.part
		part	:MotionGlowsBLK.part
		part	:BodyGlowsDownBLK.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		PMagnet ULEGR
	End

	Event
		EName	5		
		Type	Local
		At	FootL
		part	:BodyGlows.part
		part	:PurpleBodyGlows.part
		part	:BodyGlowsBLK.part
		bhvr	Behaviors/GenericParticleFade.bhvr
	End
	
	Event
		EName	2
		Type	Local
		At	FootR
		part	:BodyGlows.part
		part	:PurpleBodyGlows.part
		part	:BodyGlowsBLK.part
		bhvr	Behaviors/GenericParticleFade.bhvr
	End
End

########################################################

End				