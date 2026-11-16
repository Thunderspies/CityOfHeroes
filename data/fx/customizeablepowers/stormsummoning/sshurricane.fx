#########################################################
##	template

FxInfo



#########################################################
#####################  Mist 1  #########################
#########################################################

Condition
	On 	Time
	Time 	0


	Event
		EName   RingNode
		Type	local
		Geom	RootToChestAdjustment
		At	Root
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Sound coldblast2 100 100 .8

	End

End

Condition
	On 	Time
	Time 	19

	Event
		EName 	FogRing1
		Type	Local
		At	RingNode
		AltPiv	1
		bhvr	CustomizeablePowers\StormSummoning\SSSpin3.bhvr
		Part	CustomizeablePowers\StormSummoning\SSHurricaneRingSmall.part
		Sound hurricane_loop 100 100 .7

	End

	Event
		Ename	SwirlForce
		Type	Local
		At	RingNode
		AltPiv	1

		BhvrOverride
			PhysForceType		CWSwirl
			PhysForceRadius		30
			PhysForceCentripetal	.6
			PhysForcePower		50
			PhysForcePowerJitter	5
		End

		#Lifespan 40

	End

	Event
		Ename	LevitationForce
		Type	Local
		At	Root

		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		16
			PhysForcePower		45
			PhysForcePowerJitter	5
		End

		#Lifespan	30

	End


	Event
		Ename	LevitationForce
		Type	Local
		At	RingNode
		AltPiv	1

		BhvrOverride
			PositionOffset		0 6 0
			PhysForceType		Up
			PhysForceRadius		10
			PhysForcePower		-15
			PhysForcePowerJitter	5
		End

		#Lifespan	30

	End
End

Condition
	On 	Time
	Time 	20

	Event
		EName 	Hurricane
		Type	Local
		At	RingNode
		AltPiv	1
		Part	CustomizeablePowers\StormSummoning\SSHurricane01.part
		#LifeSpan 120

	End

End

Condition
	On 	Time
	Time 	30

	Event
		EName 	FogRing1
		Type	Local
		At	RingNode
		AltPiv	1
		bhvr	CustomizeablePowers\StormSummoning\SSSpin1.bhvr
		Part	CustomizeablePowers\StormSummoning\SSHurricaneRing.part
		#LifeSpan 120

	End

End


End



