#############################################################
## GraspingEarthHitLarge.fx
#############################################################

FxInfo
ClampMaxScale 3.5 3.5 3.5

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	dum
		Type	Create
		Inherit	ALL
		Update	Position
		At	Root
		Bhvr	behaviors\StoneCages.bhvr
		geom	DummyEarth
		Sound GraniteEarthHit3 100 100 1.0
	End
End

Condition
	On 	Time
	Time 	7

	Event
		EName 	Stonesickle05
		Type	Create
		Inherit	ALL
		Update	Position
		At	Root
		Bhvr	CustomizeablePowers\EarthControl\Splat_GroundCracks.bhvr
		Splat	ImpactCracks.tga
		Sound explohit 100 100 0.75
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Stonesickle05
		Type	local
		At	dum
		AltPiv	5
		Bhvr	behaviors\GraspingHandz.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part2	CustomizeablePowers\EarthControl\EarthDust.part
		Part3	CustomizeablePowers\EarthControl\EarthDust2.part
		Sound explohit 100 100 0.75
		geom	Tintable_FX_StoneWall_05
	End
End

Condition
	On	Time
	Time	3

	Event
		EName 	Stonesickle09
		Type	local
		At	dum
		AltPiv	9
		Bhvr	behaviors\GraspingHandz.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	Tintable_FX_StoneWall_01
		Part2	CustomizeablePowers\EarthControl\EarthDust.part
		Part3	CustomizeablePowers\EarthControl\EarthDust2.part
		SOund	Stone_loop 80 30 0.6
	End
End

Condition
	On	Time
	Time	14

	Event
		EName 	Stonesickle12
		Type	local
		At	dum
		AltPiv	12
		Bhvr	behaviors\GraspingHandz.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part2	CustomizeablePowers\EarthControl\EarthDust.part
		Part3	CustomizeablePowers\EarthControl\EarthDust2.part
		geom	Tintable_FX_StoneWall_12
	End
End

Condition
	On 	Time
	Time 	6

	Event
		EName 	Stonesickle07
		Type	local
		At	dum
		AltPiv	7
		Bhvr	behaviors\GraspingHandz.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part2	CustomizeablePowers\EarthControl\EarthDust.part
		Part3	CustomizeablePowers\EarthControl\EarthDust2.part
		Sound explohit 100 100 0.5
		geom	Tintable_FX_StoneWall_09
	End
End

Condition
	On 	Time
	Time 	9
	Event
		EName 	Stonesickle01
		Type	local
		At	dum
		AltPiv	1
		Bhvr	behaviors\GraspingHands1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part2	CustomizeablePowers\EarthControl\EarthDust.part
		Part3	CustomizeablePowers\EarthControl\EarthDust2.part
		geom	Tintable_FX_StoneWall_07
	End
End

Condition
	On 	Time
	Time 	7

	Event
		EName 	Stonesickle02
		Type	local
		At	dum
		AltPiv 	2
		Bhvr	behaviors\GraspingHands2.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part2	CustomizeablePowers\EarthControl\EarthDust.part
		Part3	CustomizeablePowers\EarthControl\EarthDust2.part
		geom	Tintable_FX_StoneWall_02
	End
End

Condition
	On 	Time
	Time 	13

	Event
		EName 	Stonesickle11
		Type	local
		At	dum
		AltPiv	11
		Bhvr	behaviors\GraspingHands2.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part2	CustomizeablePowers\EarthControl\EarthDust.part
		Part3	CustomizeablePowers\EarthControl\EarthDust2.part
		geom	Tintable_FX_StoneWall_08
	End
End

Condition
	On	Time
	Time	6

	Event
		EName 	Stonesickle08
		Type	local
		At	dum
		AltPiv	8
		Bhvr	behaviors\GraspingHands1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part2	CustomizeablePowers\EarthControl\EarthDust.part
		Part3	CustomizeablePowers\EarthControl\EarthDust2.part
		geom	Tintable_FX_StoneWall_11
	End

	Event
		EName 	Stonesickle04
		Type	local
		At	dum
		AltPiv	4
		Bhvr	behaviors\GraspingHands2.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part2	CustomizeablePowers\EarthControl\EarthDust.part
		Part3	CustomizeablePowers\EarthControl\EarthDust2.part
		geom	Tintable_FX_StoneWall_04
	End
End

Condition
	On	Time
	Time	12

	Event
		EName 	Stonesickle03
		Type	local
		At	dum
		AltPiv	3
		Bhvr	behaviors\GraspingHands1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part2	CustomizeablePowers\EarthControl\EarthDust.part
		Part3	CustomizeablePowers\EarthControl\EarthDust2.part
		geom	Tintable_FX_StoneWall_03
	End
End

Condition
	On 	Time
	Time 	8

	Event
		EName 	Stonesickle06
		Type	local
		At	dum
		AltPiv	6
		Bhvr	behaviors\GraspingHands2.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part2	CustomizeablePowers\EarthControl\EarthDust.part
		Part3	CustomizeablePowers\EarthControl\EarthDust2.part
		geom	Tintable_FX_StoneWall_06
	End
End

Condition
	On 	Time
	Time 	5

	Event
		EName 	Stonesickle10
		Type	local
		At	dum
		AltPiv	10
		Bhvr	behaviors\GraspingHands1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part2	CustomizeablePowers\EarthControl\EarthDust.part
		Part3	CustomizeablePowers\EarthControl\EarthDust2.part
		geom	Tintable_FX_StoneWall_10
	End
End

#############################################################

Condition
	On 	Time
	Time 	7

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Root
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		20
			PhysForcePower		150
			PhysForcePowerJitter	50
		End
		Lifespan 1
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Root
		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		15
			PhysForcePower		200
			PhysForcePowerJitter	50
		End
		Lifespan 15
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Root
		ChildFX	:Child_RockScatter1.fx
	End

	Event
		HardwareOnly
		Type	Start
		At	Root
		ChildFX	:Child_RockScatter1.fx
	End
End

#############################################################

End