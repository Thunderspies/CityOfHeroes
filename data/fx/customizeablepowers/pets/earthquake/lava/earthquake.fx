#############################################################
## EarthQuake.fx
#############################################################

FxInfo

## AUDIO ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		Sound Rumble1_loop 100 100 0.76
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Root
		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		30
			PhysForcePower		320
			PhysForcePowerJitter	100
		End
		Lifespan 1
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Root
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		30
			PhysForcePower		220
			PhysForcePowerJitter	30
		End
		Lifespan 1
	End
End

Condition
	On	Cycle
	Time	46
	Chance	1

	Event
		Type	start
		At	Root
		Sound Quake3 100 100 0.64
		Sound Quake4 100 100 0.76
		Sound Quake3 100 100 0.68
		Sound Rumble4 100 100 0.55
		Sound Quake5 100 100 0.6
		Lifespan 46
	End
End


Condition
	On	Cycle
	Time	5
	Chance	0.33
	Random	1

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Root
		BhvrOverride
			PhysForceType		In
			PhysForceRadius		45
			PhysForcePower		25
			PhysForcePowerJitter	10
		End
		Lifespan 1
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Root
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		55
			PhysForcePower		100
			PhysForcePowerJitter	10
		End
		Lifespan 1
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Root
		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		50
			PhysForcePower		75
			PhysForcePowerJitter	45
		End
		Lifespan 1
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Root
		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		50
			PhysForcePower		65
			PhysForcePowerJitter	45
		End
		Lifespan 1
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName 	StonecrackGeom
		Type	start
		At	Root
		Bhvr	behaviors\QuakeCrackScale.bhvr
		geom	Crack01
		Sound LavaSteam2_loop 28 28 0.33
	End

#############################################################

	Event
		Type	start
		At	Root
		bhvr	behaviors\Quake.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_Fault_Lava
	End

	Event
		Type	start
		At	Root
		bhvr	behaviors\Quake.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_Fault_Lava_AddGlow
	End

#############################################################

	Event
		Type	start
		At	Root
		bhvr	behaviors\Quake1a.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_Fault_Lava_01
	End

	Event
		Type	start
		At	Root
		bhvr	behaviors\Quake1a.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_Fault_Lava_01_AddGlow
	End

#############################################################

	Event
		Type	start
		At	Root
		bhvr	behaviors\Quake2.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_Fault_Lava_02
	End

	Event
		Type	start
		At	Root
		bhvr	behaviors\Quake2.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_Fault_Lava_02_AddGlow
	End

#############################################################

	Event
		Type	start
		At	Root
		bhvr	behaviors\Quake1.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_Fault_Lava_03
	End

	Event
		Type	start
		At	Root
		bhvr	behaviors\Quake1.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_Fault_Lava_03_AddGlow
	End

#############################################################

	Event
		Type	start
		At	Root
		bhvr	behaviors\Quake2.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_Fault_Lava_04
	End

	Event
		Type	start
		At	Root
		bhvr	behaviors\Quake2.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_Fault_Lava_04_AddGlow
	End

#############################################################

	Event
		Type	start
		At	Root
		bhvr	behaviors\Quake3.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_Fault_Lava_05
	End

	Event
		Type	start
		At	Root
		bhvr	behaviors\Quake3.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_Fault_Lava_05_AddGlow
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName 	hitrings
		Type	start
		At	Root
		Part	CustomizeablePowers\Pets\Earthquake\Lava\QuakeLightRays.part
		Part	CustomizeablePowers\Pets\Earthquake\Lava\QuakeRing.part
		Part	CustomizeablePowers\Pets\Earthquake\Lava\QuakeRingJagged.part
		Part	CustomizeablePowers\Pets\Earthquake\Lava\QuakeRingJaggedGlow.part
		Part	CustomizeablePowers\Pets\Earthquake\Lava\QuakeRingRough.part
	End

	Event
		EName 	StoneDustRingHit
		Type	start
		At	Root
		Part	CustomizeablePowers\Pets\Earthquake\Lava\QuakeDust2Later.part
		Part	CustomizeablePowers\Pets\Earthquake\Lava\QuakeDustFlatLater.part
		Part	CustomizeablePowers\Pets\Earthquake\Lava\QuakeRadiusRocks.part
		Sound Quake3 100 100 0.6
		Lifespan 70
	End
End

#############################################################

Condition
	On 	Time
	Time 	0
	Repeat	24

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 1 0
		End
		ChildFX	CustomizeablePowers\EarthControl\Lava\Child_RockChunkBrownScatter.fx
		Lifespan 100
	End
End

## CAMERA SHAKE ###########################################################

Condition
	On	Time
	Time	0

	Event
		EName 	cameraShake01
		Type	start
		At	Root
		bhvr	Behaviors\EarthQuakeInitial.bhvr
		Lifespan 70
	End
End

Condition
	On	Cycle
	Time	20
	Chance	1

	Event
		Type	start
		At	Root
		Bhvr	Behaviors\EarthQuakeSustained.bhvr
		Lifespan 20
	End
End

#############################################################

End