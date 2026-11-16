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
	InpName	WepL
End

Input  
	InpName	WepR
End

LifeSpan	10

##################################

Condition
	On	Time
	Time	9

	Event
		Type	Local
		At	Origin
		Sound PunchA 50 50 .66
	End
End


Condition
	On	Time
	Time	22
		
	Event	
		ENAME	WepLCastCloud
		Type	Local
		At	WepL
		part1	:FlightAssaultHands01.part
		Pmagnet Chest
		Lifespan 7
	End

End



Condition
	On	Time
	Time	22
		
	Event	
		ENAME	WepRCastCloud
		Type	Local
		At	WepR
		part1	:FlightAssaultHands01.part
		Pmagnet Chest
		Lifespan 7
	End

End



End				