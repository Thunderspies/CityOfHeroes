#########################################################
##	RadiationAura
FxInfo

LifeSpan	50

Input  
	InpName	Hips
End

########################################################





Condition
	On	Time
	Time	30

	Event
		
		Type	Posit
		At	Hips
		part1	:RadiationElectronsHit.part
		part2	:RadiationBubbleDotsHit.part
		Part3	:RadiationBubblesHit.part
		Part4	:RadiationBubblesSlow.part
		PMagnet	Hips
		
	End

	Event
		Type	Posit
		At	Hips
		part2	:RadiationAuraGreenHit.part
		part3	:RadiationAuraYellowHit.part
		
		
	End


End


Condition
	On 	Time
	Time 	40
	
	Event
		Type	Destroy
		EName	LightRays
	End


End

End				