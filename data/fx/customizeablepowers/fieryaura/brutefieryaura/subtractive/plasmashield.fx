#############################################################
## PlasmaShield.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		Sound flasharrowhit 70 70 0.85
		Sound ignite2 50 50 0.5

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
		Part1	CustomizeablePowers\ThermalRadiation\Subtractive\FireShieldCloud01B.part
		Part2	CustomizeablePowers\ThermalRadiation\Subtractive\FireShieldCloud02B.part
		Part2	CustomizeablePowers\ThermalRadiation\Subtractive\FireShieldCloud02B_Alpha.part
		Part4	CustomizeablePowers\ThermalRadiation\Subtractive\FireShieldSparksB.part
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
		Part1	CustomizeablePowers\ThermalRadiation\Subtractive\FireShieldCloud01B.part
		Part2	CustomizeablePowers\ThermalRadiation\Subtractive\FireShieldCloud02B.part
		Part2	CustomizeablePowers\ThermalRadiation\Subtractive\FireShieldCloud02B_Alpha.part
		Part4	CustomizeablePowers\ThermalRadiation\Subtractive\FireShieldSparksB.part
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
		part	CustomizeablePowers\ThermalRadiation\Subtractive\FireCore.part
	End

	Event
		EName	Hips
		Type	Local
		At	Hips
		part	CustomizeablePowers\ThermalRadiation\Subtractive\FireCoreTrail1.part
	End

	Event
		EName	Chest
		Type	Local
		At	back
		part	CustomizeablePowers\ThermalRadiation\Subtractive\FireCore4.part
	End

	Event
		EName	Chest
		Type	Local
		At	Chest
		part	CustomizeablePowers\ThermalRadiation\Subtractive\FireCoreTrail2.part
		Part	CustomizeablePowers\ThermalRadiation\Subtractive\FireCoreSparks.part
	End

	Event
		EName	Lleg
		Type	Local
		At	LlegL
		part	CustomizeablePowers\ThermalRadiation\Subtractive\FireCore1.part
		part	CustomizeablePowers\ThermalRadiation\Subtractive\FireCoreTrail.part
	End

	Event
		EName	Rleg
		Type	Local
		At	LlegR
		part	CustomizeablePowers\ThermalRadiation\Subtractive\FireCore1.part
		part	CustomizeablePowers\ThermalRadiation\Subtractive\FireCoreTrail.part
	End

	Event
		EName	Lleg
		Type	Local
		At	FootL
		part	CustomizeablePowers\ThermalRadiation\Subtractive\FireCore1Feet.part
		part	CustomizeablePowers\ThermalRadiation\Subtractive\FireCoreTrailFeet.part
	End

	Event
		EName	Rleg
		Type	Local
		At	FootR
		part	CustomizeablePowers\ThermalRadiation\Subtractive\FireCore1Feet.part
		part	CustomizeablePowers\ThermalRadiation\Subtractive\FireCoreTrailFeet.part
	End
End

#############################################################

End