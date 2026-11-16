#############################################################
## ChockingCloud.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Pivot1
		Type	local
		At	root
		Geom	HotFeet01
	End
End

Condition
	On 	cycle
	Time 	10
	Chance	0.4

	Event
		Type	Local
		At	Pivot1
		altpiv	11
		Part1	:Bubble_A.part
		Part2	:Bubble_B.part
		Part4	:spark1.part
		Lifespan 7
	End
End

Condition
	On 	cycle
	Time 	13
	Chance	0.4

	Event
		Type	Local
		At	Pivot1
		altpiv	4
		Part1	:Bubble_A.part
		Part2	:Bubble_B.part
		Part4	:spark1.part
		Lifespan 8
	End
End

Condition
	On 	cycle
	Time 	15
	Chance	0.4

	Event
		Type	Local
		At	Pivot1
		altpiv	8
		Part1	:Bubble_A.part
		Part2	:Bubble_B.part
		Part4	:spark1.part
		Lifespan 10
	End
End

Condition
	On 	cycle
	Time 	14
	Chance	0.4

	Event
		Type	Local
		At	Pivot1
		altpiv	9
		Part1	:Bubble_A.part
		Part2	:Bubble_B.part
		Part4	:spark1.part
		Lifespan 9
	End
End

Condition
	On 	cycle
	Time 	17
	Chance	0.4

	Event
		Type	Local
		At	Pivot1
		altpiv	3
		Part1	:Bubble_A.part
		Part2	:Bubble_B.part
		Part4	:spark1.part
		Lifespan 10
	End
End

Condition
	On 	cycle
	Time 	18
	Chance	0.4

	Event
		Type	Local
		At	Pivot1
		altpiv	10
		Part1	:Bubble_A.part
		Part2	:Bubble_B.part
		Part4	:spark1.part
		Lifespan 8
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Pivot2
		Type	local
		At	root
		Geom	HotFeet
	End
End

Condition
	On 	cycle
	Time 	14
	Chance	0.4

	Event
		Type	Local
		At	Pivot2
		altpiv	14
		Part1	:Bubble_A.part
		Part2	:Bubble_B.part
		Part4	:spark1.part
		Lifespan 8
	End
End

Condition
	On 	cycle
	Time 	15
	Chance	0.4

	Event
		Type	Local
		At	Pivot2
		altpiv	4
		Part1	:Bubble_A.part
		Part2	:Bubble_B.part
		Part4	:spark1.part
		Lifespan 8
	End
End

Condition
	On 	cycle
	Time 	17
	Chance	0.4

	Event
		Type	Local
		At	Pivot2
		altpiv	3
		Part1	:Bubble_A.part
		Part2	:Bubble_B.part
		Part4	:spark1.part
		Lifespan 9
	End
End

Condition
	On 	cycle
	Time 	20
	Chance	0.4

	Event
		Type	Local
		At	Pivot2
		altpiv	1
		Part1	:Bubble_A.part
		Part2	:Bubble_B.part
		Part4	:spark1.part
		Lifespan 9
	End
End

Condition
	On 	cycle
	Time 	20
	Chance	0.4

	Event
		Type	Local
		At	Pivot2
		altpiv	5
		Part1	:Bubble_A.part
		Part2	:Bubble_B.part
		Part4	:spark1.part
		Lifespan 10
	End
End

Condition
	On 	cycle
	Time 	22
	Chance	0.4

	Event
		Type	Local
		At	Pivot2
		altpiv	2
		Part1	:Bubble_A.part
		Part2	:Bubble_B.part
		Part4	:spark1.part
		Lifespan 8
	End
End

Condition
	On 	cycle
	Time 	23
	Chance	0.4

	Event
		Type	Local
		At	Pivot2
		altpiv	6
		Part1	:Bubble_A.part
		Part2	:Bubble_B.part
		Part4	:spark1.part
		Lifespan 8
	End
End


Condition
	On 	cycle
	Time 	21
	Chance	0.4

	Event
		Type	Local
		At	Pivot2
		altpiv	8
		Part1	:Bubble_A.part
		Part2	:Bubble_B.part
		Part4	:spark1.part
		Lifespan 8
	End
End

Condition
	On 	cycle
	Time 	18
	Chance	0.4

	Event
		Type	Local
		At	Pivot2
		altpiv	7
		Part1	:Bubble_A.part
		Part2	:Bubble_B.part
		Part4	:spark1.part
		Lifespan 9
	End
End

Condition
	On 	cycle
	Time 	18
	Chance	0.4

	Event
		Type	Local
		At	Pivot2
		altpiv	10
		Part1	:Bubble_A.part
		Part2	:Bubble_B.part
		Part4	:spark1.part
		Lifespan 8
	End
End

Condition
	On 	cycle
	Time 	19
	Chance	0.4

	Event
		Type	Local
		At	Pivot2
		altpiv	9
		Part1	:Bubble_A.part
		Part2	:Bubble_B.part
		Part4	:spark1.part
		Lifespan 10
	End
End

Condition
	On 	cycle
	Time 	20
	Chance	0.4

	Event
		Type	Local
		At	Pivot2
		altpiv	11
		Part1	:Bubble_A.part
		Part2	:Bubble_B.part
		Part4	:spark1.part
		Lifespan 9
	End
End

Condition
	On 	cycle
	Time 	21
	Chance	0.4

	Event
		Type	Local
		At	Pivot2
		altpiv	12
		Part1	:Bubble_A.part
		Part2	:Bubble_B.part
		Part4	:spark1.part
		Lifespan 10
	End
End


Condition
	On 	cycle
	Time 	26
	Chance	0.4

	Event
		Type	Local
		At	Pivot2
		altpiv	13
		Part1	:Bubble_A.part
		Part2	:Bubble_B.part
		Part4	:spark1.part
		Lifespan 9
	End
End

#############################################################

End