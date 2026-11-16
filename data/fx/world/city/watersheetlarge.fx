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
		bhvr	behaviors/WaterSheetScaleL.bhvr
		part1	:WaterSpillOverLarge.part
		Part2	:FlatWater.part
		Part3	:FlatWaterCircle.part
		Part4	:CenterWater.part
		
	End
	

End

End			