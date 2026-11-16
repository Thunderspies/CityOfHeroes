#########################################################
##	Stone_Wall
#########################################################

FxInfo

Input  
	Inpname	Root
End

#########################################################
##	Audio
#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		Sound Rumble1_loop 100 100 .76
	End
	
End

Condition

	On Cycle
	Time	46
	Chance	1
	
	Event
			
		Type	start
		At	Root
		Sound Quake3 100 100 .64
		Sound Quake4 100 100 .76
		Sound Quake3 100 100 .68
		Sound Rumble4 100 100 .55
		Sound Quake5 100 100 .6
		LifeSpan	46
		
	End

End



#########################################################

Condition
	On Cycle
	Time	40
	Chance	.8
	
	Event
			
		Type	start
		At	Root
		Bhvr	Behaviors/ReactorQuakeSpuartic.bhvr
		LifeSpan	20
		
	End

End

End


