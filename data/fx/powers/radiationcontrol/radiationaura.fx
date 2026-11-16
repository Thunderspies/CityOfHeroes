#########################################################
##	RadiationAura
FxInfo


Input  
	InpName	Hips
End

Input  
	InpName	Head
End

Input  
	InpName	Root
End

##################################
Condition
	On	Time
	Time	21
	Event
		Type	Posit
		At	Hips
		part1	:RadiationElectronsHit.part
		part2	:RadiationAuraGreen.part
		Part3	:RadiationBubblesHit.part
		Part4	:RadiationBubblesSlow.part
		part5	:RadiationAuraWhite.part
		
		PMagnet	Hips
	End

	Event
		Type	Posit
		At	Hips
		part4	:RadiationBubbleDotsHit.part
		PMagnet Head
	End

	Event
		Type	Posit
		At	Hips
		part2	:RadiationAuraGreenLocal.part
		part3	:RadiationAuraYellowLocal.part
		part4	:RadiationAuraWhiteLocal.part
		
		PMagnet	Hips
	End

End

Condition
	On	Time
	Time	7
	Event
		EName	Burst
		Type	local
		At	Hips
		Part5	:RadiationRingBurst.part	
	End
End

Condition
	On	Time
	Time	0
	Event
		EName	Rings
		Type	local
		At	Hips
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
		At	Hips
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