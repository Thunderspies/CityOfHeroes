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

		#Part2	:SpillOverFlatWaterSmall.part
		Part4	:SpillOverWaterMedium.part
		
	End
	

End
End			