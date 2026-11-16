#########################################################
##	FireAura
FxInfo

Condition

	Event
		EName	Fire14
		Type	start
		At	origin
		
#		Part	:SmallSpecks.part
#		Part	:SmallSpecks2.part
		Part	:Clay_ADifuse.part
#		Part	:Clay_Flat2.part
#		Part	:Clay_Flat.part
		Part1	:Clay_a2.part
		Part2	:Clay_a.part
		Part3	:Clay_B.part
		Part4	:Clay_C.part
		Lifespan	9
		Geom	CenterDummy

	End


	Event
		Type	start
		At	Fire14
		altpiv	1
		
		Bhvr	:MudPotsSplatCoreMud.bhvr
		splat	smoke.tga
		LifeSpan	45
				
	End

	Event
		Type	start
		At	Fire14
		altpiv	1
		
		Bhvr	:MudPotsSplatCoreMudLight.bhvr
		splat	smoke.tga
		LifeSpan	45
				
	End

End

#######################################################################################

Condition
	On 	cycle
	Time 	5
	Chance	.8

	Event
		EName	Fire2
		Type	start
		At	Fire14
		altpiv	1
		
		
		Bhvr	:MudPotsSplatCoreBubblesLight.bhvr
		splat	BubbleMud.tga
		
		Lifespan	8
		LifeSpanJitter	3


	End

End

Condition
	On 	cycle
	Time 	5
	Chance	.9

	Event
		EName	Fire2
		Type	start
		At	Fire14
		altpiv	1
		
		
		Bhvr	:MudPotsSplatCoreBubblesLight.bhvr
		splat	BubbleMud.tga		
		
		Lifespan	8
		LifeSpanJitter	3


	End

End

Condition
	On 	cycle
	Time 	5
	Chance	.7


	Event
		EName	Fire2
		Type	start
		At	Fire14
		altpiv	1
		
		
		Bhvr	:MudPotsSplatCoreBubblesLight.bhvr
		splat	BubbleMud.tga		
		
		Lifespan	8
		LifeSpanJitter	3


	End

End

###################################################################################

Condition
	On 	cycle
	Time 	5
	Chance	.5

	Event
		EName	Fire2
		Type	start
		At	Fire14
		altpiv	1
		#chance	.7
		
		
		Bhvr	:MudPotsSplatCoreBubbles.bhvr
		splat	Generic_ring.tga		
		
		Lifespan	8
		LifeSpanJitter	3


	End

End

Condition
	On 	cycle
	Time 	5
	Chance	.5
	Event
		EName	Fire2
		Type	start
		At	Fire14
		altpiv	1
		#chance	.7
		
		
		Bhvr	:MudPotsSplatCoreBubbles.bhvr
		splat	Generic_ring.tga
	
		
		Lifespan	8
		LifeSpanJitter	3


	End

End

Condition
	On 	cycle
	Time 	5
	Chance	.5
	Event
		EName	Fire2
		Type	start
		At	Fire14
		altpiv	1
		#chance	.7
		
		
		Bhvr	:MudPotsSplatCoreBubbles.bhvr
		splat	Generic_ring.tga

		
		Lifespan	8
		LifeSpanJitter	3


	End

End

End				