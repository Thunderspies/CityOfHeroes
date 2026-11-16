#########################################################
## Snow Storm Fx
#########################################################

FxInfo

ClampMaxScale 1 1 1

Lifespan 30

#############################################################

Condition
	On	Time
	Time	8

	Event
		EName	Snow01
		Type	posit
		At	Origin
		bhvr    behaviors\SSSpin2.bhvr
		part	CustomizeablePowers\IceBlast\SSSnowStormMist02.part
	End

	Event
		EName	Snow01
		Type	posit
		At	Origin
		bhvr    behaviors\SSSpin3.bhvr
		part	CustomizeablePowers\IceBlast\SSSnowStormMist03.part
	End
End

Condition
	On	Time
	Time	20

	Event
		EName	Snow01
		Type	posit
		At	Origin
		bhvr    behaviors\SSSpin1.bhvr
		part	CustomizeablePowers\IceBlast\SSSnowStormMist01.part
	End
End

#############################################################

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
			PhysForceCentripetal	0.25
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