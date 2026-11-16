#########################################################
##	chill touch hit

FxInfo

#LifeSpan	250

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#

############################################################
## Splat Glow
############################################################

#Condition
#	On 	Time
#	Time 	45
#
#	Event	
#		ENAME	ChokingCloudSplat
#		Type	Posit 
#		At	Root
#		
#		Bhvr	:ChokingCloudGlow.bhvr
#		Splat	StormCloudGlow.tga
#
#		#LifeSpan 900	
#	End
#End


############################################################
## Hands
############################################################

Condition
	On 	Time
	Time 	5

	Event
		EName	Hand1
		Type	Local 
		At	WepR
		Part1	:RadiationInfectOthersHands.part
		Part2	:RadiationHandElectrons.part
		Part3	:InfectBubbleDotsHands.part
		Part4	:InfectBubblesHands.part
		PMagnet	WepR
		Sound radinfect1 66 66 .75
		
		LifeSpan	100
	End

	Event
		EName	Hand2
		Type	Local 
		At	WepL
		Part1	:RadiationInfectOthersHands.part
		Part2	:RadiationHandElectrons.part
		Part3	:InfectBubbleDotsHands.part
		Part4	:InfectBubblesHands.part
		PMagnet	WepL
		
		LifeSpan	100
	End
End

############################################################
## OutsideRing
############################################################

Condition
	On 	Time
	Time 	60

	Event
		EName 	Ring
		Type	Posit 
		At	Root
		Part1	:Yellow1.part
		Part2	:Yellow2.part
		#Part4	:PoisonBubbles.part
		Part5	:PoisonBubblesYellow.part
		Sound Tar_loop 66 66 .64
	End

	Event
		EName 	Ring
		Type	Posit 
		At	Root
		Part1	:PoisonOutsideRings.part
		Part2	:PoisonOutsideGlows.part
		Sound rumble_loop 66 66 .36
			
	End
End

############################################################
## HotFeet Hookup
######################################################################

Condition
	On 	Time
	Time 	60

	Event
		EName	Pivot1
		Type	local
		At	root
		Geom	HotFeet01
	End
End

Condition
	On 	cycle
	Time 	17
	Chance	0.4
	Event
		EName	Fire3
		Type	Local
		At	Pivot1
		altpiv	3
		Part1	:Bubble_A.part
		Part2	:Bubble_B.part
		#Part3	:Smallfire_flame_Flat.part
		Part4	:spark1.part
		##part5	WORLD/Ruins/Smallfire_Smoke3.part

		Lifespan	10
	End
End


Condition
	On 	cycle
	Time 	13
	Chance	0.4

	Event
		EName	Fire4
		Type	Local
		At	Pivot1
		altpiv	4
		Part1	:Bubble_A.part
		Part2	:Bubble_B.part
		#Part3	:Smallfire_flame_Flat.part
		Part4	:spark1.part
		##part5	WORLD/Ruins/Smallfire_Smoke3.part

		Lifespan	8
	End
End

Condition
	On 	cycle
	Time 	15
	Chance	0.4

	Event
		EName	Fire8
		Type	Local
		At	Pivot1
		altpiv	8
		Part1	:Bubble_A.part
		Part2	:Bubble_B.part
		#Part3	:Smallfire_flame_Flat.part
		Part4	:spark1.part
		##part5	WORLD/Ruins/Smallfire_Smoke3.part

		Lifespan	10
	End
End


Condition
	On 	cycle
	Time 	14
	Chance	0.4

	Event
		EName	Fire9
		Type	Local
		At	Pivot1
		altpiv	9
		Part1	:Bubble_A.part
		Part2	:Bubble_B.part
		#Part3	:Smallfire_flame_Flat.part
		Part4	:spark1.part
		##part5	WORLD/Ruins/Smallfire_Smoke3.part

		Lifespan	9
	End
End


Condition
	On 	cycle
	Time 	18
	Chance	0.4

	Event
		EName	Fire10
		Type	Local
		At	Pivot1
		altpiv	10
		Part1	:Bubble_A.part
		Part2	:Bubble_B.part
		#Part3	:Smallfire_flame_Flat.part
		Part4	:spark1.part
		##part5	WORLD/Ruins/Smallfire_Smoke3.partt

		Lifespan	8
	End
End


Condition
	On 	cycle
	Time 	10
	Chance	0.4

	Event
		EName	Fire11
		Type	Local
		At	Pivot1
		altpiv	11

		Part1	:Bubble_A.part
		Part2	:Bubble_B.part
		Part4	:spark1.part
			
		Lifespan	7		
	End
End


################################################################

Condition
	On	Time
	Time	60
	Event
		EName	Pivot2
		Type	local
		At	root
		
		Geom	HotFeet
	End
End

Condition
	On 	cycle
	Time 	20
	Chance	0.4

	Event
		EName	Fire2
		Type	Local
		At	Pivot2
		altpiv	1
		
		Part1	:Bubble_A.part
		Part2	:Bubble_B.part
		#Part3	:Smallfire_flame_Flat.part
		Part4	:spark1.part
		##part5	WORLD/Ruins/Smallfire_Smoke3.part

		Lifespan	9
	End
End

Condition
	On 	cycle
	Time 	22
	Chance	0.4

	Event
		EName	Fire2
		Type	Local
		At	Pivot2
		altpiv	2
		
		Part1	:Bubble_A.part
		Part2	:Bubble_B.part
		#Part3	:Smallfire_flame_Flat.part
		Part4	:spark1.part
		##part5	WORLD/Ruins/Smallfire_Smoke3.part

		Lifespan	8
	End
End


Condition
	On 	cycle
	Time 	17
	Chance	0.4
	Event
		EName	Fire3
		Type	Local
		At	Pivot2
		altpiv	3
		Part1	:Bubble_A.part
		Part2	:Bubble_B.part
		#Part3	:Smallfire_flame_Flat.part
		Part4	:spark1.part
		##part5	WORLD/Ruins/Smallfire_Smoke3.part

		Lifespan	9
	End
End


Condition
	On 	cycle
	Time 	15
	Chance	0.4

	Event
		EName	Fire4
		Type	Local
		At	Pivot2
		altpiv	4
		Part1	:Bubble_A.part
		Part2	:Bubble_B.part
		#Part3	:Smallfire_flame_Flat.part
		Part4	:spark1.part
		##part5	WORLD/Ruins/Smallfire_Smoke3.part

		Lifespan	8
	End
End


Condition
	On 	cycle
	Time 	20
	Chance	0.4
	Event
		EName	Fire5
		Type	Local
		At	Pivot2
		altpiv	5
		Part1	:Bubble_A.part
		Part2	:Bubble_B.part
		#Part3	:Smallfire_flame_Flat.part
		Part4	:spark1.part
		##part5	WORLD/Ruins/Smallfire_Smoke3.part
		Lifespan	10
	End
End


Condition
	On 	cycle
	Time 	23
	Chance	0.4

	Event
		EName	Fire6
		Type	Local
		At	Pivot2
		altpiv	6
		Part1	:Bubble_A.part
		Part2	:Bubble_B.part
		#Part3	:Smallfire_flame_Flat.part
		Part4	:spark1.part
		##part5	WORLD/Ruins/Smallfire_Smoke3.part

		Lifespan	8
	End
End


Condition
	On 	cycle
	Time 	18
	Chance	0.4

	Event
		EName	Fire7
		Type	Local
		At	Pivot2
		altpiv	7
		Part1	:Bubble_A.part
		Part2	:Bubble_B.part
		#Part3	:Smallfire_flame_Flat.part
		Part4	:spark1.part
		##part5	WORLD/Ruins/Smallfire_Smoke3.part

		Lifespan	9
	End
End


Condition
	On 	cycle
	Time 	21
	Chance	0.4

	Event
		EName	Fire8
		Type	Local
		At	Pivot2
		altpiv	8
		Part1	:Bubble_A.part
		Part2	:Bubble_B.part
		#Part3	:Smallfire_flame_Flat.part
		Part4	:spark1.part
		##part5	WORLD/Ruins/Smallfire_Smoke3.part

		Lifespan	8
	End
End


Condition
	On 	cycle
	Time 	19
	Chance	0.4

	Event
		EName	Fire9
		Type	Local
		At	Pivot2
		altpiv	9
		Part1	:Bubble_A.part
		Part2	:Bubble_B.part
		#Part3	:Smallfire_flame_Flat.part
		Part4	:spark1.part
		##part5	WORLD/Ruins/Smallfire_Smoke3.part

		Lifespan	10
	End
End


Condition
	On 	cycle
	Time 	18
	Chance	0.4

	Event
		EName	Fire10
		Type	Local
		At	Pivot2
		altpiv	10
		Part1	:Bubble_A.part
		Part2	:Bubble_B.part
		#Part3	:Smallfire_flame_Flat.part
		Part4	:spark1.part
		##part5	WORLD/Ruins/Smallfire_Smoke3.part

		Lifespan	8
	End
End


Condition
	On 	cycle
	Time 	20
	Chance	0.4

	Event
		EName	Fire11
		Type	Local
		At	Pivot2
		altpiv	11

		Part1	:Bubble_A.part
		Part2	:Bubble_B.part
		#Part3	:Smallfire_flame_Flat.part
		Part4	:spark1.part
		##part5	WORLD/Ruins/Smallfire_Smoke3.part
			
		Lifespan	9		
	End
End

Condition
	On 	cycle
	Time 	21
	Chance	0.4

	Event
		EName	Fire11
		Type	Local
		At	Pivot2
		altpiv	12

		Part1	:Bubble_A.part
		Part2	:Bubble_B.part
		#Part3	:Smallfire_flame_Flat.part
		Part4	:spark1.part
		##part5	WORLD/Ruins/Smallfire_Smoke3.part
			
		Lifespan	10
	End
End


Condition
	On 	cycle
	Time 	26
	Chance	0.4

	Event
		EName	Fire11
		Type	Local
		At	Pivot2
		altpiv	13

		Part1	:Bubble_A.part
		Part2	:Bubble_B.part
		#Part3	:Smallfire_flame_Flat.part
		Part4	:spark1.part
		##part5	WORLD/Ruins/Smallfire_Smoke3.part
			
		Lifespan	9
	End
End


Condition
	On 	cycle
	Time 	14
	Chance	0.4

	Event
		EName	Fire11
		Type	Local
		At	Pivot2
		altpiv	14

		Part1	:Bubble_A.part
		Part2	:Bubble_B.part
		#Part3	:Smallfire_flame_Flat.part
		Part4	:spark1.part
		##part5	WORLD/Ruins/Smallfire_Smoke3.part
			
		Lifespan	8
	End
End


#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#

End