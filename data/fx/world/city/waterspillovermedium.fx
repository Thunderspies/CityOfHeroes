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

		
		bhvr	behaviors/WaterSheetscaleM.bhvr
		part1	:WaterFountainMistBody2Medium.part
				
	End
	

End

End			