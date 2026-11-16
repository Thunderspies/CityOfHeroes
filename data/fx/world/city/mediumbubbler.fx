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
		
		part1	:WaterFountainBodyL2.part
		part3	:WaterFountainMistBodyL2.part
		part4	:WaterBubble2L2.part
		part5	:WaterBubbleBody2L2.part
				
	End

	Event
		EName 	AttachmentPoint
		Type	Local 
		At	head

		Part3	:FlatWaterCircleMedium.part
		Part4	:CenterWaterMedium2.part
		
	End
	

End
End			