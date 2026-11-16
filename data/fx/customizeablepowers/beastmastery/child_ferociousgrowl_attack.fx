#############################################################
## GroundPound_Electricity_CHILD.fx
#############################################################

FxInfo
LifeSpan 50

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	ElectricityAnchor
		Type	Local
		At	Root
		BhvrOverride
			PYRRotate	0 -20 0
			PositionOffset	0.0 0.0 -30
			Scale		50.0 50.0 50.0
		End
		GEOM	FX_ConeOffsets
	End

	Event
		EName	TEST
		Type	Local
		At	ElectricityAnchor
		BhvrOverride
			Scale	0.2 0.2 0.2
		End
	End

	Event
		EName	ElecTarget01
		Type	Local
		At	ElectricityAnchor
		AltPiv	1
		BhvrOverride
			Scale	0.1 0.1 0.1
		End

	End


	Event
		EName	ElecTarget02
		Type	Local
		At	ElectricityAnchor
		AltPiv	5
		BhvrOverride
			Scale	0.1 0.1 0.1
		End
	End

	Event
		EName	ElecTarget03
		Type	Local
		At	ElectricityAnchor
		AltPiv	6
		BhvrOverride
			Scale	0.1 0.1 0.1
		End
	End

#############################################################

	Event
		EName	Prime
		Type	Start
		At	ElectricityAnchor
		Bhvr	behaviors\projectile2.bhvr
		BhvrOverride
			TrackRate		3.5

			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		150
			PhysForcePowerJitter	50
		End
		Part	:Dust_Projectile.part
		Magnet	ElecTarget01
		LookAt  ElecTarget01
	End

	Event
		EName	Prime2
		Type	start
		At	ElectricityAnchor
		Bhvr	behaviors\projectile2.bhvr
		BhvrOverride
			TrackRate		3.5

			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		150
			PhysForcePowerJitter	50
		End
		Part	:Dust_Projectile.part
		Magnet	ElecTarget02
		LookAt  ElecTarget02
	End

	Event
		EName	Prime3
		Type	start
		At	ElectricityAnchor
		Bhvr	behaviors\projectile2.bhvr
		BhvrOverride
			TrackRate		3.5

			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		150
			PhysForcePowerJitter	50
		End
		Geom	CenterDummy
		Part	:Dust_Projectile.part
		Magnet	ElecTarget03
		LookAt  ElecTarget03
	End
End

Condition
	On	PrimeHit

	Event
		EName	Prime
		Type	DESTROY
	End
End

Condition
	On	Prime2Hit

	Event
		EName	Prime2
		Type	DESTROY
	End
End

Condition
	On	Prime3Hit

	Event
		EName	Prime3
		Type	DESTROY
	End
End
#############################################################

End
