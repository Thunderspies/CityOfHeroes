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

lifespan	69

Condition
	On 	Time
	Time 	29

	Event
		Type Local
		At origin
		Sound Eviscerate5 100.0 100.0 .8
	End
End



Condition
	On 	Time
	Time 	65

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
	Time 	67

	Event
		EName	streak
		Type	Destroy
		
	End
	

End


End
##################################
