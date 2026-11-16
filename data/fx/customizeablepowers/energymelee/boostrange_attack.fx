#############################################################
## BoostRange_Attack.fx
#############################################################

FxInfo

LifeSpan 45

#############################################################

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	CustomizeablePowers\EnergyMelee\LightRaysThin.part
		part	CustomizeablePowers\EnergyMelee\HalfEnergyRing.part
		part	CustomizeablePowers\EnergyMelee\HalfEnergyRing2.part
		part	CustomizeablePowers\EnergyMelee\HalfEnergyRingUp.part
		Sound aim5 60 60 0.6
		Lifespan 20
	End
End

#############################################################

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	FootR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\EnergyMelee\RangeBoostBodyGlows.part
	End

	Event
		Type	Local
		At	ULEGL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\EnergyMelee\RangeBoostMotionGlows.part
		PMagnet Knee1
	End

	Event
		Type	Local
		At	LLEGL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\EnergyMelee\RangeBoostArmMotionGlows.part
		part2	CustomizeablePowers\EnergyMelee\RangeBoostMotionGlows.part
		PMagnet FootL
	End

	Event
		Type	Local
		At	FootL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\EnergyMelee\RangeBoostBodyGlows.part
	End

	Event
		Type	Local
		At	ULEGR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\EnergyMelee\RangeBoostMotionGlows.part
		PMagnet Kneer
	End

	Event
		Type	Local
		At	LLEGR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\EnergyMelee\RangeBoostArmMotionGlows.part
		part2	CustomizeablePowers\EnergyMelee\RangeBoostMotionGlows.part
		PMagnet FootR
	End

	Event
		Type	Local
		At	Head
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\EnergyMelee\RangeBoostArmMotionGlows.part
		part2	CustomizeablePowers\EnergyMelee\RangeBoostBodyGlows.part
	End

	Event
		Type	Local
		At	UArmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\EnergyMelee\RangeBoostArmMotionGlows.part
		part2	CustomizeablePowers\EnergyMelee\RangeBoostArmMotionGlows2.part
		PMagnet ElbowL
	End

	Event
		Type	Local
		At	LArmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\EnergyMelee\RangeBoostArmMotionGlows.part
		part2	CustomizeablePowers\EnergyMelee\RangeBoostArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		Type	Local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\EnergyMelee\RangeBoostArmGlows.part
	End

	Event
		Type	Local
		At	UArmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\EnergyMelee\RangeBoostArmMotionGlows.part
		part2	CustomizeablePowers\EnergyMelee\RangeBoostArmMotionGlows2.part
		PMagnet ElbowL
	End

	Event
		Type	Local
		At	LArmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\EnergyMelee\RangeBoostArmMotionGlows.part
		part2	CustomizeablePowers\EnergyMelee\RangeBoostArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		Type	Local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\EnergyMelee\RangeBoostArmGlows.part
	End
End

#############################################################

End