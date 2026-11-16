#########################################################

FxInfo
	
LifeSpan 0

Input  
	InpName	root
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	AttachmentPoint
		Type	Local 
		At	root
		altpiv	2
		geom	WaterGeometry
		part1	:V_COV_WaterFountainMistBody2.part
		Part2	:V_COV_FlatWater.part
		Part3	:V_COV_FlatWaterCircle.part
		Part4	:V_COV_CenterWater.part
		
	End
	

End

End			