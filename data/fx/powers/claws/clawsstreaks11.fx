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
		Sound whoosh12 100.0 100.0 .75
	End

End


Condition
	On 	Time
	Time 	21

	Event
		EName	All
		Type	Destroy
		
	End
	

End

End
##################################
