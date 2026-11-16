#############################################################
## SSSnowStormSelfTarget.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	8

	Event
		EName	Snow01
		Type	posit
		At	T_root
		bhvr    behaviors\SSSpin2.bhvr
		part	CustomizeablePowers\IceBlast\SSSnowStormMist02.part
	End

	Event
		EName	Snow01
		Type	posit
		At	T_root
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
		At	T_root
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
		At	T_root
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
		At	T_root
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