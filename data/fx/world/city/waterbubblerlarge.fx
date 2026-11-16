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
		part1	:WaterFountainBodyLarge.part
		part2	:WaterFountainMistBodyLarge.part
		part3	:WaterBubble2Large.part
		part4	:WaterBubbleBody2Large.part
		Sound waterfountain1_loop 90 90 .82
	End

	Event
		EName 	AttachmentPoint
		Type	Local 
		At	head
			
		Part1	:FlatWaterLarge.part
		Part2	:FlatWaterCircleLarge.part
		Part3	:CenterWaterLarge.part
		
	End
	

End

End			