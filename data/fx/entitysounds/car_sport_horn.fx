#########################################################
##	Sports Car Horn SFX
#########################################################

FxInfo

Lifespan 1

Input  
	InpName	Origin
End
Condition
	Chance 0.1
	Event
		Type	Local 
		At	Origin
		Sound HornSport1 150 150 1
	End
	Event
		Type	Local 
		At	Origin
		Sound HornSport1Dist 500 500 .6
	End
End