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
		part1	World/Lairs/Sewers/PipeSplash_Sml.part
		part2	World/Lairs/Sewers/PipeSplash2_Sml.part
		part3	World/Lairs/Sewers/PipeSplashMist_Sml.part
		part4	World/Lairs/Sewers/PipeSplashFlat_Sml.part
		part5	World/Lairs/Sewers/PipeSplashMist2_Sml.part
	End
	

End

End			