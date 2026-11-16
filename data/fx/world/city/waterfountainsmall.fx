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
		
		part1	:WaterFountainBodySmall.part
		part3	:WaterFountainMistBodySmall.part
		part4	:WaterBubble2Small.part
		part5	:WaterBubbleBody2Small.part
				
	End

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