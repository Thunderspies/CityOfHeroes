#############################################################
## MindWall.fx
#############################################################

FxInfo

LifeSpan 120

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On	Time
	Time	25

	Event
		EName	Ring
		Type	Local
		At	Root
		Bhvr	behaviors\GenericParticleFade.bhvr
		part1	:StrengthRingUp.part
		Lifespan 20
	End
End

#############################################################

Condition
	On	Time
	Time	20

	Event
		Type	Local
		At	ULEGR
		Bhvr	behaviors\GenericParticleFade.bhvr
		part1	:MindWallMotionGlows.part
		PMagnet Kneer
	End

	Event
		Type	Local
		At	LLEGR
		Bhvr	behaviors\GenericParticleFade.bhvr
		part1	:MindWallArmMotionGlows.part
		part2	:MindWallMotionGlows.part
		PMagnet FootR
	End

#	Event
#		Type	Local
#		At	FootR
#		Bhvr	behaviors\GenericParticleFade.bhvr
#		part1	:MindWallBodyGlows.part
#	End

	Event
		Type	Local
		At	ULEGL
		Bhvr	behaviors\GenericParticleFade.bhvr
		part1	:MindWallMotionGlows.part
		PMagnet Knee1
	End

	Event
		Type	Local
		At	LLEGL
		Bhvr	behaviors\GenericParticleFade.bhvr
		part1	:MindWallArmMotionGlows.part
		part2	:MindWallMotionGlows.part
		PMagnet FootL
	End

#	Event
#		Type	Local
#		At	FootL
#		Bhvr	behaviors\GenericParticleFade.bhvr
#		part1	:MindWallBodyGlows.part
#	End
End

Condition
	On	Time
	Time	20

	Event
		Type	Local
		At	Head
		Bhvr	behaviors\GenericParticleFade.bhvr
		part1	:MindWallArmMotionGlows.part
		part2	:MindWallHeadGlows.part
	End

	Event
		Type	Local
		At	UArmL
		Bhvr	behaviors\GenericParticleFade.bhvr
		part1	:MindWallArmMotionGlows.part
		part2	:MindWallArmMotionGlows2.part
		PMagnet ElbowL
	End

	Event
		Type	Local
		At	LArmL
		Bhvr	behaviors\GenericParticleFade.bhvr
		part1	:MindWallArmMotionGlows.part
		part2	:MindWallArmMotionGlows2.part
		PMagnet HandL
	End

#	Event
#		Type	Local
#		At	WepL
#		Bhvr	behaviors\GenericParticleFade.bhvr
#		part1	:MindWallArmGlows.part
#	End

	Event
		Type	Local
		At	UArmR
		Bhvr	behaviors\GenericParticleFade.bhvr
		part1	:MindWallArmMotionGlows.part
		part2	:MindWallArmMotionGlows2.part
		PMagnet ElbowL
	End

	Event
		Type	Local
		At	LArmR
		Bhvr	behaviors\GenericParticleFade.bhvr
		part1	:MindWallArmMotionGlows.part
		part2	:MindWallArmMotionGlows2.part
		PMagnet HandL
	End

#	Event
#		Type	Local
#		At	WepR
#		Bhvr	behaviors\GenericParticleFade.bhvr
#		part1	:MindWallArmGlows.part
#	End
End

#############################################################
##################Sound#####################################
Condition
	On	Time
	Time	20

	Event
		Type	Local
		At	Head
		Sound Enforced_Morale_Hit_01 50 50 .85
	End
#############################################################

End