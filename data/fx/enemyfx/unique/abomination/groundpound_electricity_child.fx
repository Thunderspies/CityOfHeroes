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
			Scale	20.0 20.0 20.0
		End
		GEOM	FX_ConeOffsets
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
		Type	start
		At	Root
		Bhvr	behaviors\projectile1.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		150
			PhysForcePowerJitter	50
		End
		Part	:Electricity_Projectile.part
		Magnet	ElecTarget01
		LookAt  ElecTarget01
	End

	Event
		EName	Prime2
		Type	start
		At	Root
		Bhvr	behaviors\projectile1.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		150
			PhysForcePowerJitter	50
		End
		Part	:Electricity_Projectile.part
		Magnet	ElecTarget02
		LookAt  ElecTarget02
	End

	Event
		EName	Prime3
		Type	start
		At	Root
		Bhvr	behaviors\projectile1.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		150
			PhysForcePowerJitter	50
		End
		Geom	CenterDummy
		Part	:Electricity_Projectile.part
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
