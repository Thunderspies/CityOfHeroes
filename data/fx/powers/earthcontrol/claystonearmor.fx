#########################################################
##	FireAura
FxInfo


Input  
	InpName	Root
End

Input
	Inpname Origin
End

Input  
	InpName	WepR
End

Input
	Inpname WepL
End

################################################################
Condition
	On	Time
	Time	0

	Event
		EName	Pivot
		Type	local
		At	root
		
		Part3	:ClayArmorRings.part
		Part4	:HotFeetBaseFlat.part
		part5	:HotFeetBaseFlat2.part
		Bhvr	behaviors/ScaleClay.bhvr
		Geom	HotFeet
		
	End


End


########################################################################################################



########################################################################################################

Condition
	On	Time
	Time 	0
		
	Event
		Type	Local
		At 	origin
		Sound Stone4 100 100 .9
	End
End

Condition
	On	Time
	Time 	18
		
	Event
		Type	Local
		At 	origin
		Sound mudpots_loop 100 100 .66
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 400
	End
End

Condition
	On 	cycle
	Time 	20
	Chance	.25

	Event
		EName	Fire11
		Type	start
		At	Pivot
		altpiv	1
		
		Part4	:SmallSpecks.part
		Part	:SmallSpecks2.part
		Part	:Clay_ADifuse.part
		Part	:Clay_Flat2.part
		Part3	:Clay_Flat.part	
		Part	:Clay_a2.part
		Part1	:Clay_a.part
		Part2	:Clay_B.part
		Part	:Clay_C.part
		Lifespan	8		

	End

	Event
		EName	Fire2
		Type	start
		At	Pivot
		altpiv	1
		
		
		#Part	:Clay_B2.part
		Part	:Clay_B3.part
		
		
		Lifespan	8


	End

End

Condition
	On 	cycle
	Time 	32
	Chance	.25

	Event
		EName	Fire2
		Type	start
		At	Pivot
		altpiv	2
				
		#Part	:Clay_B2.part
		Part	:Clay_B3.part
		
		
		Lifespan	19


	End

	Event
		EName	Fire11
		Type	start
		At	Pivot
		altpiv	2
		
		Part4	:SmallSpecks.part
		Part	:SmallSpecks2.part
		Part	:Clay_ADifuse.part
		Part	:Clay_Flat2.part
		Part3	:Clay_Flat.part	
		Part	:Clay_a2.part
		Part1	:Clay_a.part
		Part2	:Clay_B.part
		Part	:Clay_C.part
		Lifespan	10		

	End

End


Condition
	On 	cycle
	Time 	37
	Chance	.25
	Event
		EName	Fire3
		Type	start
		At	Pivot
		altpiv	3
		
		#Part	:Clay_B2.part
		Part	:Clay_B3.part
		
		
		Lifespan	18

	End

	Event
		EName	Fire11
		Type	start
		At	Pivot
		altpiv	3
		
		Part4	:SmallSpecks.part
		Part	:SmallSpecks2.part
		Part	:Clay_ADifuse.part
		Part	:Clay_Flat2.part
		Part3	:Clay_Flat.part	
		Part	:Clay_a2.part
		Part1	:Clay_a.part
		Part2	:Clay_B.part
		Part	:Clay_C.part
		Lifespan	9		

	End
End


Condition
	On 	cycle
	Time 	35
	Chance	.25

	Event
		EName	Fire4
		Type	start
		At	Pivot
		altpiv	4
		
		#Part	:Clay_B2.part
		Part	:Clay_B3.part
		
		
		Lifespan	19

	End

	Event
		EName	Fire11
		Type	start
		At	Pivot
		altpiv	4
		
		Part4	:SmallSpecks.part
		Part	:SmallSpecks2.part
		Part	:Clay_ADifuse.part
		Part	:Clay_Flat2.part
		Part3	:Clay_Flat.part	
		Part	:Clay_a2.part
		Part1	:Clay_a.part
		Part2	:Clay_B.part
		Part	:Clay_C.part
		Lifespan	8		

	End

End


Condition
	On 	cycle
	Time 	36
	Chance	.25
	Event
		EName	Fire5
		Type	start
		At	Pivot
		altpiv	5
		
		#Part	:Clay_B2.part
		Part	:Clay_B3.part
		
		
		Lifespan	20

	End

	Event
		EName	Fire11
		Type	start
		At	Pivot
		altpiv	5
		
		Part4	:SmallSpecks.part
		Part	:SmallSpecks2.part
		Part	:Clay_ADifuse.part
		Part	:Clay_Flat2.part
		Part3	:Clay_Flat.part	
		Part	:Clay_a2.part
		Part1	:Clay_a.part
		Part2	:Clay_B.part
		Part	:Clay_C.part
		Lifespan	7		

	End

End


Condition
	On 	cycle
	Time 	43
	Chance	.25

	Event
		EName	Fire6
		Type	start
		At	Pivot
		altpiv	6
		
		#Part	:Clay_B2.part
		Part	:Clay_B3.part
		
		Lifespan	18

	End

	Event
		EName	Fire11
		Type	start
		At	Pivot
		altpiv	6
		
		Part4	:SmallSpecks.part
		Part	:SmallSpecks2.part
		Part	:Clay_ADifuse.part
		Part	:Clay_Flat2.part
		Part3	:Clay_Flat.part	
		Part	:Clay_a2.part
		Part1	:Clay_a.part
		Part2	:Clay_B.part
		Part	:Clay_C.part
		Lifespan	8		

	End

End


Condition
	On 	cycle
	Time 	37
	Chance	.25

	Event
		EName	Fire7
		Type	start
		At	Pivot
		altpiv	7
		
		#Part	:Clay_B2.part
		Part	:Clay_B3.part
		
		
		Lifespan	19

	End

	Event
		EName	Fire11
		Type	start
		At	Pivot
		altpiv	7
		
		Part4	:SmallSpecks.part
		Part	:SmallSpecks2.part
		Part	:Clay_ADifuse.part
		Part	:Clay_Flat2.part
		Part3	:Clay_Flat.part	
		Part	:Clay_a2.part
		Part1	:Clay_a.part
		Part2	:Clay_B.part
		Part	:Clay_C.part
		Lifespan	10		

	End
End


Condition
	On 	cycle
	Time 	40
	Chance	.25

	Event
		EName	Fire8
		Type	start
		At	Pivot
		altpiv	8
		
		#Part	:Clay_B2.part
		Part	:Clay_B3.part
		
		
		Lifespan	20

	End

	Event
		EName	Fire11
		Type	start
		At	Pivot
		altpiv	8
		
		Part4	:SmallSpecks.part
		Part	:SmallSpecks2.part
		Part	:Clay_ADifuse.part
		Part	:Clay_Flat2.part
		Part3	:Clay_Flat.part	
		Part	:Clay_a2.part
		Part1	:Clay_a.part
		Part2	:Clay_B.part
		Part	:Clay_C.part
		Lifespan	9		

	End
End


Condition
	On 	cycle
	Time 	34
	Chance	.25

	Event
		EName	Fire9
		Type	start
		At	Pivot
		altpiv	9
		
		#Part	:Clay_B2.part
		Part	:Clay_B3.part
		
		
		Lifespan	18

	End

	Event
		EName	Fire11
		Type	start
		At	Pivot
		altpiv	9
		
		Part4	:SmallSpecks.part
		Part	:SmallSpecks2.part
		Part	:Clay_ADifuse.part
		Part	:Clay_Flat2.part
		Part3	:Clay_Flat.part	
		Part	:Clay_a2.part
		Part1	:Clay_a.part
		Part2	:Clay_B.part
		Part	:Clay_C.part
		Lifespan	10		

	End

End


Condition
	On 	cycle
	Time 	38
	Chance	.25

	Event
		EName	Fire10
		Type	start
		At	Pivot
		altpiv	10
		
		#Part	:Clay_B2.part
		Part	:Clay_B3.part
		
		Lifespan	20

	End

	Event
		EName	Fire11
		Type	start
		At	Pivot
		altpiv	10
		
		Part4	:SmallSpecks.part
		Part	:SmallSpecks2.part
		Part	:Clay_ADifuse.part
		Part	:Clay_Flat2.part
		Part3	:Clay_Flat.part	
		Part	:Clay_a2.part
		Part1	:Clay_a.part
		Part2	:Clay_B.part
		Part	:Clay_C.part
		Lifespan	11		

	End

End


Condition
	On 	cycle
	Time 	40
	Chance	.25

	Event
		EName	Fire11
		Type	start
		At	Pivot
		altpiv	11
		
		#Part	:Clay_B2.part
		Part	:Clay_B3.part
		
			
		Lifespan	19		

	End

	Event
		EName	Fire11
		Type	start
		At	Pivot
		altpiv	11
		
		Part4	:SmallSpecks.part
		Part	:SmallSpecks2.part
		Part	:Clay_ADifuse.part
		Part	:Clay_Flat2.part
		Part3	:Clay_Flat.part	
		Part	:Clay_a2.part
		Part1	:Clay_a.part
		Part2	:Clay_B.part
		Part	:Clay_C.part
		Lifespan	10		

	End
End

Condition
	On 	cycle
	Time 	41
	Chance	.25

	Event
		EName	Fire11
		Type	start
		At	Pivot
		altpiv	12
		
		#Part	:Clay_B2.part
		Part	:Clay_B3.part
			
		Lifespan	18		

	End

	Event
		EName	Fire11
		Type	start
		At	Pivot
		altpiv	12
		
		Part4	:SmallSpecks.part
		Part	:SmallSpecks2.part
		Part	:Clay_ADifuse.part
		Part	:Clay_Flat2.part
		Part3	:Clay_Flat.part	
		Part	:Clay_a2.part
		Part1	:Clay_a.part
		Part2	:Clay_B.part
		Part	:Clay_C.part
		Lifespan	9		

	End

End

Condition
	On 	cycle
	Time 	36
	Chance	.25

	Event
		EName	Fire11
		Type	start
		At	Pivot
		altpiv	13
		
		#Part	:Clay_B2.part
		Part	:Clay_B3.part
					
		Lifespan	20		

	End

	Event
		EName	Fire11
		Type	start
		At	Pivot
		altpiv	13
		
		Part4	:SmallSpecks.part
		Part	:SmallSpecks2.part
		Part	:Clay_ADifuse.part
		Part	:Clay_Flat2.part
		Part3	:Clay_Flat.part	
		Part	:Clay_a2.part
		Part1	:Clay_a.part
		Part2	:Clay_B.part
		Part	:Clay_C.part
		Lifespan	8		

	End

End

Condition
	On 	cycle
	Time 	44
	Chance	.25

	Event
		EName	Fire11
		Type	start
		At	Pivot
		altpiv	14
		
		#Part5	:Clay_B2.part
		Part	:Clay_B3.part
		
		Lifespan	19		

	End

	Event
		EName	Fire11
		Type	start
		At	Pivot
		altpiv	14
		
		Part	:SmallSpecks.part
		Part	:SmallSpecks2.part
		Part	:Clay_ADifuse.part
		Part	:Clay_Flat2.part
		Part	:Clay_Flat.part
		Part1	:Clay_a2.part
		Part2	:Clay_a.part
		Part3	:Clay_B.part
		Part4	:Clay_C.part
		Lifespan	9		

	End

End

End				