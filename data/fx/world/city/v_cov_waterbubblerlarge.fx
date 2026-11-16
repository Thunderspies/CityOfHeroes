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
		part1	:V_COV_WaterFountainBodyLarge.part
		part2	:V_COV_WaterFountainMistBodyLarge.part
		part3	:V_COV_WaterBubble2Large.part
		part4	:V_COV_WaterBubbleBody2Large.part
		Sound waterfountain1_loop 90 90 .82
	End

	Event
		EName 	AttachmentPoint
		Type	Local 
		At	head
			
		Part1	:V_COV_FlatWaterLarge.part
		Part2	:V_COV_FlatWaterCircleLarge.part
		Part3	:V_COV_CenterWaterLarge.part
		
	End
	

End

End			