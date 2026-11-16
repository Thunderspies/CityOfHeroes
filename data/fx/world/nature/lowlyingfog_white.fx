#########################################################
##	Fog, White (no green/color) - rroth)
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
		EName 	RoachGroupSmall
		Type	Posit 
		At	Root
		Part1	:LowFogFlat.part
		Part2	:LowFogFlat3_white.part
		Part3	:LowLyingFog.part
	End


End

End			