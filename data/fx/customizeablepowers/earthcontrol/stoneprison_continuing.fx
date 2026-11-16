#############################################################
## HEADER
#############################################################

FxInfo

ClampMaxScale 3.5 3.5 3.5

#############################################################

Condition
	On 	Time
	Time 	0
	Repeat	11

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 1 0
			StartJitter	3 0 3
		End
		ChildFX	V_COV\PhysicsEnabled\RockDebrisGoldScatter.fx
		Lifespan 100
	End
End

Condition
	On 	Time
	Time 	5
	Repeat	11

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 1 0
			StartJitter	3 0 3
		End
		ChildFX	V_COV\PhysicsEnabled\RockDebrisGrayScatter.fx
		Lifespan 100
	End
End

Condition
	On 	Time
	Time 	7
	Repeat	11

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 1 0
			StartJitter	3 0 3
		End
		ChildFX	V_COV\PhysicsEnabled\RockDebrisGoldScatter.fx
		Lifespan 100
	End
End

Condition
	On 	Time
	Time 	11
	Repeat	11

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 1 0
			StartJitter	3 0 3
		End
		ChildFX	V_COV\PhysicsEnabled\RockDebrisGrayScatter.fx
		Lifespan 100
	End
End

Condition
	On 	Time
	Time 	14
	Repeat	11

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 1 0
			StartJitter	3 0 3
		End
		ChildFX	V_COV\PhysicsEnabled\RockDebrisGoldScatter.fx
		Lifespan 100
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	dum
		Type	Create
		Inherit	ALL
		Update	Position
		At	Root
		Bhvr	CustomizeablePowers\EarthControl\GenericEarthFade2.bhvr
		geom	DummyEarth
		Sound GraniteEarthHit3 100 100 1.0
	End

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

Condition
	On 	Time
	Time 	0

	Event
		EName 	Stonesickle05
		Type	local
		At	dum
		AltPiv	5
		Bhvr	CustomizeablePowers\EarthControl\GraspingHandz.bhvr
		geom	FX_StoneWall_05
		Part1	CustomizeablePowers\EarthControl\EarthCrack01.part
		Part2	CustomizeablePowers\EarthControl\EarthDust.part
		Part3	CustomizeablePowers\EarthControl\EarthDust2.part
		Sound explohit 100 100 .75
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
		Bhvr	CustomizeablePowers\EarthControl\GraspingHandz.bhvr
		geom	FX_StoneWall_01
		Part1	CustomizeablePowers\EarthControl\EarthCrack01.part
		Part2	CustomizeablePowers\EarthControl\EarthDust.part
		Part3	CustomizeablePowers\EarthControl\EarthDust2.part
		#Part4	:HandRocks01a.part
		#Part5	:HandRocks02a.part
		SOund	Stone_loop 80 30 .6
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
		Bhvr	CustomizeablePowers\EarthControl\GraspingHandz.bhvr
		geom	FX_StoneWall_12
		Part1	CustomizeablePowers\EarthControl\EarthCrack01.part
		Part2	CustomizeablePowers\EarthControl\EarthDust.part
		Part3	CustomizeablePowers\EarthControl\EarthDust2.part
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
		Bhvr	CustomizeablePowers\EarthControl\GraspingHandz.bhvr
		geom	FX_StoneWall_09
		Part1	CustomizeablePowers\EarthControl\EarthCrack01.part
		Part2	CustomizeablePowers\EarthControl\EarthDust.part
		Part3	CustomizeablePowers\EarthControl\EarthDust2.part
		Sound explohit 100 100 .5
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
		Bhvr	CustomizeablePowers\EarthControl\GraspingHands1.bhvr
		geom	FX_StoneWall_07
		Part1	CustomizeablePowers\EarthControl\EarthCrack01.part
		Part2	CustomizeablePowers\EarthControl\EarthDust.part
		Part3	CustomizeablePowers\EarthControl\EarthDust2.part
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
		Bhvr	CustomizeablePowers\EarthControl\GraspingHands2.bhvr
		geom	FX_StoneWall_02
		Part1	CustomizeablePowers\EarthControl\EarthCrack01.part
		Part2	CustomizeablePowers\EarthControl\EarthDust.part
		Part3	CustomizeablePowers\EarthControl\EarthDust2.part
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
		Bhvr	CustomizeablePowers\EarthControl\GraspingHands2.bhvr
		geom	FX_StoneWall_08
		Part1	CustomizeablePowers\EarthControl\EarthCrack01.part
		Part2	CustomizeablePowers\EarthControl\EarthDust.part
		Part3	CustomizeablePowers\EarthControl\EarthDust2.part
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
		Bhvr	CustomizeablePowers\EarthControl\GraspingHands1.bhvr
		geom	FX_StoneWall_11
		Part1	CustomizeablePowers\EarthControl\EarthCrack01.part
		Part2	CustomizeablePowers\EarthControl\EarthDust.part
		Part3	CustomizeablePowers\EarthControl\EarthDust2.part
	End

	Event
		EName 	Stonesickle04
		Type	local
		At	dum
		AltPiv	4
		Bhvr	CustomizeablePowers\EarthControl\GraspingHands2.bhvr
		geom	FX_StoneWall_04
		Part1	CustomizeablePowers\EarthControl\EarthCrack01.part
		Part2	CustomizeablePowers\EarthControl\EarthDust.part
		Part3	CustomizeablePowers\EarthControl\EarthDust2.part
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
		Bhvr	CustomizeablePowers\EarthControl\GraspingHands1.bhvr
		geom	FX_StoneWall_03
		Part1	CustomizeablePowers\EarthControl\EarthCrack01.part
		Part2	CustomizeablePowers\EarthControl\EarthDust.part
		Part3	CustomizeablePowers\EarthControl\EarthDust2.part
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
		Bhvr	CustomizeablePowers\EarthControl\GraspingHands2.bhvr
		geom	FX_StoneWall_06
		Part1	CustomizeablePowers\EarthControl\EarthCrack01.part
		Part2	CustomizeablePowers\EarthControl\EarthDust.part
		Part3	CustomizeablePowers\EarthControl\EarthDust2.part
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
		Bhvr	CustomizeablePowers\EarthControl\GraspingHands1.bhvr
		geom	FX_StoneWall_10
		Part1	CustomizeablePowers\EarthControl\EarthCrack01.part
		Part2	CustomizeablePowers\EarthControl\EarthDust.part
		Part3	CustomizeablePowers\EarthControl\EarthDust2.part
	End
End

## SPLAT - CRACKS ON GROUND ###########################################################

Condition
	On 	Time
	Time 	7

	Event
		EName 	Stonesickle05
		Type	Create
		Inherit	ALL
		Update	Position
		At	Root
		Bhvr	CustomizeablePowers\EarthControl\crackScale2.bhvr
		geom	Crack01
		Sound explohit 100 100 .75
	End
End

#############################################################

End