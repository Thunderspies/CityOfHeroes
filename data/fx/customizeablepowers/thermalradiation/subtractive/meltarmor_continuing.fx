#########################################################
## Thermal Radiation - Melt Armor (Continuing FX)
#########################################################

FXInfo

ClampMaxScale 3.5 3.5 3.5

Lifespan 1200

## SOUND FX #######################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	WepR
		Sound firestreaks 70 70 1.0
	End
End

Condition
	On 	Time
	Time 	4

	Event
		Type	local
		At	WepR
		Sound melt3_loop 60.0 60.0 .22
	End
End

Condition
	On 	Time
	Time 	5

	Event
		Type	local
		At	WepR
		Sound Melt2_loop 60.0 60.0 .24
	End
End

## HIT #######################################################

Condition
	On	Time
	Time	0

	Event
		EName	Pivot
		Type	Local
		At	head
		part2	CustomizeablePowers\CorruptorBuff\ThermalRadiation\FireCorea.part
	End

	Event
		EName	Hips
		Type	Local
		At	Hips
		part3	CustomizeablePowers\CorruptorBuff\ThermalRadiation\FireCoreTrail1a.part
	End

	Event
		EName	Chest
		Type	Local
		At	back
		part2	CustomizeablePowers\CorruptorBuff\ThermalRadiation\FireCore4a.part
	End

	Event
		EName	Chest
		Type	Local
		At	Chest
		part3	CustomizeablePowers\CorruptorBuff\ThermalRadiation\FireCoreTrail2a.part
		Part4	CustomizeablePowers\CorruptorBuff\ThermalRadiation\FireCoreSparksa.part
	End

	Event
		EName	Lleg
		Type	Local
		At	LlegL
		part1	CustomizeablePowers\CorruptorBuff\ThermalRadiation\FireCore1a.part
		part2	CustomizeablePowers\CorruptorBuff\ThermalRadiation\FireCoreTraila.part
	End

	Event
		EName	Rleg
		Type	Local
		At	LlegR
		part1	CustomizeablePowers\CorruptorBuff\ThermalRadiation\FireCore1a.part
		part2	CustomizeablePowers\CorruptorBuff\ThermalRadiation\FireCoreTraila.part
	End

	Event
		EName	Lleg
		Type	Local
		At	FootL
		part1	CustomizeablePowers\CorruptorBuff\ThermalRadiation\FireCore1Feeta.part
		part2	CustomizeablePowers\CorruptorBuff\ThermalRadiation\FireCoreTrailFeeta.part
	End

	Event
		EName	Rleg
		Type	Local
		At	FootR
		part1	CustomizeablePowers\CorruptorBuff\ThermalRadiation\FireCore1Feeta.part
		part2	CustomizeablePowers\CorruptorBuff\ThermalRadiation\FireCoreTrailFeeta.part
	End

#	Event
#		Type	local
#		At	root
#		geom	HeatWaveSphereRed
#		bhvr	:Scale.bhvr
#	End

	Event
		Type	Local
		At	Root
		part	CustomizeablePowers\CorruptorBuff\ThermalRadiation\Steam.part
	End
End

## MOLTEN DRIPS #######################################################

Condition
	On	Cycle
	Time	10
	Chance	0.25

	Event
		Type	local
		At	neck
		BHVR	Behaviors\GenericParticleFade2.bhvr
		part	CustomizeablePowers\CorruptorBuff\ThermalRadiation\MetalDripWellupInvert.part
		part	CustomizeablePowers\CorruptorBuff\ThermalRadiation\MetalDripWellup_Invert.part
		part	CustomizeablePowers\CorruptorBuff\ThermalRadiation\MetalDripWellup.part
		LifeSpan	20
	End
End

Condition
	On	Cycle
	Time	10
	Chance	0.125
	Event
		Type	local
		At	neck
		BHVR	Behaviors\GenericParticleFade2.bhvr
		part	CustomizeablePowers\CorruptorBuff\ThermalRadiation\MetalDrips.part
		part	CustomizeablePowers\CorruptorBuff\ThermalRadiation\MeltingMetal.part
		LifeSpan	20
	End
End

Condition
	On	cycle
	Time	8
	Chance	0.125

	Event
		Type	local
		At	SpadL
		BHVR	Behaviors\GenericParticleFade2.bhvr
		part	CustomizeablePowers\CorruptorBuff\ThermalRadiation\MetalDripWellupInvert.part
		part	CustomizeablePowers\CorruptorBuff\ThermalRadiation\MetalDripWellup_Invert.part
		part	CustomizeablePowers\CorruptorBuff\ThermalRadiation\MetalDripWellup.part
		LifeSpan	20
	End
End

Condition
	On	cycle
	Time	7
	Chance	0.125

	Event
		Type	local
		At	SpadR
		BHVR	Behaviors\GenericParticleFade2.bhvr
		part	CustomizeablePowers\CorruptorBuff\ThermalRadiation\MetalDripWellupInvert.part
		part	CustomizeablePowers\CorruptorBuff\ThermalRadiation\MetalDripWellup_Invert.part
		part	CustomizeablePowers\CorruptorBuff\ThermalRadiation\MetalDripWellup.part
		LifeSpan	20
	End
End


Condition
	On	Cycle
	Time	7
	Chance	0.125

	Event
		Type	local
		At	UARML
		BHVR	Behaviors\GenericParticleFade2.bhvr
		part	CustomizeablePowers\CorruptorBuff\ThermalRadiation\MetalDripWellupInvert.part
		part	CustomizeablePowers\CorruptorBuff\ThermalRadiation\MetalDripWellup_Invert.part
		part	CustomizeablePowers\CorruptorBuff\ThermalRadiation\MetalDripWellup.part
		LifeSpan	20
	End
End

Condition
	On	Cycle
	Time	8
	Chance	0.125

	Event
		Type	local
		At	UARMR
		BHVR	Behaviors\GenericParticleFade2.bhvr
		part	CustomizeablePowers\CorruptorBuff\ThermalRadiation\MetalDripWellupInvert.part
		part	CustomizeablePowers\CorruptorBuff\ThermalRadiation\MetalDripWellup_Invert.part
		part	CustomizeablePowers\CorruptorBuff\ThermalRadiation\MetalDripWellup.part
		LifeSpan	20
	End
End

Condition
	On	Cycle
	Time	10
	Chance	0.125

	Event
		Type	local
		At	WepL
		BHVR	Behaviors\GenericParticleFade2.bhvr
		part	CustomizeablePowers\CorruptorBuff\ThermalRadiation\MetalDripWellupInvert.part
		part	CustomizeablePowers\CorruptorBuff\ThermalRadiation\MetalDripWellup_Invert.part
		part	CustomizeablePowers\CorruptorBuff\ThermalRadiation\MetalDripWellup.part
		LifeSpan	20
	End
End

Condition
	On	Cycle
	Time	9
	Chance	0.125

	Event
		Type	local
		At	WepR
		BHVR	Behaviors\GenericParticleFade2.bhvr
		part	CustomizeablePowers\CorruptorBuff\ThermalRadiation\MetalDripWellupInvert.part
		part	CustomizeablePowers\CorruptorBuff\ThermalRadiation\MetalDripWellup_Invert.part
		part	CustomizeablePowers\CorruptorBuff\ThermalRadiation\MetalDripWellup.part
		LifeSpan	20
	End
End

Condition
	On	Cycle
	Time	7
	Chance	0.125

	Event
		Type	local
		At	LLEGL
		BHVR	Behaviors\GenericParticleFade2.bhvr
		part	CustomizeablePowers\CorruptorBuff\ThermalRadiation\MetalDripWellupInvert.part
		part	CustomizeablePowers\CorruptorBuff\ThermalRadiation\MetalDripWellup_Invert.part
		part	CustomizeablePowers\CorruptorBuff\ThermalRadiation\MetalDripWellup.part
		LifeSpan	20
	End
End

Condition
	On	Cycle
	Time	8
	Chance	0.125

	Event
		Type	local
		At	LLEGR
		BHVR	Behaviors\GenericParticleFade2.bhvr
		part	CustomizeablePowers\CorruptorBuff\ThermalRadiation\MetalDripWellupInvert.part
		part	CustomizeablePowers\CorruptorBuff\ThermalRadiation\MetalDripWellup_Invert.part
		part	CustomizeablePowers\CorruptorBuff\ThermalRadiation\MetalDripWellup.part
		LifeSpan	20
	End
End

Condition
	On	cycle
	Time	9
	Chance	0.125

	Event
		Type	local
		At	ULEGL
		BHVR	Behaviors\GenericParticleFade2.bhvr
		part	CustomizeablePowers\CorruptorBuff\ThermalRadiation\MetalDripWellupInvert.part
		part	CustomizeablePowers\CorruptorBuff\ThermalRadiation\MetalDripWellup_Invert.part
		part	CustomizeablePowers\CorruptorBuff\ThermalRadiation\MetalDripWellup.part
		LifeSpan	20
	End
End

Condition
	On	cycle
	Time	8
	Chance	0.125

	Event
		Type	local
		At	ULEGR
		BHVR	Behaviors\GenericParticleFade2.bhvr
		part	CustomizeablePowers\CorruptorBuff\ThermalRadiation\MetalDripWellupInvert.part
		part	CustomizeablePowers\CorruptorBuff\ThermalRadiation\MetalDripWellup_Invert.part
		part	CustomizeablePowers\CorruptorBuff\ThermalRadiation\MetalDripWellup.part
		LifeSpan	20
	End
End

Condition
	On	cycle
	Time	10
	Chance	0.125

	Event
		Type	local
		At	LArmL
		BHVR	Behaviors\GenericParticleFade2.bhvr
		part	CustomizeablePowers\CorruptorBuff\ThermalRadiation\MetalDripWellupInvert.part
		part	CustomizeablePowers\CorruptorBuff\ThermalRadiation\MetalDripWellup_Invert.part
		part	CustomizeablePowers\CorruptorBuff\ThermalRadiation\MetalDripWellup.part
		LifeSpan	20
	End
End

Condition
	On	cycle
	Time	8
	Chance	0.125

	Event
		Type	local
		At	LArmR
		BHVR	Behaviors\GenericParticleFade2.bhvr
		part	CustomizeablePowers\CorruptorBuff\ThermalRadiation\MetalDripWellupInvert.part
		part	CustomizeablePowers\CorruptorBuff\ThermalRadiation\MetalDripWellup_Invert.part
		part	CustomizeablePowers\CorruptorBuff\ThermalRadiation\MetalDripWellup.part
		LifeSpan	20
	End
End

#########################################################

End