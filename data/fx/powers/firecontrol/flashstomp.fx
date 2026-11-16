#########################################################
##	FireAura
FxInfo
lifespan	240

Input  
	InpName	Origin
End

Input  
	InpName	Target
End

Input  
	InpName	T_Root
End

Input  
	InpName	FootL
End

Input  
	InpName	FootR
End

################################################################

Condition
	On	Time
	Time	0

	Event
		EName	InitialSwordFire
		Type	Local 
		At	FootR
		Part1	:FireHandz.part
		#Sound	Firehand_loop 100.0 30.0 .45
	
		LifeSpan	65
	End


	Event
		EName	InitialSwordFire1
		Type	Local
		At	FootL
		Part1	:FireHandz.part
		#Sound	Ignite2 100.0 30.0 .8
	
		LifeSpan	65
	End

End

Condition
	On	Time
	Time	0

	Event
		EName   RadiusFxScale
		Type	local
		At	T_Root
		Bhvr	:Inferno.bhvr
		Sound Hotignite3 88 88 .83
	End
	
	Event
		EName 	Prime1
		Type	Local 
		At	RadiusFxScale
		bhvr    behaviors/powers/firecontrol/Infernobubble.bhvr
		Part1	powers/firecontrol/InfernoYellow2.part
		Part2	powers/firecontrol/InfernoYellowFlame01.part
		
	End



################################################################

	Event
		EName	Pivot1
		Type	start
		At	T_root
		#Part2	:HotfeetRing4.part
		Geom	HotFeet01
		
	End
	
	Event
		EName	Ring
		Type	start
		At	T_root
		Part2	:HotfeetRing4.part
				
		LifeSpan	140
	End

End

Condition
	On 	Time
	Time 	0
	Event
		EName 	Prime8
		Type	Local 
		At	RadiusFxScale
		Part1   :FlashFireRing.part

	End
End

Condition
	On 	cycle
	Time 	17
	Chance	.5
	Event
		EName	Fire3
		Type	Local
		At	Pivot1
		altpiv	3
		Part1	:Smallfire_flame_A.part
		Part2	:Smallfire_flame_B.part
		Part3	:Smallfire_flame_Flat.part
		Part4	:spark1.part
		##part5	WORLD/Ruins/Smallfire_Smoke3.part

		Lifespan	10

	End

End


Condition
	On 	cycle
	Time 	13
	Chance	.5

	Event
		EName	Fire4
		Type	Local
		At	Pivot1
		altpiv	4
		Part1	:Smallfire_flame_A.part
		Part2	:Smallfire_flame_B.part
		Part3	:Smallfire_flame_Flat.part
		Part4	:spark1.part
		part5	WORLD/Ruins/Smallfire_Smoke3.part

		Lifespan	8

	End

End

Condition
	On 	cycle
	Time 	15
	Chance	.5

	Event
		EName	Fire8
		Type	Local
		At	Pivot1
		altpiv	8
		Part1	:Smallfire_flame_A.part
		Part2	:Smallfire_flame_B.part
		Part3	:Smallfire_flame_Flat.part
		Part4	:spark1.part
		##part5	WORLD/Ruins/Smallfire_Smoke3.part

		Lifespan	10

	End

End


Condition
	On 	cycle
	Time 	14
	Chance	.5

	Event
		EName	Fire9
		Type	Local
		At	Pivot1
		altpiv	9
		Part1	:Smallfire_flame_A.part
		Part2	:Smallfire_flame_B.part
		Part3	:Smallfire_flame_Flat.part
		Part4	:spark1.part
		part5	WORLD/Ruins/Smallfire_Smoke3.part

		Lifespan	9

	End

End


Condition
	On 	cycle
	Time 	18
	Chance	.5

	Event
		EName	Fire10
		Type	Local
		At	Pivot1
		altpiv	10
		Part1	:Smallfire_flame_A.part
		Part2	:Smallfire_flame_B.part
		Part3	:Smallfire_flame_Flat.part
		Part4	:spark1.part
		##part5	WORLD/Ruins/Smallfire_Smoke3.part

		Lifespan	8

	End

End


Condition
	On 	cycle
	Time 	10
	Chance	.5

	Event
		EName	Fire11
		Type	Local
		At	Pivot1
		altpiv	11

		Part1	:Smallfire_flame_A.part
		Part2	:Smallfire_flame_B.part
		Part3	:Smallfire_flame_Flat.part
		Part4	:spark1.part
		part5	WORLD/Ruins/Smallfire_Smoke3.part
			
		Lifespan	7		

	End

End

################################################################
Condition
	On	Time
	Time	0
	Event
		EName	Pivot
		Type	start
		At	T_root
		
		Part4	:aHotFeetBaseFlat.part
		part5	:aHotFeetBaseFlat2.part
		part3	:aHotFeetBaseFlat3.part
		
		Geom	HotFeet
		
	End

End

Condition
	On 	cycle
	Time 	20
	Chance	.5

	Event
		EName	Fire2
		Type	Local
		At	Pivot
		altpiv	1
		
		Part1	:Smallfire_flame_A.part
		Part2	:Smallfire_flame_B.part
		Part3	:Smallfire_flame_Flat.part
		Part4	:spark1.part
		part5	WORLD/Ruins/Smallfire_Smoke3.part

		Lifespan	9


	End

End

Condition
	On 	cycle
	Time 	22
	Chance	.5

	Event
		EName	Fire2
		Type	Local
		At	Pivot
		altpiv	2
		
		Part1	:Smallfire_flame_A.part
		Part2	:Smallfire_flame_B.part
		Part3	:Smallfire_flame_Flat.part
		Part4	:spark1.part
		part5	WORLD/Ruins/Smallfire_Smoke3.part

		Lifespan	8


	End

End


Condition
	On 	cycle
	Time 	17
	Chance	.5
	Event
		EName	Fire3
		Type	Local
		At	Pivot
		altpiv	3
		Part1	:Smallfire_flame_A.part
		Part2	:Smallfire_flame_B.part
		Part3	:Smallfire_flame_Flat.part
		Part4	:spark1.part
		#part5	WORLD/Ruins/Smallfire_Smoke3.part

		Lifespan	9

	End

End


Condition
	On 	cycle
	Time 	15
	Chance	.5

	Event
		EName	Fire4
		Type	Local
		At	Pivot
		altpiv	4
		Part1	:Smallfire_flame_A.part
		Part2	:Smallfire_flame_B.part
		Part3	:Smallfire_flame_Flat.part
		Part4	:spark1.part
		part5	WORLD/Ruins/Smallfire_Smoke3.part

		Lifespan	8

	End

End


Condition
	On 	cycle
	Time 	20
	Chance	.5
	Event
		EName	Fire5
		Type	Local
		At	Pivot
		altpiv	5
		Part1	:Smallfire_flame_A.part
		Part2	:Smallfire_flame_B.part
		Part3	:Smallfire_flame_Flat.part
		Part4	:spark1.part
		#part5	WORLD/Ruins/Smallfire_Smoke3.part

		Lifespan	10

	End

End


Condition
	On 	cycle
	Time 	23
	Chance	.5

	Event
		EName	Fire6
		Type	Local
		At	Pivot
		altpiv	6
		Part1	:Smallfire_flame_A.part
		Part2	:Smallfire_flame_B.part
		Part3	:Smallfire_flame_Flat.part
		Part4	:spark1.part
		#part5	WORLD/Ruins/Smallfire_Smoke3.part

		Lifespan	8

	End

End


Condition
	On 	cycle
	Time 	18
	Chance	.5

	Event
		EName	Fire7
		Type	Local
		At	Pivot
		altpiv	7
		Part1	:Smallfire_flame_A.part
		Part2	:Smallfire_flame_B.part
		Part3	:Smallfire_flame_Flat.part
		Part4	:spark1.part
		part5	WORLD/Ruins/Smallfire_Smoke3.part

		Lifespan	9

	End

End


Condition
	On 	cycle
	Time 	21
	Chance	.5

	Event
		EName	Fire8
		Type	Local
		At	Pivot
		altpiv	8
		Part1	:Smallfire_flame_A.part
		Part2	:Smallfire_flame_B.part
		Part3	:Smallfire_flame_Flat.part
		Part4	:spark1.part
		#part5	WORLD/Ruins/Smallfire_Smoke3.part

		Lifespan	8

	End

End


Condition
	On 	cycle
	Time 	19
	Chance	.5

	Event
		EName	Fire9
		Type	Local
		At	Pivot
		altpiv	9
		Part1	:Smallfire_flame_A.part
		Part2	:Smallfire_flame_B.part
		Part3	:Smallfire_flame_Flat.part
		Part4	:spark1.part
		part5	WORLD/Ruins/Smallfire_Smoke3.part

		Lifespan	10

	End

End


Condition
	On 	cycle
	Time 	18
	Chance	.5

	Event
		EName	Fire10
		Type	Local
		At	Pivot
		altpiv	10
		Part1	:Smallfire_flame_A.part
		Part2	:Smallfire_flame_B.part
		Part3	:Smallfire_flame_Flat.part
		Part4	:spark1.part
		part5	WORLD/Ruins/Smallfire_Smoke3.part

		Lifespan	8

	End

End


Condition
	On 	cycle
	Time 	20
	Chance	.5

	Event
		EName	Fire11
		Type	Local
		At	Pivot
		altpiv	11

		Part1	:Smallfire_flame_A.part
		Part2	:Smallfire_flame_B.part
		Part3	:Smallfire_flame_Flat.part
		Part4	:spark1.part
		#part5	WORLD/Ruins/Smallfire_Smoke3.part
			
		Lifespan	9		

	End

End

Condition
	On 	cycle
	Time 	21
	Chance	.5

	Event
		EName	Fire11
		Type	Local
		At	Pivot
		altpiv	12

		Part1	:Smallfire_flame_A.part
		Part2	:Smallfire_flame_B.part
		Part3	:Smallfire_flame_Flat.part
		Part4	:spark1.part
		#part5	WORLD/Ruins/Smallfire_Smoke3.part
			
		Lifespan	10		

	End



End

Condition
	On 	cycle
	Time 	26
	Chance	.5

	Event
		EName	Fire11
		Type	Local
		At	Pivot
		altpiv	13

		Part1	:Smallfire_flame_A.part
		Part2	:Smallfire_flame_B.part
		Part3	:Smallfire_flame_Flat.part
		Part4	:spark1.part
		#part5	WORLD/Ruins/Smallfire_Smoke3.part
			
		Lifespan	9		

	End



End

Condition
	On 	cycle
	Time 	14
	Chance	.5

	Event
		EName	Fire11
		Type	Local
		At	Pivot
		altpiv	14

		Part1	:Smallfire_flame_A.part
		Part2	:Smallfire_flame_B.part
		Part3	:Smallfire_flame_Flat.part
		Part4	:spark1.part
		#part5	WORLD/Ruins/Smallfire_Smoke3.part
			
		Lifespan	8		

	End



End

End				