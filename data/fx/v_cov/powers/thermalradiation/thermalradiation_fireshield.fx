#############################################################
## FireShield.fx
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
		Type	local
		At	root
		Bhvr	powers\firecontrol\FireShield.bhvr
		Sound aura6 50 50 .24
		Sound aura5 50 50 .22
		Sound aura7 50 50 .19
		LifeSpan 30
	End
End

#############################################################

Condition
	On	Time
	Time	20

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
		Part1	V_COV\Powers\ThermalRadiation\FireShieldCloud01B.part
		Part2	V_COV\Powers\ThermalRadiation\FireShieldCloud02B.part
		Part4	V_COV\Powers\ThermalRadiation\FireShieldSparksB.part
		LifeSpan 30
	End
End

Condition
	On	Time
	Time	0

	Event
		EName	Pivot
		Type	Local
		At	head
		part2	V_COV\Powers\ThermalRadiation\FireCore.part
	End

	Event
		EName	Hips
		Type	Local
		At	Hips
		part3	V_COV\Powers\ThermalRadiation\FireCoreTrail1.part
	End

	Event
		EName	Chest
		Type	Local
		At	back
		part2	V_COV\Powers\ThermalRadiation\FireCore4.part
	End

	Event
		EName	Chest
		Type	Local
		At	Chest
		part3	V_COV\Powers\ThermalRadiation\FireCoreTrail2.part
		Part4	V_COV\Powers\ThermalRadiation\FireCoreSparks.part
	End

	Event
		EName	Lleg
		Type	Local
		At	LlegL
		part1	V_COV\Powers\ThermalRadiation\FireCore1.part
		part2	V_COV\Powers\ThermalRadiation\FireCoreTrail.part
	End

	Event
		EName	Rleg
		Type	Local
		At	LlegR
		part1	V_COV\Powers\ThermalRadiation\FireCore1.part
		part2	V_COV\Powers\ThermalRadiation\FireCoreTrail.part
	End

	Event
		EName	Lleg
		Type	Local
		At	FootL
		part1	V_COV\Powers\ThermalRadiation\FireCore1Feet.part
		part2	V_COV\Powers\ThermalRadiation\FireCoreTrailFeet.part
	End

	Event
		EName	Rleg
		Type	Local
		At	FootR
		part1	V_COV\Powers\ThermalRadiation\FireCore1Feet.part
		part2	V_COV\Powers\ThermalRadiation\FireCoreTrailFeet.part
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	ChildFXFireContinuing
		Type	Local
		At	head
		ChildFX	V_COV\GenericPowerContinuingFx\FireContinuing.fx
	End
End

#############################################################

End