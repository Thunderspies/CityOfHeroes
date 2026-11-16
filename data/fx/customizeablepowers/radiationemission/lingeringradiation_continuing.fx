#########################################################
##	RadiationAura
FxInfo

#LifeSpan	40

Input  
	InpName	Hips
End

########################################################


Condition
	On	Time
	Time	7

	Event
		
		Type	Posit
		At	Hips
		#part1	:RadiationElectronsHit.part
		part2	CustomizeablePowers\RadiationEmission\RadiationBubbleBlastDotsHit.part
		#Part3	:RadiationBubblesHit.part
		Part4	CustomizeablePowers\RadiationEmission\RadiationBubblesSlow.part
		#part5	:RadiationAuraYellowHit2.part
		PMagnet	Hips
		
	End

	Event
		ENAME	LocalSystems
		Type	Posit
		At	Hips
		
		part2	CustomizeablePowers\RadiationEmission\RadiationBubbleBlastDotsHitx.part
		#Part3	:RadiationBubblesHitx.part
		Part4	CustomizeablePowers\RadiationEmission\RadiationBubblesSlowx.part
		#part5	:RadiationAuraYellowHit2x.part
		PMagnet	Hips
		
	End

	Event
		ENAME	LocalSystems
		Type	Posit
		At	Hips
		
		part4	CustomizeablePowers\RadiationEmission\RadiationTealCloud.part
			#part5	:RadiationAuraYellowHit3x.part
		PMagnet	Hips
		
	End

End


End				