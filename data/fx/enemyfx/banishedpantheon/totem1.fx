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
		Sound totem1_loop 60.0 60.0 .4
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
