#########################################################
##	FireAura
FxInfo
lifespan	240

Input  
	InpName	Root
End

Input  
	InpName	T_Root
End

Input  
	InpName	WepL
End

Input  
	InpName	WepR
End

Input  
	InpName	mystic
End

Input  
	InpName	footR
End

################################################################

Condition
	On	Time
	Time	0

	Event
		EName	InitialSwordFire
		Type	Local 
		At	WepR
		Part1	:FireHandz.part
		
	
		LifeSpan	65
	End


	Event
		EName	InitialSwordFire1
		Type	Local
		At	WepL
		Part1	:FireHandz.part
		
	
		LifeSpan	65
	End
	
	Event
		EName	Pivot
		Type	start
		At	T_root
		
		Geom	HotFeet
		
	End

	Event
		EName	Pivot1
		Type	start
		At	T_root
	
		Geom	HotFeet01
		
	End
End

Condition
	On	Time
	Time	23

	Event
		EName   RadiusFxScale
		Type	local
		At	mystic  
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

	Event
		Ename	ExplosiveForce
		Type	Start
		At	T_Root
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		50
			PhysForcePower		400
			PhysForcePowerJitter	100
		End

		Lifespan	1

	End


################################################################

	
	
	Event
		EName	Ring
		Type	start
		At	mystic
		Part2	:HotfeetRing4.part
				
		LifeSpan	140
	End

End

Condition
	On 	Time
	Time 	23
	Event
		EName 	Prime8
		Type	Local 
		At	RadiusFxScale
		Part1   :FlashFireRing.part

	End
End

Condition
	On 	cycle
	Time 	27
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
	Time 	23
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
	Time 	25
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
	Time 	24
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
	Time 	28
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
	Time 	20
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
#################################################################################
################################################################

Condition
	On	Time
	Time	0
	Event
		EName	hook
		Type	local
		At	mystic
		Bhvr	Behaviors/FootStompBehemoth.bhvr
#		Geom	FootHookup
		
	End

End

Condition
	On	Time
	Time	15
	
	Event
		EName	P2
		Type	start
		At	hook
#		altpiv	1
		Bhvr	Behaviors/CameraShake01.bhvr
		Part4	:aHotFeetBaseFlatx.part
		part5	:aHotFeetBaseFlat2.part
		#part3	:aHotFeetBaseFlat3x.part
		
	End

End
###############################################
Condition
	On 	cycle
	Time 	30
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
	Time 	32
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
	Time 	37
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
	Time 	35
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
	Time 	30
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
	Time 	33
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
	Time 	38
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
	Time 	31
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
	Time 	39
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
	Time 	38
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
	Time 	30
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
	Time 	31
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
	Time 	36
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
	Time 	34
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