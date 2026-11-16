#
FxInfo


###########################################################

Condition
	On	Time
	Time	0

	Event
		EName	ElectricityAnchor
		Type	Start
		At	Root
		BhvrOverride
			Scale		60.0 60.0 60.0
		End
		GEOM	FX_ConeOffsets
	End

	Event
		EName	ElectricityAnchor2
		Type	Start
		At	Root
		BhvrOverride
			Scale		3.0 3.0 3.0
		End
		GEOM	FX_ConeOffsets
	End

	Event
		EName	RootAnchor
		Type	Local
		At	ElectricityAnchor2
		AltPiv	1
		BhvrOverride
			Scale	1.001 1.001 1.001
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

	Event
		EName	Prime
		Type	StartPositOnly
		At	RootAnchor #WepR
		Bhvr	behaviors\projectile1.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		150
			PhysForcePowerJitter	50
		End
		Part	:FrigidSmash_Projectile_Dust.part
		Magnet	ElecTarget01
		LookAt  ElecTarget01
	End

	Event
		EName	PrimeA
		Type	PositOnly
		At	Prime
		Part	:FrigidSmash_Projectile_Shards.part
		Part	:DustPillar_Up.part
		Part	:DustPillar_Up_Additive.part
		Lifespan 17
	End

	Event
		EName	Prime2
		Type	StartPositOnly
		At	RootAnchor #WepR
		Bhvr	behaviors\projectile1.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		150
			PhysForcePowerJitter	50
		End
		Part	:FrigidSmash_Projectile_Dust.part
		Magnet	ElecTarget02
		LookAt  ElecTarget02
	End

	Event
		EName	PrimeB
		Type	PositOnly
		At	Prime2
		Part	:FrigidSmash_Projectile_Shards.part
		Part	:DustPillar_Up.part
		Part	:DustPillar_Up_Additive.part
		Lifespan 17
	End

	Event
		EName	Prime3
		Type	Start
		At	RootAnchor #WepR
		Bhvr	behaviors\projectile1.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		150
			PhysForcePowerJitter	50
		End
		Geom	CenterDummy
		Part	:FrigidSmash_Projectile_Dust.part
		Magnet	ElecTarget03
		LookAt  ElecTarget03
	End

	Event
		EName	PrimeC
		Type	PositOnly
		At	Prime3
		Part	:FrigidSmash_Projectile_Shards.part
		Part	:DustPillar_Up.part
		Part	:DustPillar_Up_Additive.part

		Lifespan 17
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

End
