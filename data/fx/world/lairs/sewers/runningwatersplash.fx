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
		
		part4	World/Lairs/Sewers/WaterMistDark.part
		part5	:WaterSplash2Dark.part
#		Part	:WaterMist2Dark.part

		part1	World/Lairs/Sewers/WaterSplash.part
		part2	World/Lairs/Sewers/WaterSplash2.part
		part3	World/Lairs/Sewers/WaterMist2.part
		part4	World/Lairs/Sewers/WaterMist.part
		part5	World/Lairs/Sewers/WaterSplashMist2.part
		SOund 	DrainSplash_loop 30.0 17.0 .4
	End
	

End

End			