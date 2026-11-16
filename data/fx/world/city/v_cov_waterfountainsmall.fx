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
		
		part1	:V_COV_WaterFountainBodySmall.part
		part3	:V_COV_WaterFountainMistBodySmall.part
		part4	:V_COV_WaterBubble2Small.part
		part5	:V_COV_WaterBubbleBody2Small.part
				
	End

	Event
		EName 	AttachmentPoint
		Type	Local 
		At	head

		geom	WaterGeometry
		
		part1	:V_COV_WaterFountainMistBody2Small.part
		Part2	:V_COV_FlatWaterSmall.part
		Part4	:V_COV_CenterWaterSmall.part
		
	End
	

End
End			