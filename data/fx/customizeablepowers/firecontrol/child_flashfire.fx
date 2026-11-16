#############################################################
## Child_Flashfire.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Pivot
		Type	start
		At	Root
		Geom	HotFeet
	End

	Event
		EName	Pivot1
		Type	start
		At	Root
		Geom	HotFeet01
	End
End

#############################################################

Condition
	On 	cycle
	Time 	17
	Chance	0.5
	Event
		EName	Fire3
		Type	Local
		At	Pivot1
		altpiv	3
		Part1	CustomizeablePowers\FireControl\Smallfire_flame_A.part
		Part2	CustomizeablePowers\FireControl\Smallfire_flame_B.part
		Part3	CustomizeablePowers\FireControl\Smallfire_flame_Flat.part
		Part4	CustomizeablePowers\FireControl\spark1.part
		LifeSpan 10
	End
End

Condition
	On 	cycle
	Time 	13
	Chance	0.5

	Event
		EName	Fire4
		Type	Local
		At	Pivot1
		altpiv	4
		Part1	CustomizeablePowers\FireControl\Smallfire_flame_A.part
		Part2	CustomizeablePowers\FireControl\Smallfire_flame_B.part
		Part3	CustomizeablePowers\FireControl\Smallfire_flame_Flat.part
		Part4	CustomizeablePowers\FireControl\spark1.part
		part5	WORLD\Ruins\Smallfire_Smoke3.part
		LifeSpan 8
	End
End

Condition
	On 	cycle
	Time 	15
	Chance	0.5

	Event
		EName	Fire8
		Type	Local
		At	Pivot1
		altpiv	8
		Part1	CustomizeablePowers\FireControl\Smallfire_flame_A.part
		Part2	CustomizeablePowers\FireControl\Smallfire_flame_B.part
		Part3	CustomizeablePowers\FireControl\Smallfire_flame_Flat.part
		Part4	CustomizeablePowers\FireControl\spark1.part
		LifeSpan 10
	End
End

Condition
	On 	cycle
	Time 	14
	Chance	0.5

	Event
		EName	Fire9
		Type	Local
		At	Pivot1
		altpiv	9
		Part1	CustomizeablePowers\FireControl\Smallfire_flame_A.part
		Part2	CustomizeablePowers\FireControl\Smallfire_flame_B.part
		Part3	CustomizeablePowers\FireControl\Smallfire_flame_Flat.part
		Part4	CustomizeablePowers\FireControl\spark1.part
		part5	WORLD\Ruins\Smallfire_Smoke3.part
		LifeSpan 9
	End
End

Condition
	On 	cycle
	Time 	18
	Chance	0.5

	Event
		EName	Fire10
		Type	Local
		At	Pivot1
		altpiv	10
		Part1	CustomizeablePowers\FireControl\Smallfire_flame_A.part
		Part2	CustomizeablePowers\FireControl\Smallfire_flame_B.part
		Part3	CustomizeablePowers\FireControl\Smallfire_flame_Flat.part
		Part4	CustomizeablePowers\FireControl\spark1.part
		LifeSpan 8
	End
End

Condition
	On 	cycle
	Time 	10
	Chance	0.5

	Event
		EName	Fire11
		Type	Local
		At	Pivot1
		altpiv	11
		Part1	CustomizeablePowers\FireControl\Smallfire_flame_A.part
		Part2	CustomizeablePowers\FireControl\Smallfire_flame_B.part
		Part3	CustomizeablePowers\FireControl\Smallfire_flame_Flat.part
		Part4	CustomizeablePowers\FireControl\spark1.part
		part5	WORLD\Ruins\Smallfire_Smoke3.part
		LifeSpan 7
	End
End

#############################################################

Condition
	On 	cycle
	Time 	17
	Chance	0.5

	Event
		EName	Fire3
		Type	Local
		At	Pivot
		altpiv	3
		Part1	CustomizeablePowers\FireControl\Smallfire_flame_A.part
		Part2	CustomizeablePowers\FireControl\Smallfire_flame_B.part
		Part3	CustomizeablePowers\FireControl\Smallfire_flame_Flat.part
		Part4	CustomizeablePowers\FireControl\spark1.part
		Lifespan 10
	End
End

Condition
	On 	cycle
	Time 	13
	Chance	0.5

	Event
		EName	Fire4
		Type	Local
		At	Pivot
		altpiv	4
		Part1	CustomizeablePowers\FireControl\Smallfire_flame_A.part
		Part2	CustomizeablePowers\FireControl\Smallfire_flame_B.part
		Part3	CustomizeablePowers\FireControl\Smallfire_flame_Flat.part
		Part4	CustomizeablePowers\FireControl\spark1.part
		part5	CustomizeablePowers\FireControl\Smallfire_Smoke3.part
		Lifespan 8
	End
End

Condition
	On 	cycle
	Time 	15
	Chance	0.5

	Event
		EName	Fire8
		Type	Local
		At	Pivot
		altpiv	8
		Part1	CustomizeablePowers\FireControl\Smallfire_flame_A.part
		Part2	CustomizeablePowers\FireControl\Smallfire_flame_B.part
		Part3	CustomizeablePowers\FireControl\Smallfire_flame_Flat.part
		Part4	CustomizeablePowers\FireControl\spark1.part
		Lifespan 10
	End
End

Condition
	On 	cycle
	Time 	14
	Chance	0.5

	Event
		EName	Fire9
		Type	Local
		At	Pivot
		altpiv	9
		Part1	CustomizeablePowers\FireControl\Smallfire_flame_A.part
		Part2	CustomizeablePowers\FireControl\Smallfire_flame_B.part
		Part3	CustomizeablePowers\FireControl\Smallfire_flame_Flat.part
		Part4	CustomizeablePowers\FireControl\spark1.part
		part5	CustomizeablePowers\FireControl\Smallfire_Smoke3.part
		Lifespan 9
	End
End

Condition
	On 	cycle
	Time 	18
	Chance	0.5

	Event
		EName	Fire10
		Type	Local
		At	Pivot
		altpiv	10
		Part1	CustomizeablePowers\FireControl\Smallfire_flame_A.part
		Part2	CustomizeablePowers\FireControl\Smallfire_flame_B.part
		Part3	CustomizeablePowers\FireControl\Smallfire_flame_Flat.part
		Part4	CustomizeablePowers\FireControl\spark1.part
		Lifespan 8
	End
End

Condition
	On 	cycle
	Time 	10
	Chance	0.5

	Event
		EName	Fire11
		Type	Local
		At	Pivot
		altpiv	11
		Part1	CustomizeablePowers\FireControl\Smallfire_flame_A.part
		Part2	CustomizeablePowers\FireControl\Smallfire_flame_B.part
		Part3	CustomizeablePowers\FireControl\Smallfire_flame_Flat.part
		Part4	CustomizeablePowers\FireControl\spark1.part
		part5	CustomizeablePowers\FireControl\Smallfire_Smoke3.part
		Lifespan 7
	End
End

Condition
	On 	cycle
	Time 	20
	Chance	0.5

	Event
		EName	Fire2
		Type	Local
		At	Pivot
		altpiv	1
		Part1	CustomizeablePowers\FireControl\Smallfire_flame_A.part
		Part2	CustomizeablePowers\FireControl\Smallfire_flame_B.part
		Part3	CustomizeablePowers\FireControl\Smallfire_flame_Flat.part
		Part4	CustomizeablePowers\FireControl\spark1.part
		part5	CustomizeablePowers\FireControl\Smallfire_Smoke3.part
		Lifespan 9
	End
End

Condition
	On 	cycle
	Time 	22
	Chance	0.5

	Event
		EName	Fire2
		Type	Local
		At	Pivot
		altpiv	2
		Part1	CustomizeablePowers\FireControl\Smallfire_flame_A.part
		Part2	CustomizeablePowers\FireControl\Smallfire_flame_B.part
		Part3	CustomizeablePowers\FireControl\Smallfire_flame_Flat.part
		Part4	CustomizeablePowers\FireControl\spark1.part
		part5	CustomizeablePowers\FireControl\Smallfire_Smoke3.part
		Lifespan 8
	End
End

Condition
	On 	cycle
	Time 	17
	Chance	0.5
	Event
		EName	Fire3
		Type	Local
		At	Pivot
		altpiv	3
		Part1	CustomizeablePowers\FireControl\Smallfire_flame_A.part
		Part2	CustomizeablePowers\FireControl\Smallfire_flame_B.part
		Part3	CustomizeablePowers\FireControl\Smallfire_flame_Flat.part
		Part4	CustomizeablePowers\FireControl\spark1.part
		Lifespan 9
	End
End

Condition
	On 	cycle
	Time 	15
	Chance	0.5

	Event
		EName	Fire4
		Type	Local
		At	Pivot
		altpiv	4
		Part1	CustomizeablePowers\FireControl\Smallfire_flame_A.part
		Part2	CustomizeablePowers\FireControl\Smallfire_flame_B.part
		Part3	CustomizeablePowers\FireControl\Smallfire_flame_Flat.part
		Part4	CustomizeablePowers\FireControl\spark1.part
		part5	CustomizeablePowers\FireControl\Smallfire_Smoke3.part
		Lifespan 8
	End
End

Condition
	On 	cycle
	Time 	20
	Chance	0.5

	Event
		EName	Fire5
		Type	Local
		At	Pivot
		altpiv	5
		Part1	CustomizeablePowers\FireControl\Smallfire_flame_A.part
		Part2	CustomizeablePowers\FireControl\Smallfire_flame_B.part
		Part3	CustomizeablePowers\FireControl\Smallfire_flame_Flat.part
		Part4	CustomizeablePowers\FireControl\spark1.part
		Lifespan 10
	End
End

Condition
	On 	cycle
	Time 	23
	Chance	0.5

	Event
		EName	Fire6
		Type	Local
		At	Pivot
		altpiv	6
		Part1	CustomizeablePowers\FireControl\Smallfire_flame_A.part
		Part2	CustomizeablePowers\FireControl\Smallfire_flame_B.part
		Part3	CustomizeablePowers\FireControl\Smallfire_flame_Flat.part
		Part4	CustomizeablePowers\FireControl\spark1.part
		Lifespan 8
	End
End

Condition
	On 	cycle
	Time 	18
	Chance	0.5

	Event
		EName	Fire7
		Type	Local
		At	Pivot
		altpiv	7
		Part1	CustomizeablePowers\FireControl\Smallfire_flame_A.part
		Part2	CustomizeablePowers\FireControl\Smallfire_flame_B.part
		Part3	CustomizeablePowers\FireControl\Smallfire_flame_Flat.part
		Part4	CustomizeablePowers\FireControl\spark1.part
		part5	CustomizeablePowers\FireControl\Smallfire_Smoke3.part
		Lifespan 9
	End
End

Condition
	On 	cycle
	Time 	21
	Chance	0.5

	Event
		EName	Fire8
		Type	Local
		At	Pivot
		altpiv	8
		Part1	CustomizeablePowers\FireControl\Smallfire_flame_A.part
		Part2	CustomizeablePowers\FireControl\Smallfire_flame_B.part
		Part3	CustomizeablePowers\FireControl\Smallfire_flame_Flat.part
		Part4	CustomizeablePowers\FireControl\spark1.part
		Lifespan 8
	End
End

Condition
	On 	cycle
	Time 	19
	Chance	0.5

	Event
		EName	Fire9
		Type	Local
		At	Pivot
		altpiv	9
		Part1	CustomizeablePowers\FireControl\Smallfire_flame_A.part
		Part2	CustomizeablePowers\FireControl\Smallfire_flame_B.part
		Part3	CustomizeablePowers\FireControl\Smallfire_flame_Flat.part
		Part4	CustomizeablePowers\FireControl\spark1.part
		part5	CustomizeablePowers\FireControl\Smallfire_Smoke3.part
		Lifespan 10
	End
End

Condition
	On 	cycle
	Time 	18
	Chance	0.5

	Event
		EName	Fire10
		Type	Local
		At	Pivot
		altpiv	10
		Part1	CustomizeablePowers\FireControl\Smallfire_flame_A.part
		Part2	CustomizeablePowers\FireControl\Smallfire_flame_B.part
		Part3	CustomizeablePowers\FireControl\Smallfire_flame_Flat.part
		Part4	CustomizeablePowers\FireControl\spark1.part
		part5	CustomizeablePowers\FireControl\Smallfire_Smoke3.part
		Lifespan 8
	End
End

Condition
	On 	cycle
	Time 	20
	Chance	0.5

	Event
		EName	Fire11
		Type	Local
		At	Pivot
		altpiv	11
		Part1	CustomizeablePowers\FireControl\Smallfire_flame_A.part
		Part2	CustomizeablePowers\FireControl\Smallfire_flame_B.part
		Part3	CustomizeablePowers\FireControl\Smallfire_flame_Flat.part
		Part4	CustomizeablePowers\FireControl\spark1.part
		Lifespan 9
	End
End

Condition
	On 	cycle
	Time 	21
	Chance	0.5

	Event
		EName	Fire11
		Type	Local
		At	Pivot
		altpiv	12
		Part1	CustomizeablePowers\FireControl\Smallfire_flame_A.part
		Part2	CustomizeablePowers\FireControl\Smallfire_flame_B.part
		Part3	CustomizeablePowers\FireControl\Smallfire_flame_Flat.part
		Part4	CustomizeablePowers\FireControl\spark1.part
		Lifespan 10
	End
End

Condition
	On 	cycle
	Time 	26
	Chance	0.5

	Event
		EName	Fire11
		Type	Local
		At	Pivot
		altpiv	13
		Part1	CustomizeablePowers\FireControl\Smallfire_flame_A.part
		Part2	CustomizeablePowers\FireControl\Smallfire_flame_B.part
		Part3	CustomizeablePowers\FireControl\Smallfire_flame_Flat.part
		Part4	CustomizeablePowers\FireControl\spark1.part
		Lifespan 9
	End
End

Condition
	On 	cycle
	Time 	14
	Chance	0.5

	Event
		EName	Fire11
		Type	Local
		At	Pivot
		altpiv	14
		Part1	CustomizeablePowers\FireControl\Smallfire_flame_A.part
		Part2	CustomizeablePowers\FireControl\Smallfire_flame_B.part
		Part3	CustomizeablePowers\FireControl\Smallfire_flame_Flat.part
		Part4	CustomizeablePowers\FireControl\spark1.part
		Lifespan 8
	End
End

#############################################################

End