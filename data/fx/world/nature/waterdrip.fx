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
	Time 	70
	Chance	.8

	Event
		EName 	GroupSmall
		Type	Posit 
		At	origin
		Bhvr	behaviors/SlimeDrip.bhvr
		Part1	World/Nature/WaterWellup.part
		Part2	World/Nature/WaterDrip.part
		LifeSpan	70		
	End


End


End			