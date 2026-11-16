#########################################################
##	SStreetSteeam01
##
FxInfo
	
LifeSpan 0

Input  
	InpName	root
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Fountain01
		Type	local
		At	root
		Geom	Fountain01
		
	End

	Event
		EName 	AttachmentPoint
		Type	Local 
		At	Fountain01
		altpiv	1
		part1	:V_COV_WaterFountainBody.part
		part3	:V_COV_WaterFountainMistBody.part
		part4	:V_COV_WaterBubble2.part
		part5	:V_COV_WaterBubbleBody2a.part
				
	End

	Event
		EName 	AttachmentPoint
		Type	Local 
		At	Fountain01
		altpiv	2
		geom	WaterGeometry
		part1	:V_COV_WaterFountainMistBody2.part
		Part2	:V_COV_FlatWater.part
		Part3	:V_COV_FlatWaterCircle.part
		Part4	:V_COV_CenterWatera.part
		
	End
	

End
End			