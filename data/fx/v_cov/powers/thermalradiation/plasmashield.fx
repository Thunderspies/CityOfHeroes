#############################################################
## PlasmaShield.fx
#############################################################

FxInfo

#########################################################

Input
	Inpname	Hips
End

#########################################################

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
		Part1	V_COV\Powers\ThermalRadiation\PlasmaShieldCloud01B.part
		Part2	V_COV\Powers\ThermalRadiation\PlasmaShieldCloud02B.part
		Part4	V_COV\Powers\ThermalRadiation\PlasmaShieldSparksB.part
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
		Part1	V_COV\Powers\ThermalRadiation\PlasmaShieldCloud01B.part
		Part2	V_COV\Powers\ThermalRadiation\PlasmaShieldCloud02B.part
		Part4	V_COV\Powers\ThermalRadiation\PlasmaShieldSparksB.part
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
		part	V_COV\Powers\ThermalRadiation\PlasmaFireCore.part
	End

	Event
		EName	Hips
		Type	Local
		At	Hips
		part	V_COV\Powers\ThermalRadiation\PlasmaFireCoreTrail1.part
	End

	Event
		EName	Chest
		Type	Local
		At	back
		part	V_COV\Powers\ThermalRadiation\PlasmaFireCore4.part
	End

	Event
		EName	Chest
		Type	Local
		At	Chest
		part	V_COV\Powers\ThermalRadiation\PlasmaFireCoreTrail2.part
		Part	V_COV\Powers\ThermalRadiation\PlasmaFireCoreSparks.part
	End

	Event
		EName	Lleg
		Type	Local
		At	LlegL
		part	V_COV\Powers\ThermalRadiation\PlasmaFireCore1.part
		part	V_COV\Powers\ThermalRadiation\PlasmaFireCoreTrail.part
	End

	Event
		EName	Rleg
		Type	Local
		At	LlegR
		part	V_COV\Powers\ThermalRadiation\PlasmaFireCore1.part
		part	V_COV\Powers\ThermalRadiation\PlasmaFireCoreTrail.part
	End

	Event
		EName	Lleg
		Type	Local
		At	FootL
		part	V_COV\Powers\ThermalRadiation\PlasmaFireCore1Feet.part
		part	V_COV\Powers\ThermalRadiation\PlasmaFireCoreTrailFeet.part
	End

	Event
		EName	Rleg
		Type	Local
		At	FootR
		part	V_COV\Powers\ThermalRadiation\PlasmaFireCore1Feet.part
		part	V_COV\Powers\ThermalRadiation\PlasmaFireCoreTrailFeet.part
	End
End

#############################################################

#Condition
#	On	Time
#	Time	0
#
#	Event
#		EName	ChildPlasmaContinuing.FX
#		Type	Local
#		At	Root
#		ChildFX	V_COV\GenericPowerContinuingFx\PlasmaContinuing.fx
#	End
#End

#############################################################

End