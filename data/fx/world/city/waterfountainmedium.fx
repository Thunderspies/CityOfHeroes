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
		
		part1	:WaterFountainBody.part
		part3	:WaterFountainMistBody.part
		part4	:WaterBubble2.part
		part5	:WaterBubbleBody2.part
				
	End

	Event
		EName 	AttachmentPoint
		Type	Local 
		At	head

		geom	WaterGeometry
		bhvr	behaviors/WaterSheetscaleM.bhvr
		part1	:WaterFountainMistBody2Medium.part
		Part2	:FlatWaterMedium.part
		Part4	:CenterWaterMedium.part
		
	End
	

End
End			