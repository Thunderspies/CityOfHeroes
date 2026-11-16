#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	Mystic
End

Condition
	On 	Time
	Time 	13

	Event
		EName	Streak
		Type	local
		At	Mystic
		Geom	swoosh
		bhvr	behaviors/Claws.bhvr
		Sound hobble2 100.0 100.0 .8
	End

End


Condition
	On 	Time
	Time 	44

	Event
		EName	All
		Type	Destroy
		
	End
	

End

End
##################################
