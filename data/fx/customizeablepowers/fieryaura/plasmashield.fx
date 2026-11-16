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

#Condition
#	On	Time
#	Time	15
#
#	Event
#		EName   RadiusFxScalex
#		Type	local
#		At	root
#		Bhvr	powers\firecontrol\FireShield.bhvr
#		LifeSpan 30
#	End
#
#	Event
#		EName 	Prime1
#		Type	Local
#		At	RadiusFxScalex
#		bhvr    behaviors\powers\firecontrol\Infernobubble4.bhvr
#		Part1	:PlasmaShieldCloud01B.part
#		Part2	:PlasmaShieldCloud02B.part
#		Part4	:PlasmaShieldSparksB.part
#		LifeSpan 30
#	End
#End
#
#Condition
#	On	Time
#	Time	65
#
#	Event
#		EName   RadiusFxScalex
#		Type	local
#		At	root
#		Bhvr	powers\firecontrol\FireShield.bhvr
#		LifeSpan 30
#	End
#
#	Event
#		EName 	Prime1
#		Type	Local
#		At	RadiusFxScalex
#		bhvr    behaviors\powers\firecontrol\Infernobubble4.bhvr
#		Part1	:PlasmaShieldCloud01B.part
#		Part2	:PlasmaShieldCloud02B.part
#		Part4	:PlasmaShieldSparksB.part
#		LifeSpan 30
#	End
#End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Pivot
		Type	Local
		At	head
		part	:FireCore.part
	End

	Event
		EName	Hips
		Type	Local
		At	Hips
		part	:FireCoreTrail1.part
	End

	Event
		EName	Chest
		Type	Local
		At	back
		part	:FireCore4.part
	End

	Event
		EName	Chest
		Type	Local
		At	Chest
		part	:FireCoreTrail2.part
		Part	:FireCoreSparks.part
	End

	Event
		EName	Lleg
		Type	Local
		At	LlegL
		part	:FireCore1.part
		part	:FireCoreTrail.part
	End

	Event
		EName	Rleg
		Type	Local
		At	LlegR
		part	:FireCore1.part
		part	:FireCoreTrail.part
	End

	Event
		EName	Lleg
		Type	Local
		At	FootL
		part	:FireCore1Feet.part
		part	:FireCoreTrailFeet.part
	End

	Event
		EName	Rleg
		Type	Local
		At	FootR
		part	:FireCore1Feet.part
		part	:FireCoreTrailFeet.part
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