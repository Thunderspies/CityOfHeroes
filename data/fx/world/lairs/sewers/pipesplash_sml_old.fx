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
		part1	World/Lairs/Sewers/PipeSplash_Sml_OLD.part
		part2	World/Lairs/Sewers/PipeSplash2_Sml_OLD.part
		part3	World/Lairs/Sewers/PipeSplashMist_Sml_OLD.part
		part4	World/Lairs/Sewers/PipeSplashFlat_Sml_OLD.part
		part5	World/Lairs/Sewers/PipeSplashMist2_Sml_OLD.part
	End
	

End

End			