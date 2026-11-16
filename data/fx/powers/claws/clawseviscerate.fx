#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	
	Inpname Origin
End

Input

	Inpname	Mystic

End

lifespan	50

Condition
	On 	Time
	Time 	10

	Event
		Type Local
		At origin
		Sound Eviscerate5 100.0 100.0 .8
	End
End



Condition
	On 	Time
	Time 	46

	Event
		EName	Streak
		Type	start
		At	Mystic
		Geom	swoosh
		bhvr	behaviors/Claws.bhvr
	End

End


Condition
	On 	Time
	Time 	48

	Event
		EName	streak
		Type	Destroy
		
	End
	

End


End
##################################
