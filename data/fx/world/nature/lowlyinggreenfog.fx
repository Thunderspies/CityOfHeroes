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
		EName 	RoachGroupSmall
		Type	Posit 
		At	Root
		Part1	:GreenLowFogFlat.part
		Part2	:GreenLowFogFlat2.part
		Part3	:GreenLowLyingFog.part
	End


End

End			