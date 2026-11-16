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
		
		part1	:WaterFountainBodySmall.part
		part2	:WaterFountainMistBodySmall.part
		part3	:WaterBubble2Small.part
		part4	:WaterBubbleBody2Small.part
				
	End

	Event
		EName 	AttachmentPoint
		Type	Local 
		At	head
		
		Part1	:FlatWaterSmall.part
		#Part2	:FlatWaterCircleSmall.part
		Part3	:CenterWaterSmall.part
		
	End
	

End

End			