#########################################################
##	FireRing
##
FxInfo
	

Input  
	InpName	Root
End

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Posit 
		At	Root
		Part1	:LowFogFlatLRG.part
		Part2	:LowFogFlat2LRG.part
		Part3	:LowLyingFogLRG.part
	End


End

End			