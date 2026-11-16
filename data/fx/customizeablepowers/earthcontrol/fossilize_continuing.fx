#########################################################
##	Stone_Wall
#########################################################

FxInfo

ClampMaxScale 5.0 5.0 5.0

#############################################################

Condition
	On	Time
	Time	5

	Event
		Type	start
		At	Root
		Part2	CustomizeablePowers\EarthControl\FossilDust2.part
		part3	CustomizeablePowers\EarthControl\FossilDust1.part
		Part	CustomizeablePowers\EarthControl\FossilRocks02.part
	End
End

Condition
	On 	Time
	Time 	3
	Repeat	7

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 1 0
			StartJitter	1 0 1
		End
		ChildFX		V_COV\PhysicsEnabled\RockDebrisGoldScatter.fx
		Lifespan	100
	End

	Event
		HardwareOnly
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 1 0
			StartJitter	1 0 1
		End
		ChildFX		V_COV\PhysicsEnabled\RockDebrisGoldExplode.fx
		Lifespan	100
	End
End

Condition
	On 	Time
	Time 	3
	Repeat	7

	Event
		Type	Local
		At	Root

		BhvrOverride
			PositionOffset	0 1 0
			StartJitter	1 0 1
		End
		ChildFX		V_COV\PhysicsEnabled\RockDebrisGrayScatter.fx
		Lifespan	100
	End

	Event
		HardwareOnly
		Type	Local
		At	Root

		BhvrOverride
			PositionOffset	0 1 0
			StartJitter	1 0 1
		End
		ChildFX		V_COV\PhysicsEnabled\RockDebrisGrayScatter.fx
		Lifespan	100
	End
End

#############################################################

Condition
	On 	Time
	Time 	9

	Event
		EName 	Stonesickle05
		Type	start
		At	root
		Bhvr	CustomizeablePowers\EarthControl\FossilcrackScale.bhvr
		geom	Crack01
		Sound explohit 100 100 .95
	End
End

Condition
	On	Time
	Time	10
	Event
		EName	Rocksz
		Type	start
		At	Root
		Bhvr	CustomizeablePowers\EarthControl\FossilScale.bhvr
		Geom	StoneFossil
	End
End

Condition
	On	Time
	Time	9

	Event
		EName	Rocks2
		Type	start
		At	Root
		Part2	CustomizeablePowers\EarthControl\FossilCrack2.part
		Lifespan 51
	End
End

Condition
	On	Time
	Time	8

	Event
		EName	Rocks3
		Type	start
		At	Root
		Part2	CustomizeablePowers\EarthControl\FossilCrack2.part
		Lifespan 55
	End
End

Condition
	On	Time
	Time	11

	Event
		EName	Rocks4
		Type	start
		At	Root
		Part2	CustomizeablePowers\EarthControl\FossilCrack2.part
		Lifespan 52
	End
End

#############################################################

End