#########################################################
##	jack
##
FxInfo


Input  
	InpName	Root
End

#Flags    InheritAlpha
LifeSpan	60

Condition
	On 	Time
	Time 	0


	Event
		Type	local 
		At	Root
		BHVR	:OffsetChest90.bhvr
		Part	:SelfMushroomPortalRingPulse.part
		Part	:SelfMushroomPortalRingPulse.part
		LifeSpan	20
	End

	Event
		EName 	particles
		Type	posit 
		At	Root
		Part	:SelfMushroomPortalglowBase.part
		
		LifeSpan	20
	End

	Event
		Type	local 
		At	Root
		BHVR	:OffsetChest90.bhvr
		Part	:SelfMushroomPortalRings.part
		Part	:SelfMushroomPortalRings2.part
		LifeSpan	30
	End

End

End			