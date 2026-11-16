#############################################################
## StalagmiteAttackHit.fx
#############################################################

FxInfo
Lifespan 100

########################################################

Input
	Inpname	Hips
End

########################################################


## SOUND FX #######################################################

Condition
	On	Time
	Time	0

	Event
		Type	start
		At	Root
		Sound explohit 80 80 0.77
	End
End

## POWER FX #######################################################

Condition
	On	Time
	Time	0

	Event
		Type	start
		At	Root
		Part	CustomizeablePowers\EarthControl\StelagmiteDust2.part
		Part	CustomizeablePowers\EarthControl\StelagmiteDust1.part
	End
End

Condition
	On	Time
	Time	0

	Event
		EName	crack
		Type	start
		At	Root
		Bhvr	CustomizeablePowers\EarthControl\StalagmiteCracks.bhvr
		Splat	ImpactCracks.tga
		Lifespan 100
		LifeSpanJitter 50
	End

	Event
		EName	AOERing
		Type	start
		At	Root
		Part	CustomizeablePowers\EarthControl\StalagmiteHitRing.part
		Lifespan 25
	End
End

## STALAGMITE GEOS #######################################################

Condition
	On	Time
	Time	0

	Event
		Type	start
		At	Root
		Bhvr	CustomizeablePowers\EarthControl\GraspingHands.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_FX_Stalagtite05
		Lifespan 55
	End
End

Condition
	On	Time
	Time	3

	Event
		Type	start
		At	Root
		Bhvr	CustomizeablePowers\EarthControl\GraspingHands.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_FX_Stalagtite01
		Lifespan 55
	End
End

Condition
	On	Time
	Time	5

	Event
		Type	start
		At	Root
		Bhvr	CustomizeablePowers\EarthControl\GraspingHands4.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_FX_Stalagtite06
		Lifespan 55
	End
End

Condition
	On	Time
	Time	8

	Event
		Type	start
		At	Root
		Bhvr	CustomizeablePowers\EarthControl\GraspingHands.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_FX_Stalagtite03
		Lifespan 55
	End
End

#############################################################

Condition
	On	Time
	Time	55

	Event
		EName	Rocks4x
		Type	start
		At	Root
		Bhvr	CustomizeablePowers\EarthControl\GraspingHandsa.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_FX_Stalagtite05
		Lifespan 20
	End
End

Condition
	On	Time
	Time	58

	Event
		EName	Rocks3x
		Type	start
		At	Root
		Bhvr	CustomizeablePowers\EarthControl\GraspingHandsa.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_FX_Stalagtite01
		Lifespan 20
	End
End

Condition
	On	Time
	Time	60

	Event
		EName	Rocks2x
		Type	start
		At	Root
		Bhvr	CustomizeablePowers\EarthControl\GraspingHands4a.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_FX_Stalagtite06
		Lifespan 20
	End
End

Condition
	On	Time
	Time	63

	Event
		Type	start
		At	Root
		Bhvr	CustomizeablePowers\EarthControl\GraspingHandsa.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_FX_Stalagtite03
		Lifespan 55
	End
End

## PHYSICS PARTICLES #######################################################

Condition
	On 	Time
	Time 	0
	Repeat	4

	Event
		Type	start
		At	Root
		ChildFX	:Child_RockDebrisGold.fx
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Root
		ChildFX	:Child_RockScatter2.fx
	End

	Event
		HardwareOnly
		Type	Start
		At	Root
		ChildFX	:Child_RockScatter2.fx
	End
End

## PHYSICS FORCES #######################################################

Condition
	On	Time
	Time	0

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Root
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		150
			PhysForcePowerJitter	50
		End
		Lifespan 10
	End
End

#########################################################

End