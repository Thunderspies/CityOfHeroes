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
		#Part1	:LowFogFlat.part
		#Part2	:LowFogFlat2.part
		#Part3	:LowLyingFog.part
		Sound fogwind_loop 44 44 .23
	End


End

End			