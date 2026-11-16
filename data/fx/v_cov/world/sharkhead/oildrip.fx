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
		Part1	V_COV/World/Sharkhead/OilWellup.part
		Part2	V_COV/World/Sharkhead/OilDrip.part
		LifeSpan	70		
	End


End


End			