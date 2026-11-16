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
		
		part1	:BellagioWaterFountainSmall.part
		part2	:BellagioFountainMistSmall.part
		part3	:BellagioWaterBubbleSmall.part
		part4	:BellagioBubbleSmall.part
				
	End
	

End
End			