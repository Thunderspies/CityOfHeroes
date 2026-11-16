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
		
		part1	:WaterFountainBody3.part
		part3	:WaterFountainMistBody3.part
		part4	:WaterBubble3.part
		part5	:WaterBubbleBody3.part
		Sound Waterfountain2_loop 65 65 .55
				
	End

	Event
		EName 	AttachmentPoint
		Type	Local 
		At	head

		Part3	:FlatWaterCircleMedium3.part
		Part4	:CenterWaterMedium3.part
		
	End
	

End
End			