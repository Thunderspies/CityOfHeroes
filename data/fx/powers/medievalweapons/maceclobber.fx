#########################################################
##	template

FxInfo

Input
	Inpname Mystic
End

Input
	Inpname Origin
End


Lifespan 36


Condition
	On 	Time
	Time 	20

	Event
		Type	local
		At	Origin
		Sound clobberwhoosh 100.0 100.0 .78
	End

End

Condition
	On 	Time
	Time 	35

	Event
		EName	Streak
		Type	local
		At	Mystic
		Geom	swoosh
		bhvr	behaviors/Medieval1.bhvr
	End

End

	
End	

	