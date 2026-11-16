#########################################################
##	
##

FxInfo

Input  
	InpName	Origin
End

Input  
	InpName	Target
End

Input  
	InpName	Root
End

Input  
	InpName	Chest
End

Input  
	InpName	T_Root
End




LifeSpan	200

##################################

COndition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		#Sound	Effigy5 100 90 .75
	End
End


Condition
	On 	Time
	Time 	0

	Event
		EName	Burst
		Type	Start
		At	Chest
		Part	:WhiteCenter.part
		Part	:Flash01.part
		Part	:Smoke1.part
		bhvr	:RocketExploShake.bhvr
		
		Lifespan 10

		
	End
End

Condition
	On 	Time
	Time 	5

	Event
		EName	Smoke
		Type	Start
		At	Head
		Part	:Smoke2.part
		Lifespan 5

	End
End


End

	