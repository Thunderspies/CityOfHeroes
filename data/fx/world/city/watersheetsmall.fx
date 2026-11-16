#########################################################
##	SStreetSteeam01
##
FxInfo
	
LifeSpan 0

Input  
	InpName	head
End

Condition
	On 	Time
	Time 	0

	
	Event
		EName 	AttachmentPoint
		Type	Local 
		At	head

		geom	WaterGeometry
		
		part1	:WaterFountainMistBody2Small.part
		Part2	:FlatWaterSmall.part
		Part4	:CenterWaterSmall.part
		
	End
	

End
End			