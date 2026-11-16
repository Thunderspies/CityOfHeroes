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
		part1	World/Lairs/Sewers/WaterSplashMist.part
		part2	World/Lairs/Sewers/WaterSplashMist2.part
		#part3	World/Lairs/Sewers/WaterMist.part
		part4	World/Lairs/Sewers/WaterMist3.part
	End
	

End

End			