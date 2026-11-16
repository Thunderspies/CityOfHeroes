#########################################################
##	FireRing
##
FxInfo
	

Input  
	InpName	origin
End

#########################################################

Condition
	On 	cycle
	Time 	20
	Chance	.8

	Event
		EName 	GroupSmall
		Type	Posit 
		At	origin
		Bhvr	behaviors/SlimeDrip.bhvr
		Part1	World/Nature/SlimeWellup_OLD.part
		Part2	World/Nature/SlimeDrip_OLD.part
		LifeSpan	20		
	End


End


End			