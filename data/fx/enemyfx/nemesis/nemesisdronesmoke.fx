#########################################################
##	template

FxInfo

Input  
	Inpname	WepL
End

Input  
	Inpname	hair
End


Flags    InheritAlpha
Condition
	On 	Time
	Time 	0

	Event
		EName	spot
		Type	Local
		At	hair
		Geom	Drone
		Sound nemesis2_loop 80 80 .29
	End

	Event
		EName	spot2	
		Type	Local
		At	spot
		altpiv	1
		part1	:DroneSmoke.part
	End
End

Condition
	On 	triggerbits
	Triggerbits	death

	Event
		EName 	spot
		Type	destroy

	End

	Event
		EName 	spot2
		Type	destroy

	End

End


End		