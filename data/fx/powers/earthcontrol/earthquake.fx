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
		EName 	hitrings
		Type	start
		At	Root
		Part	Powers\EarthControl\QuakeLightRays.part
		Part	Powers\EarthControl\QuakeRing.part
		Part	Powers\EarthControl\QuakeRingJagged.part
		Part	Powers\EarthControl\QuakeRingJaggedGlow.part
		Part	Powers\EarthControl\QuakeRingRough.part
	End

	Event
		EName 	hitrings
		Type	start
		At	Root
		Geom	Quake
		bhvr	behaviors\Quake.bhvr
	End

	Event
		EName 	hitrings
		Type	start
		At	Root
		Geom	Quake01
		bhvr	behaviors\Quake1a.bhvr
	End

	Event
		EName 	hitrings
		Type	start
		At	Root
		Geom	Quake02
		bhvr	behaviors\Quake2.bhvr
	End

	Event
		EName 	hitrings
		Type	start
		At	Root
		Geom	Quake03
		bhvr	behaviors\Quake1.bhvr
	End

	Event
		EName 	hitrings
		Type	start
		At	Root
		Geom	Quake04
		bhvr	behaviors\Quake2.bhvr
	End

	Event
		EName 	hitrings
		Type	start
		At	Root
		Geom	Quake05
		bhvr	behaviors\Quake3.bhvr
	End

	Event
		EName 	StonecrackGeom
		Type	start
		At	Root
		Bhvr	behaviors\QuakeCrackScale.bhvr
		geom	Crack01
		Sound LavaSteam2_loop 28 28 0.33
	End

	Event
		EName 	StoneDustRingHit
		Type	start
		At	Root
		Part	Powers\EarthControl\QuakeDustFlatLater2.part
		Part	Powers\EarthControl\QuakeDustLater.part
		Part	Powers\EarthControl\QuakeDust2Later.part
		Part	Powers\EarthControl\QuakeDustFlatLater.part
		Part	Powers\EarthControl\QuakeRadiusRocks.part
		Sound Quake3 100 100 0.6
		Lifespan 70
	End
End

Condition
	On 	Time
	Time 	0
	Repeat	24

	Event
		Type	Local
		At	StoneDustRingHit
		BhvrOverride
			PositionOffset	0 1 0
		End
		ChildFX	V_COV\PhysicsEnabled\Small_RockChunkGrayScatter.fx
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