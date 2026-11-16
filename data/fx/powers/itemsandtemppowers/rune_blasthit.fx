#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	20
   
Condition
	On 	Time
	Time 	0
	
	Event
		Type	local
		At	chest
		Sound Runehit2 80 80 .9
		
	End

End


Condition
	On 	Time
	Time 	0
	
	Event
		Type	local
		At	chest

		Part	:Rune3.part
		LifeSpan	10		
		
	End

End


Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	chest
		
		Part	:HandTendrilsBurst2.part
		
		LifeSpan	15		
		
	End

End    

End