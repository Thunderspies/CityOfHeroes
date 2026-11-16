#########################################################
##	template

FxInfo

Input
	InpName Origin
End

Input  
	InpName	Target
End

Lifespan 300

######################################################


Condition
	On 	Cycle
	Time 	1
	Chance	.2
	
	Event
		EName	DustStreamChild
		Type	Local
		At	Origin
		Geom	FX_DryHeaveBone
		bhvr	:DryHeaveBoneProject.bhvr
		Lifespan 10
	End

End

Condition
	On 	Cycle
	Time 	2
	Chance	.2
	
	Event
		EName	DustStreamChild
		Type	Local
		At	Origin
		Geom	FX_DryHeaveSkull
		bhvr	:DryHeaveBoneProject.bhvr
		Lifespan 10
	End

End

Condition
	On 	Cycle
	Time 	3
	Chance	.2
	
	Event
		EName	DustStreamChild
		Type	Local
		At	Origin
		Geom	FX_DryHeaveRibCage
		bhvr	:DryHeaveBoneProject.bhvr
		Lifespan 10
	End

End

End