#########################################################
##	FireRing
##
FxInfo
	

Input  
	InpName	Origin
End

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	bubbles
		Type	Local 
		At	Origin
		part1	World/Lairs/Sewers/PipeSplash.part
		part2	World/Lairs/Sewers/PipeSplash2.part
		part3	World/Lairs/Sewers/PipeSplashMist.part
#		part4	World/Lairs/Sewers/PipeSplashFlat.part
		part5	World/Lairs/Sewers/PipeSplashMist2.part
	End
	

End

End			