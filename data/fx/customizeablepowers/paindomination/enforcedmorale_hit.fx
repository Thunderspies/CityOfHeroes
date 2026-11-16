#############################################################
## EnforcedMorale_Hit.fx
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
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\PainDomination\StrengthRingUp.part
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
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\PainDomination\MindWallMotionGlows.part
		PMagnet Kneer
	End

	Event
		Type	Local
		At	LLEGR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\PainDomination\MindWallArmMotionGlows.part
		part2	CustomizeablePowers\PainDomination\MindWallMotionGlows.part
		PMagnet FootR
	End


	Event
		Type	Local
		At	ULEGL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\PainDomination\MindWallMotionGlows.part
		PMagnet Knee1
	End

	Event
		Type	Local
		At	LLEGL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\PainDomination\MindWallArmMotionGlows.part
		part2	CustomizeablePowers\PainDomination\MindWallMotionGlows.part
		PMagnet FootL
	End
End


Condition
	On	Time
	Time	20

	Event
		Type	Local
		At	Head
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\PainDomination\MindWallArmMotionGlows.part
		part2	CustomizeablePowers\PainDomination\MindWallHeadGlows.part
	End

	Event
		Type	Local
		At	UArmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\PainDomination\MindWallArmMotionGlows.part
		part2	CustomizeablePowers\PainDomination\MindWallArmMotionGlows2.part
		PMagnet ElbowL
	End

	Event
		Type	Local
		At	LArmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\PainDomination\MindWallArmMotionGlows.part
		part2	CustomizeablePowers\PainDomination\MindWallArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		Type	Local
		At	UArmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\PainDomination\MindWallArmMotionGlows.part
		part2	CustomizeablePowers\PainDomination\MindWallArmMotionGlows2.part
		PMagnet ElbowL
	End

	Event
		Type	Local
		At	LArmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\PainDomination\MindWallArmMotionGlows.part
		part2	CustomizeablePowers\PainDomination\MindWallArmMotionGlows2.part
		PMagnet HandL
	End
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