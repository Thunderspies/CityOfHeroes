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
		At	WepR
		Sound Sonic12 90 90 .9
	End
End

Condition
	On	Time
	Time	29

	Event
		Type	Local
		At	WepR
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
		At	WepR
		bhvr	CustomizeablePowers\SonicAttack\Offset2.bhvr
		BhvrOverride
			PyrRotate	200 90 0
		End
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
	#	Altpiv	4
		Part	CustomizeablePowers\SonicAttack\MussleFlashRingCore.part
		Part	CustomizeablePowers\SonicAttack\MussleFlashRing.part

		LifeSpan 20
	End
End

#############################################################

Condition
	On 	Time
	Time 	29

	Event
		Type	Local
		At	core
	#	Altpiv	4
		part	CustomizeablePowers\SonicAttack\MassiveMegaSonicRingCore.part
		part	CustomizeablePowers\SonicAttack\MassiveMegaSonicRing.part
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
