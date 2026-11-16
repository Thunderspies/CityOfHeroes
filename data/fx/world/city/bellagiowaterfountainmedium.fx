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
		EName 	Fire
		Type	local
		At	root
		Anim	FX_BellagioWater

	End

	Event
		EName 	AttachmentPoint
		Type	Local 
		At	Fire
		Animpiv	hips
		
		part1	:BellagioWaterFountainMedium.part
		part2	:BellagioFountainMistMedium.part
		part3	:BellagioWaterBubbleMedium.part
		part4	:BellagioBubbleMedium.part
				
	End
	

End
End			