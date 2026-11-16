#########################################################
##	FireAura
FxInfo

LifeSpan	70

##################################

Condition
	On	Time
	Time	0
	
	Event
		EName	InitialSwordFire
		Type	Local 
		At	WepL
		Part1	:FireHandz.part
		Sound ignite2 50 50 .5
		LifeSpan	70
	
	End

	Event
		EName	InitialSwordFire
		Type	Local 
		At	WepR
		Part1	:FireHandz.part
		#Sound	Firehand_loop 100.0 30.0 .55
		LifeSpan	70
	
	End

End
			

End				