#############################################################
## MassiveMegaSonicScream2.fx
#############################################################

FxInfo

LifeSpan 50

#############################################################

Condition
	On	Time
	Time	4

	Event
		Type	Local
		At	head
		Sound Sonic12 90 90 .9
	End
End

Condition
	On	Time
	Time	29

	Event
		Type	Local
		At	head
		Sound Sonic11 90 90 1.0
	End
End

#############################################################

Condition
	On 	Time
	Time 	29

	Event
		EName	core
		Type	Local
		At	head
		bhvr	CustomizeablePowers\SonicAttack\Offset2.bhvr
		Geom	LocalPivot01
		LifeSpan 10
	End
End

Condition
	On 	Time
	Time 	29

	Event
		EName	core3
		Type	start
		At	core
		Altpiv	4
		Part	:MussleFlashRingCore.part
		Part	:MussleFlashRing.part
		LifeSpan 20
	End
End

#############################################################

Condition
	On 	Time
	Time 	29

	Event
		Type	start
		At	core
		Altpiv	4
		part	:MassiveMegaSonicRingCore.part
		part	:MassiveMegaSonicRing.part
		LifeSpan 15
	End

	Event
		Type	Local
		At	core
		Altpiv	4
		BhvrOverride
			PositionOffset		0 0 16
			pyrRotate		15 0 0
			PhysForceType		Forward
			PhysForceRadius		32
			PhysForcePower		300
			PhysForcePowerJitter	50
		End
		Lifespan 20
	End
End

#############################################################

End
