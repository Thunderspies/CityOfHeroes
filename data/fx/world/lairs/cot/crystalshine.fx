#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	Origin
End


########################################################



Condition
	On 	Cycle
	Time 	33
	Chance	.05	

	Event
		EName 	Prime
		Bhvr	behaviors/crystalshine.bhvr
		Type	Local 
		At	Origin
		Part1	:ShineCore.part
		Part2	:ShineRings.part
		lifeSpan	33
	End


End

End
##################################

