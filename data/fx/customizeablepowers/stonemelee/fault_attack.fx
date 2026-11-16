#############################################################
## Fault_Attack.fx
#############################################################

FxInfo

Lifespan 200

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Streak1
		Type	local
		At	origin
		Sound punch13 100.0 100.0 0.85
	End
End

#############################################################

Condition
	On 	Time
	Time 	21

	Event
		Type	start
		At	mystic
		bhvr	Behaviors\CameraShakeFault.bhvr
		Part	CustomizeablePowers\StoneMelee\FaultCrack01.part
		Part	CustomizeablePowers\StoneMelee\superStrengthDust1.part
		Part	CustomizeablePowers\StoneMelee\superStrengthDust2.part
		Lifespan 40
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	mystic
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		20
			PhysForcePower		250
			PhysForcePowerJitter	100
		End
		Lifespan 1
	End
End

Condition
	On 	Time
	Time 	30

	Event
		EName	Quake
		Type	local
		At	root
		bhvr	CustomizeablePowers\StoneMelee\SlightQuake.bhvr
	End
End

#############################################################

Condition
	On 	Time
	Time 	21

	Event
		EName	prime
		Type	start
		At	mystic
		Bhvr	CustomizeablePowers\StoneMelee\Projectile2.bhvr
		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		15
			PhysForcePower		400
			PhysForcePowerJitter	100
		End
		Part	CustomizeablePowers\StoneMelee\FaultCrack.part
		Part	CustomizeablePowers\StoneMelee\FaultCrack2SmallFaint.part
		Part	CustomizeablePowers\StoneMelee\FaultCrack1SmallFaint.part
		Magnet	T_Root
		Sound quake1 120 120 0.9
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	T_Root
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		30
			PhysForcePower		500
			PhysForcePowerJitter	100
		End
		Lifespan 1
	End

	Event
		EName 	hitrings
		Type	start
		At	T_Root
		Part	CustomizeablePowers\StoneMelee\Faultflash.part
		Part	CustomizeablePowers\StoneMelee\Faultflash2.part
		Part	CustomizeablePowers\StoneMelee\FaultCrackHitRadius.part
		Part	CustomizeablePowers\StoneMelee\FaultCrackHitRadius2.part
	End

	Event
		Type	start
		At	T_Root
		bhvr	CustomizeablePowers\StoneMelee\fault.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_Fault
		Lifespan 30
	End

	Event
		Type	start
		At	T_Root
		bhvr	CustomizeablePowers\StoneMelee\fault1a.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_Fault01
		Lifespan 30
	End

	Event
		Type	start
		At	T_Root
		bhvr	CustomizeablePowers\StoneMelee\fault2.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_Fault02
		Lifespan 30
	End

	Event
		Type	start
		At	T_Root
		bhvr	CustomizeablePowers\StoneMelee\fault1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_Fault03
		Lifespan 30
	End

	Event
		Type	start
		At	T_Root
		bhvr	CustomizeablePowers\StoneMelee\fault2.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_Fault04
		Lifespan 30
	End

	Event
		Type	start
		At	T_Root
		bhvr	CustomizeablePowers\StoneMelee\fault5.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_Fault05
		Lifespan 30
	End

	Event
		EName 	StonecrackGeom
		Type	start
		At	T_root
		Bhvr	CustomizeablePowers\StoneMelee\FaultCrackScale.bhvr
		geom	Crack01
		Lifespan 30
	End

	Event
		EName 	StoneDustRingHit
		Type	start
		At	T_Root
		Part	CustomizeablePowers\StoneMelee\FaultDustFlatLater2.part
		Part	CustomizeablePowers\StoneMelee\FaultDustLater.part
		Part	CustomizeablePowers\StoneMelee\FaultDust2Later.part
		Part	CustomizeablePowers\StoneMelee\FaultDustFlatLater.part
		Part	CustomizeablePowers\StoneMelee\FaultRadiusRocks.part
		Part	CustomizeablePowers\StoneMelee\FaultDustCenter.part
		Part	CustomizeablePowers\StoneMelee\FaultDust2Center.part
		Lifespan 70
	End
End

#############################################################

End