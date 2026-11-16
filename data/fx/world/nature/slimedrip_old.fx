#########################################################
##	FireRing
##
FxInfo
	

Input  
	InpName	Root
End

#########################################################

Condition
	On 	cycle
	Time 	70
	Chance	.85
	
	Event
		EName 	RoachGroupSmall
		Type	Posit 
		At	Root
		Bhvr	behaviors/SlimeDrip.bhvr
		Part1	World/Nature/SlimeWellup_OLD.part
		Part2	World/Nature/SlimeDrip_OLD.part

		LifeSpan	70
	End

	
End


End			