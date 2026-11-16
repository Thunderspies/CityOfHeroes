#########################################################
##	RadiationAura
FxInfo

LifeSpan	200

Input  
	InpName	Hair
End

Input  
	InpName	Hips
End

##################################
Condition
	On	Time
	Time	0

	Event
		
		Type	Posit
		At	Hips
		part1	:RadiationElectronsHit.part
		#part2	:RadiationAuraGreen.part
		Part3	:RadiationBubblesHit.part
		Part4	:RadiationBubblesSlow.part
		#part5	:RadiationAuraWhite.part
		
	End

	Event
		Type	Posit
		At	Hips
		part2	:RadiationAuraGreenHit.part
		part3	:RadiationAuraYellowHit.part
		part4	:RadiationBubbleDotsHit.part
		
	End

	Event
		Type	Posit
		At	Hips
		part4	:RadiationBubbleDotsHit.part
		PMagnet Hair
	End


End

End				