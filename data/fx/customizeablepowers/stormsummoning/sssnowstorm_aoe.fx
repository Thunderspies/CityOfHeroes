#########################################################
## Snow Storm Fx
#########################################################

FxInfo

ClampMaxScale 1 1 1

Lifespan 30

#########################################################

Condition
	On	Time
	Time	0

	Event
		EName	OverHeadNode
		Type	posit
		At	Origin
		Geom	OverHeadDummy01
	End
End

Condition
	On	Time
	Time	16

	Event
		Type	local
		At	OverHeadNode
		AltPiv	1
		bhvr    behaviors/SSSpin4.bhvr
		part	:SSSnow01.part
	End

	Event
		Type	local
		At	OverHeadNode
		AltPiv	1
		bhvr    behaviors/SSSpin5.bhvr
		part	:SSSnow02.part
	End
End

Condition
	On	Time
	Time	8

	Event
		EName	Snow01
		Type	posit
		At	Origin
		bhvr    behaviors/SSSpin1.bhvr
		part	:SSSnowStormMist01.part
		part	:SSSnowStormMistAdditive01.part
	End
End

Condition
	On	Time
	Time	16

	Event
		EName	Snow01
		Type	posit
		At	Origin
		bhvr    behaviors/SSSpin2.bhvr
		part	:SSSnowStormMist02.part
		part	:SSSnowStormMistAdditive02.part
	End
End

## PHYSICS FORCES #######################################################

Condition
	On	Time
	Time	8

	Event
		Ename	GravitationalForce
		Type	Local
		At	Origin
		BhvrOverride
			PhysForceType		CWSwirl
			PhysForceRadius		25
			PhysForceCentripetal	.25
			PhysForcePower		40
			PhysForcePowerJitter	5
		End
	End

	Event
		Ename	LevitationForce
		Type	Local
		At	Origin
		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		30
			PhysForcePower		40
			PhysForcePowerJitter	5
		End
	End
End

#############################################################

End