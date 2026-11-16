#########################################################
##	FlameSword
##
FxInfo
	
LifeSpan 0

Input  
	InpName	Root
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	Root
		Part1	Yellow2.part
		Part2	YellowFlame01.part
		#Part3	RedFlame01.part
	End
End

End			