#########################################################
##	GroupFlight
FxInfo


Input  
	InpName	Origin
End

Input  
	InpName	Target
End

Input  
	InpName	Chest
End

Input  
	InpName	Eyes
End

Input  
	InpName	WepR
End

LifeSpan	12

##################################

Condition
	On	Time
	Time	2
		
	Event	
		ENAME	CloudBurstR
		Type	Start
		At	Chest
		part1	:FlightAssaultHitCloud01.part
		Pmagnet Chest
		Sound PP13 80 80 .8
		Lifespan 12
	End

End

Condition
	On	Time
	Time	3
		
	Event	
		ENAME	CloudBurstR
		Type	Start
		At	Chest
		part1	:FlightAssaultHitStar01.part
		part2	:FlightAssaultHitGlow01.part
		part3	:FlightAssaultHitStar02.part
		Pmagnet Chest
		Lifespan 5
	End

End


End				