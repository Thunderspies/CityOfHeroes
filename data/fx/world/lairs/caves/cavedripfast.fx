#########################################################
##	FireRing
##
FxInfo
	
#########################################################
Condition
	On 	cycle
	Time 	20
	Chance	.8

	Event
		EName 	GroupSmall
		Type	Posit 
		At	origin
		Part1	:CaveWellup.part
		Part2	:CaveDrip.part
		Lifespan 80				
	End


End

End