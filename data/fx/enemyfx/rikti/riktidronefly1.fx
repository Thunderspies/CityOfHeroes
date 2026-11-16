#########################################################
##	Martial Arts Hit FX
########################################################
FxInfo

Input  
	Inpname	Origin
End


Condition
	On 	Time
	Time 	0

	Event
		EName	Sound
		Type	local 
		At	Origin
		Sound	spaceship6_loop 70.0 50.0 .4	
	End

End

Condition
	On 	triggerbits
	Triggerbits DEATH

	Event
		EName	Sound
		Type	destroy		
	End

End


End
