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

		
		bhvr	behaviors/WaterSheetscaleL.bhvr
		part1	:WaterFountainMistBody2Large.part
		
		
	End
	

End
End			