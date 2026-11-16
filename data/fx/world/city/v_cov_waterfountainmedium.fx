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
		
		part1	:V_COV_WaterFountainBody.part
		part3	:V_COV_WaterFountainMistBody.part
		part4	:V_COV_WaterBubble2.part
		part5	:V_COV_WaterBubbleBody2.part
				
	End

	Event
		EName 	AttachmentPoint
		Type	Local 
		At	head

		geom	WaterGeometry
		bhvr	behaviors/WaterSheetscaleM.bhvr
		part1	:V_COV_WaterFountainMistBody2Medium.part
		Part2	:V_COV_FlatWaterMedium.part
		Part4	:V_COV_CenterWaterMedium.part
		
	End
	

End
End			