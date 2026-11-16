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
		part1	:WaterFountainBody.part
		part3	:WaterFountainMistBody.part
		part4	:WaterBubble2.part
		part5	:WaterBubbleBody2.part
				
	End

	Event
		EName 	AttachmentPoint
		Type	Local 
		At	Fountain01
		altpiv	2
		geom	WaterGeometry
		part1	:WaterFountainMistBody2.part
		Part2	:FlatWater.part
		Part3	:FlatWaterCircle.part
		Part4	:CenterWater.part
		
	End
	

End
End			