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
		Sound smallwhoosh 40.0 40.0 .76
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

