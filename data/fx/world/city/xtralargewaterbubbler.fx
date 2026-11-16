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
		
		part1	:WaterFountainBodyLarge.part
		part3	:WaterFountainMistBodyLarge.part
		part4	:WaterBubble2Large.part
		part5	:WaterBubbleBody2Large.part
				
	End

	Event
		EName 	AttachmentPoint
		Type	Local 
		At	head

		Part3	:FlatWaterXtraLarge.part
		Part4	:CenterWaterXtraLarge.part
		
	End
	

End
End			