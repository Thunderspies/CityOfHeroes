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
		Part1	:FogFlat.part
		Part2	:FogFlat2.part
		Part3	:Fog.part
	End


End

End			