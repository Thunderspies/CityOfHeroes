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
		#Part1	:LargeLowFogFlat.part
		#Part2	:LargeLowFogFlat2.part
		#Part3	:LargeLowLyingFog.part
		Sound fogwind_loop 60 60 .29
	End


End

End			