#############################################################
## Particle System
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	15

	Event
		EName	2
		Type	Local
		At	Chest
		Bhvr	Behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Sound forcefield5_loop 60 60 0.66
		Lifespan 180
	End
End

Condition
	On	Time
	Time	15


	Event
		Type	Local
		At	Waist
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part2	CustomizeablePowers\SuperStrength\Rage_BigFlare1.part
	End
End

Condition
	On	Time
	Time	15

	Event
		Type	Local
		At	Hair
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part2	CustomizeablePowers\SuperStrength\StrengthHeadGlows.part
		PMagnet Neck
		Sound rage 100 100 1.0
	End

	Event
		Type	Local
		At	UArmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\SuperStrength\Rage_ArmMotionGlows.part
		PMagnet LArmL
	End

	Event
		Type	Local
		At	LArmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\SuperStrength\Rage_ArmMotionGlows.part
		PMagnet WepL
	End

	Event
		Type	Local
		At	UArmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\SuperStrength\Rage_ArmMotionGlows.part
		PMagnet LArmR
	End

	Event
		Type	Local
		At	LArmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\SuperStrength\Rage_ArmMotionGlows.part
		PMagnet WepR
	End

	Event
		Type	Local
		At	ULEGR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\SuperStrength\Rage_MotionGlows.part
		PMagnet LlegR
	End

	Event
		Type	Local
		At	LLEGR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\SuperStrength\Rage_MotionGlows.part
		PMagnet FootR
	End

	Event
		Type	Local
		At	ULEGL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\SuperStrength\Rage_MotionGlows.part
		PMagnet LLEGL
	End

	Event
		Type	Local
		At	LLEGL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\SuperStrength\Rage_MotionGlows.part
		PMagnet FootL
	End
End

#############################################################

End