#########################################################
##	Fire_Ball
##
FxInfo



Condition
	On 	Time
	Time 	0

	Event
		EName	InitialSwordFire
		Type	Local 
		At	WepR
		Part1	:HellClawHands.part
		Part4	:DarkSparks.part
	
	End

	Event
		EName	InitialSwordFire
		Type	Local 
		At	WepL
		Part1	:HellClawHands.part
		Part4	:DarkSparks.part
	
	End


End

Condition
	On 	Time
	Time  	4
	Event
		Type	Local
		At	Origin
		Sound Fireball1 60.0 60.0 .33
	End

End


End	


			