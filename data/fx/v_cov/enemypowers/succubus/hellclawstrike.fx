#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	100


Condition
	On 	Time
	Time 	0

	Event
		EName 	StreakR
		Type	Local 
		At	WepR
		Part	:HellClawStreak.part
		Part1	:HellClawHands.part
		Part4	:DarkSparks.part
		#Sound	Succubusclaw 70 60 .77
		Sound warshadepunch 70 70 .9
	End

	Event
		EName 	StreakL
		Type	Local 
		At	WepL
		Part	:HellClawStreak.part
		Part1	:HellClawHands.part
		Part4	:DarkSparks.part
	End

End

Condition
	On	Time
	Time	30
			
	Event
		Ename	all
		Type	Destroy
	End

End

End	


			