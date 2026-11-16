#########################################################

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
		
		part1	:WaterFountainBody.part
		part3	:WaterFountainMistBody.part
		part4	:WaterBubble2.part
		part5	:WaterBubbleBody2.part
				
	End

	Event
		EName 	AttachmentPoint
		Type	Local 
		At	head
		
		#part1	:WaterFountainMistBody2.part
		Part2	:FlatWater.part
		Part3	:FlatWaterCircle.part
		Part4	:CenterWater.part
		
	End
	

End

End			