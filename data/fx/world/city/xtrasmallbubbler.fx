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

		Part2	:FlatWaterXtraSmall.part
		Part4	:CenterWaterXtraSmall.part
		
	End
	

End
End			