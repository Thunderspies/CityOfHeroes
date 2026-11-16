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
		
		part1	:V_COV_WaterFountainBody3.part
		part3	:V_COV_WaterFountainMistBody3.part
		part4	:V_COV_WaterBubble3.part
		part5	:V_COV_WaterBubbleBody3.part
		Sound Waterfountain2_loop 30 30 .28
				
	End

	Event
		EName 	AttachmentPoint
		Type	Local 
		At	head

		Part3	:V_COV_FlatWaterCircleMedium3.part
		Part4	:V_COV_CenterWaterMedium3.part
		
	End
	

End
End			