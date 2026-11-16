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
		Sound macheteWhoosh 30.0 30.0 .45
	End

End


Condition
	On 	Time
	Time 	20

	Event
		EName	All
		Type	Destroy
		
	End
	

End

End
##################################
