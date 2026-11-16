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

		geom	WaterGeometry
		bhvr	behaviors/WaterSheetscaleM.bhvr
		part1	:WaterFountainMistBody2Medium.part
		Part2	:FlatWaterMedium.part
		Part4	:CenterWaterMedium.part
		
	End
	

End

End			