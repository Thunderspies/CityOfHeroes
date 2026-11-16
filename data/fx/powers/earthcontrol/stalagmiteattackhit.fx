#########################################################
## Stone Control - Stalagmite Hit
#########################################################

FxInfo
LifeSpan 100

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
		Part2	:StelagmiteDust2.part
		part3	:StelagmiteDust1.part
	End
End

Condition
	On	Time
	Time	0

	Event
		EName	crack
		Type	start
		At	Root
		Bhvr	Powers/Earthcontrol/StalagmiteCracks.bhvr
		Splat	ImpactCracks.tga
		Lifespan	100
		LifeSpanJitter	50
	End

	Event
		EName	AOERing
		Type	start
		At	Root
		Part2	:StalagmiteHitRing.part
		Lifespan	25
	End
End

## STALAGMITE GEOS #######################################################

Condition
	On	Time
	Time	0

	Event
		EName	Rocks4
		Type	start
		At	Root
		Bhvr	behaviors/GraspingHands.bhvr
		Geom	FX_Stalagtite05
		Lifespan 63
	End
End

Condition
	On	Time
	Time	3

	Event
		EName	Rocks3
		Type	start
		At	Root
		Bhvr	behaviors/GraspingHands.bhvr
		Geom	FX_Stalagtite01
		Lifespan 55
	End
End

Condition
	On	Time
	Time	5

	Event
		EName	Rocks2
		Type	start
		At	Root

		Bhvr	behaviors/GraspingHands4.bhvr
		Geom	FX_Stalagtite06
		Lifespan 55
	End
End

Condition
	On	Time
	Time	56

	Event
		EName	Rocksx
		Type	start
		At	Root
		Bhvr	behaviors/GraspingHands3a.bhvr
		Geom	FX_Stalagtite03
		LifeSpan 20
	End
End

Condition
	On	Time
	Time	57

	Event
		EName	Rocks3x
		Type	start
		At	Root
		Bhvr	behaviors/GraspingHandsa.bhvr
		Geom	FX_Stalagtite01
		LifeSpan 20
	End
End

Condition
	On	Time
	Time	61

	Event
		EName	Rocks2x
		Type	start
		At	Root
		Geom	FX_Stalagtite06
		Bhvr	behaviors/GraspingHands4a.bhvr
		LifeSpan 20
	End
End

Condition
	On	Time
	Time	64

	Event
		EName	Rocks4x
		Type	start
		At	Root
		Bhvr	behaviors/GraspingHandsa.bhvr
		Geom	FX_Stalagtite05
		LifeSpan 20
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
		ChildFX	V_COV\PhysicsEnabled\RockDebrisGold.fx
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