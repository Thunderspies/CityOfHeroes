#############################################################
## PlasmaShield.fx
#############################################################

FxInfo

#########################################################

Input
	Inpname	Hips
End

#########################################################\

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		Sound flasharrowhit 70 70 0.85
	End

	Event
		Type	Local
		At	Origin
		Sound ignite2 50 50 0.5
	End
End

#############################################################

Condition
	On	Time
	Time	15

	Event
		EName   RadiusFxScalex
		Type	local
		At	root
		Bhvr	powers\firecontrol\FireShield.bhvr
		LifeSpan 30
	End

	Event
		EName 	Prime1
		Type	Local
		At	RadiusFxScalex
		bhvr    behaviors\powers\firecontrol\Infernobubble4.bhvr
		Part1	CustomizeablePowers\ThermalRadiation\PlasmaShieldCloud01B.part
		Part2	CustomizeablePowers\ThermalRadiation\PlasmaShieldCloud02B.part
		Part4	CustomizeablePowers\ThermalRadiation\PlasmaShieldSparksB.part
		LifeSpan 30
	End
End

Condition
	On	Time
	Time	65

	Event
		EName   RadiusFxScalex
		Type	local
		At	root
		Bhvr	powers\firecontrol\FireShield.bhvr
		LifeSpan 30
	End

	Event
		EName 	Prime1
		Type	Local
		At	RadiusFxScalex
		bhvr    behaviors\powers\firecontrol\Infernobubble4.bhvr
		Part1	CustomizeablePowers\ThermalRadiation\PlasmaShieldCloud01B.part
		Part2	CustomizeablePowers\ThermalRadiation\PlasmaShieldCloud02B.part
		Part4	CustomizeablePowers\ThermalRadiation\PlasmaShieldSparksB.part
		LifeSpan 30
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Pivot
		Type	Local
		At	head
		part	CustomizeablePowers\ThermalRadiation\PlasmaFireCore.part
	End

	Event
		EName	Hips
		Type	Local
		At	Hips
		part	CustomizeablePowers\ThermalRadiation\PlasmaFireCoreTrail1.part
	End

	Event
		EName	Chest
		Type	Local
		At	back
		part	CustomizeablePowers\ThermalRadiation\PlasmaFireCore4.part
	End

	Event
		EName	Chest
		Type	Local
		At	Chest
		part	CustomizeablePowers\ThermalRadiation\PlasmaFireCoreTrail2.part
		Part	CustomizeablePowers\ThermalRadiation\PlasmaFireCoreSparks.part
	End

	Event
		EName	Lleg
		Type	Local
		At	LlegL
		part	CustomizeablePowers\ThermalRadiation\PlasmaFireCore1.part
		part	CustomizeablePowers\ThermalRadiation\PlasmaFireCoreTrail.part
	End

	Event
		EName	Rleg
		Type	Local
		At	LlegR
		part	CustomizeablePowers\ThermalRadiation\PlasmaFireCore1.part
		part	CustomizeablePowers\ThermalRadiation\PlasmaFireCoreTrail.part
	End

	Event
		EName	Lleg
		Type	Local
		At	FootL
		part	CustomizeablePowers\ThermalRadiation\PlasmaFireCore1Feet.part
		part	CustomizeablePowers\ThermalRadiation\PlasmaFireCoreTrailFeet.part
	End

	Event
		EName	Rleg
		Type	Local
		At	FootR
		part	CustomizeablePowers\ThermalRadiation\PlasmaFireCore1Feet.part
		part	CustomizeablePowers\ThermalRadiation\PlasmaFireCoreTrailFeet.part
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	ChildPlasmaContinuing.FX
		Type	Local
		At	Root
		ChildFX	CustomizeablePowers\ThermalRadiation\PlasmaShield_Continuing.fx
	End
End

#############################################################

End