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
		
		part1	:WaterFountainBodyBubblerL2.part
		part3	:WaterFountainMistBodyBubblerL2.part
		part4	:WaterBubble2BubblerL2.part
		part5	:WaterBubbleBody2BubblerL2.part
				
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