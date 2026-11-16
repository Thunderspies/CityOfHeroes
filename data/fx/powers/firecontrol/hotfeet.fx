#########################################################
## Fire Contrcol - Hot Feet
#########################################################

FxInfo

#Lifespan	180

## HANDS #######################################################

Condition
	On	Time
	Time	0

	Event
		EName	InitialSwordFire
		Type	Local 
		At	WepR
		Part1	:FireHandz.part
		Sound Firehand_loop 100.0 100.0 .45
	
		LifeSpan	60
	End

	Event
		EName	InitialSwordFire1
		Type	Local
		At	WepL
		Part1	:FireHandz.part
		Sound Ignite2 100.0 100.0 .8
	
		LifeSpan	60
	End
End

## GROUND FX #######################################################

Condition
	On 	Time
	Time 	0
	
	Event
		Type	Local
		At	Root
		Part1	:HotfeetRing.part		

		LifeSpan	10
	End
End

Condition
	On 	cycle
	Time 	30
	
	Event
		Type	Local
		At	Root
		Part1	:HotfeetRing.part

		LifeSpan	30
	End
End

Condition
	On 	Time
	Time 	25
	
	Event
		EName	splat
		Type	Local
		At	root
		
		Part	:HotFeetBaseFlat.part
		Part	:HotFeetBaseFlat2.part
		
	End
	
End

###################################################################################################

Condition
	On	Time
	Time	30

	Event
		EName	Pivot
		Type	local
		At	root
		Geom	HotFeet
		Sound flameburst_loop 60 60 .39
		bhvr	behaviors/soundFade2.bhvr
		Flags	PowerLoopingSound
		Lifespan 480
		
	End

	Event
		Ename	fireSound
		Type	Local	
		At	root
		Sound fireballfly 60 60 .7
	End
End


########################################################################################################

Condition
	On	Time
	Time	0

	Event
		Ename	FlamesOffset
		Type	Local	
		At	Root
		Geom	HotFeet01
	End
End

Condition
	On 	cycle
	Time 	20
	Chance	0.4

	Event
		EName	Fire2
		Type	start
		At	FlamesOffset
		altpiv	1
		
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
	Time 	22
	Chance	0.4

	Event
		EName	Fire2
		Type	start
		At	FlamesOffset
		altpiv	2
		
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
	Time 	27
	Chance	0.4
	Event
		EName	Fire3
		Type	start
		At	FlamesOffset
		altpiv	3
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
	Time 	25
	Chance	0.4

	Event
		EName	Fire4
		Type	start
		At	FlamesOffset
		altpiv	4
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
	Time 	26
	Chance	0.4
	Event
		EName	Fire5
		Type	start
		At	FlamesOffset
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
	Chance	0.4

	Event
		EName	Fire6
		Type	start
		At	FlamesOffset
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
	Time 	27
	Chance	.4

	Event
		EName	Fire7
		Type	start
		At	FlamesOffset
		altpiv	7
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
	Time 	30
	Chance	0.4

	Event
		EName	Fire8
		Type	start
		At	FlamesOffset
		altpiv	8
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
	Time 	24
	Chance	.4

	Event
		EName	Fire9
		Type	start
		At	FlamesOffset
		altpiv	9
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
	Time 	28
	Chance	0.4

	Event
		EName	Fire10
		Type	start
		At	FlamesOffset
		altpiv	10
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
	Time 	30
	Chance	0.4

	Event
		EName	Fire11
		Type	start
		At	FlamesOffset
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
	Chance	0.4

	Event
		EName	Fire11
		Type	start
		At	FlamesOffset
		altpiv	12

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
	Time 	26
	Chance	0.4

	Event
		EName	Fire11
		Type	start
		At	FlamesOffset
		altpiv	13

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
	Time 	34
	Chance	0.4

	Event
		EName	Fire11
		Type	start
		At	FlamesOffset
		altpiv	14

		Part1	:Smallfire_flame_A.part
		Part2	:Smallfire_flame_B.part
		Part3	:Smallfire_flame_Flat.part
		Part4	:spark1.part
		#part5	WORLD/Ruins/Smallfire_Smoke3.part
			
		Lifespan	9		
	End
End

########################################################################################################

End				