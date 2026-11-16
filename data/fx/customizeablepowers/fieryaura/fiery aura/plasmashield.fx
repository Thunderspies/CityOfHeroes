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
		Sound flasharrowhit 70 70 .85
		Sound ignite2 50 50 .5

	End

	Event
		Type	Local
		At	Origin
		Sound ignite2 50 50 .5
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
		Part1	:PlasmaShieldCloud01B.part
		Part2	:PlasmaShieldCloud02B.part
		Part4	:PlasmaShieldSparksB.part
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
		Part1	:PlasmaShieldCloud01B.part
		Part2	:PlasmaShieldCloud02B.part
		Part4	:PlasmaShieldSparksB.part
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
		part	:PlasmaFireCore.part
	End

	Event
		EName	Hips
		Type	Local
		At	Hips
		part	:PlasmaFireCoreTrail1.part
	End

	Event
		EName	Chest
		Type	Local
		At	back
		part	:PlasmaFireCore4.part
	End

	Event
		EName	Chest
		Type	Local
		At	Chest
		part	:PlasmaFireCoreTrail2.part
		Part	:PlasmaFireCoreSparks.part
	End

	Event
		EName	Lleg
		Type	Local
		At	LlegL
		part	:PlasmaFireCore1.part
		part	:PlasmaFireCoreTrail.part
	End

	Event
		EName	Rleg
		Type	Local
		At	LlegR
		part	:PlasmaFireCore1.part
		part	:PlasmaFireCoreTrail.part
	End

	Event
		EName	Lleg
		Type	Local
		At	FootL
		part	:PlasmaFireCore1Feet.part
		part	:PlasmaFireCoreTrailFeet.part
	End

	Event
		EName	Rleg
		Type	Local
		At	FootR
		part	:PlasmaFireCore1Feet.part
		part	:PlasmaFireCoreTrailFeet.part
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
		ChildFX	:PlasmaContinuing.fx
	End
End

#############################################################

End