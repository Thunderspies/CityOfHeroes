#############################################################
## SSCloudCast.fx
#############################################################

FxInfo
Lifespan 100

#########################################################
#####################  CLOUD 1  #########################
#########################################################

Condition

	On 	Time
	Time	0

	Event
		ENAME	OverHead
		Type	Local
		At	Root
		Geom	OverHeadDummy
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Sound coldblast2 100 100 .75
	End

End

Condition

	On 	Time
	Time	30

	Event
		ENAME	Lightning
		Type	Start
		At	OverHead
		AltPiv	1
		part	CustomizeablePowers\StormSummoning\SSLSCloudLightning01.part
	End

End

Condition

	On 	Time
	Time	10

	Event
		ENAME	Mist
		Type	Start
		At	OverHead
		AltPiv	1
		part	CustomizeablePowers\StormSummoning\SSCloudCastMist.part
		Bhvr	CustomizeablePowers\StormSummoning\Spin2.bhvr
		LifeSpan 60
	End

	Event
		Ename	GravitationalForce
		Type	Local
		At	OverHead
		AltPiv	1

		BhvrOverride
			PhysForceType		CCWSwirl
			PhysForceRadius		20
			PhysForceCentripetal	.5
			PhysForcePower		45
			PhysForcePowerJitter	5
		End

		LifeSpan 60

	End

	Event
		Ename	LevitationForce
		Type	Local
		At	OverHead
		AltPiv	1

		BhvrOverride
			PositionOffset		0 -6 0
			PhysForceType		Up
			PhysForceRadius		15
			PhysForcePower		25
			PhysForcePowerJitter	5
		End

		LifeSpan 100

	End
End



End