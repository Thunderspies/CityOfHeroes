#########################################################
##	jack
##
FxInfo


Input  
	InpName	Root
End

Flags    InheritAlpha


Condition
	On 	Time
	Time 	0

	Event
		EName 	Crystal
		Type	posit 
		At	Root
		Bhvr	Behaviors/RockScaleSmall2.bhvr
		Geom	MultiCrystals
	End

	Event
		Type	posit 
		At	Root
		Part	:CrystalglowFlat.part
		Part	:CrystalglowRing.part
		Part	:CrystalglowRing2.part
		Part	:CrystalglowRingBase.part
	End

End

Condition
	On 	cycle
	Time 	20
	Chance	.1

	Event
		
		Type	posit 
		At	Crystal
		altpiv	1
		
		part1	:FortitudeRaysSmall.part
		part2	:FortitudeRing.part
		part3	:FortitudeRing.part
		part4	:FortitudeDots.part

		lifespan	25
	End

End

Condition
	On 	cycle
	Time 	33
	Chance	.1

	Event
		
		Type	posit 
		At	Crystal
		altpiv	2
		part1	:FortitudeRaysSmall.part
		part2	:FortitudeRing.part
		part3	:FortitudeRing.part
		part4	:FortitudeDots.part

		lifespan	25
	End
End

Condition
	On 	cycle
	Time 	26
	Chance	.1

	Event
		
		Type	posit 
		At	Crystal
		altpiv	3
		part1	:FortitudeRaysSmall.part
		part2	:FortitudeRing.part
		part3	:FortitudeRing.part
		part4	:FortitudeDots.part

		lifespan	25
	End
End

Condition
	On 	cycle
	Time 	31
	Chance	.1

	Event
		
		Type	posit 
		At	Crystal
		altpiv	4
		part1	:FortitudeRaysSmall.part
		part2	:FortitudeRing.part
		part3	:FortitudeRing.part
		part4	:FortitudeDots.part

		lifespan	25
	End
End

Condition
	On 	cycle
	Time 	20
	Chance	.1

	Event
		
		Type	posit 
		At	Crystal
		altpiv	5
		part1	:FortitudeRaysSmall.part
		part2	:FortitudeRing.part
		part3	:FortitudeRing.part
		part4	:FortitudeDots.part

		lifespan	25
	End

End

Condition
	On 	cycle
	Time 	25
	Chance	.1

	Event
		
		Type	posit 
		At	Crystal
		altpiv	6
		part1	:FortitudeRaysSmall.part
		part2	:FortitudeRing.part
		part3	:FortitudeRing.part
		part4	:FortitudeDots.part

		lifespan	25
	End
End

Condition
	On 	cycle
	Time 	34
	Chance	.1

	Event
		
		Type	posit 
		At	Crystal
		altpiv	7
		part1	:FortitudeRaysSmall.part
		part2	:FortitudeRing.part
		part3	:FortitudeRing.part
		part4	:FortitudeDots.part

		lifespan	25
	End

End

Condition
	On 	cycle
	Time 	20
	Chance	.1

	Event
		
		Type	posit 
		At	Crystal
		altpiv	8
		part1	:FortitudeRaysSmall.part
		part2	:FortitudeRing.part
		part3	:FortitudeRing.part
		part4	:FortitudeDots.part

		lifespan	25
	End
End

Condition
	On 	cycle
	Time 	28
	Chance	.1

	Event
		
		Type	posit 
		At	Crystal
		altpiv	9
		part1	:FortitudeRaysSmall.part
		part2	:FortitudeRing.part
		part3	:FortitudeRing.part
		part4	:FortitudeDots.part

		lifespan	25
	End
End

Condition
	On 	cycle
	Time 	24
	Chance	.1
	
	Event
		
		Type	posit 
		At	Crystal
		altpiv	10
		part1	:FortitudeRaysSmall.part
		part2	:FortitudeRing.part
		part3	:FortitudeRing.part
		part4	:FortitudeDots.part

		lifespan	25
	End
End

Condition
	On 	cycle
	Time 	27
	Chance	.1
	
	Event
		
		Type	posit 
		At	Crystal
		altpiv	11
		part1	:FortitudeRaysSmall.part
		part2	:FortitudeRing.part
		part3	:FortitudeRing.part
		part4	:FortitudeDots.part

		lifespan	25
	End
End

Condition
	On 	cycle
	Time 	25
	Chance	.1
	
	Event
		
		Type	posit 
		At	Crystal
		altpiv	12
		part1	:FortitudeRaysSmall.part
		part2	:FortitudeRing.part
		part3	:FortitudeRing.part
		part4	:FortitudeDots.part

		lifespan	25
	End
End

Condition
	On 	cycle
	Time 	33
	Chance	.1
	
	Event
		
		Type	posit 
		At	Crystal
		altpiv	13
		part1	:FortitudeRaysSmall.part
		part2	:FortitudeRing.part
		part3	:FortitudeRing.part
		part4	:FortitudeDots.part

		lifespan	25
	End
End

Condition
	On 	cycle
	Time 	35
	Chance	.1
	
	Event
		
		Type	posit 
		At	Crystal
		altpiv	14
		part1	:FortitudeRaysSmall.part
		part2	:FortitudeRing.part
		part3	:FortitudeRing.part
		part4	:FortitudeDots.part

		lifespan	25
	End
End

Condition
	On 	cycle
	Time 	22
	Chance	.1
	
	Event
		
		Type	posit 
		At	Crystal
		altpiv	15
		part1	:FortitudeRaysSmall.part
		part2	:FortitudeRing.part
		part3	:FortitudeRing.part
		part4	:FortitudeDots.part

		lifespan	25
	End
End

End			