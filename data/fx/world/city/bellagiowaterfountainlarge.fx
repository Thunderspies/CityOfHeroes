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
		
		part1	:BellagioWaterFountainLarge.part
		part2	:BellagioFountainMistLarge.part
		part3	:BellagioWaterBubbleLarge.part
		part4	:BellagioBubbleLarge.part
				
	End
	

End
End			