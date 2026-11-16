#########################################################
##	RadiationAura
FxInfo




##################################
Condition
	On	Time
	Time	21
	Event
		Type	Posit
		At	Chest
		part1	:RadiationElectronsHit.part
		part2	:RadiationAuraGreen.part
		Part3	:RadiationBubblesHit.part
		Part4	:RadiationBubblesSlow.part
		part5	:RadiationAuraWhite.part
		
		PMagnet	Chest
	End

	Event
		Type	Posit
		At	Chest
		part4	:RadiationBubbleDotsHit.part
		PMagnet Head
	End

	Event
		Type	Posit
		At	Chest
		part2	:RadiationAuraGreenLocal.part
		part3	:RadiationAuraYellowLocal.part
		part4	:RadiationAuraWhiteLocal.part
		
		PMagnet	Chest
	End

End

Condition
	On	Time
	Time	7
	Event
		EName	Burst
		Type	local
		At	Chest
		Part5	:RadiationRingBurst.part	
	End
End

Condition
	On	Time
	Time	0
	Event
		EName	Rings
		Type	local
		At	Chest
		Part4	:RadiationRings.part
		Part5	:RadiationRingBurst.part	
	End

	Event
		EName	Rings
		Type	local
		At	Root
		#Part3	:RadiationRingsContinuedFlat.part
		Part4	:RadiationRingsFlat.part
			
	End
End

Condition
	On	Time
	Time	55
	Event
		EName	RingsContinued
		Type	local
		At	Chest
		Part4	:RadiationRingsContinued.part
		Part5	:RadiationLightHitLocal.part
	End
End

Condition
	On	Time
	Time	70
		
	Event
		Ename	Rings
		Type	Destroy
	End

End

Condition
	On	Time
	Time	70
		
	Event
		Ename	Burst
		Type	Destroy
	End

End


End				