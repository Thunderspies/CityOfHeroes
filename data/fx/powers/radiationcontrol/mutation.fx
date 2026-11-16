#########################################################
##	chill touch hit
FxInfo

Input  
	InpName	T_root
End

Input  
	InpName	WepR
End

Input  
	InpName	WepL
End

LifeSpan	30

############################################################


Condition
	On 	Time
	Time 	24
	
	Event
		Type	posit
		At 	T_root
		#Sound	Spewhiss 66 22 .5
	End
End
	


Condition
	On 	Time
	Time	25
	
	Event
		Type	posit
		At 	T_root
		#Sound	radiation5_loop 66 22 .45
	End
End


Condition
	On 	Time
	Time 	0

	Event
		EName 	Orb
		Type	posit 
		At	T_root
		part1	:MutationMist.part
		part2	:MutationMist2.part
		Part3	:MutationRingsFlat.Part	
		Part4	:MutationMistGlows.part
		Part5	:MutationMistRings.part
	
		#Sound	enervating2_loop 66 22 .23
	End

End

Condition
	On 	Time
	Time 	5

	Event
		EName 	rays
		Type	posit 
		At	T_root
		Part1	:MutationRAYS.part
		
	End
End


End			