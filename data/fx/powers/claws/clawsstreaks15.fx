#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	Mystic
End

Condition
	On 	Time
	Time 	3

	Event
		EName	Streak
		Type	local
		At	Mystic
		Geom	swoosh
		bhvr	behaviors/Claws.bhvr
		Sound metalsqueak1 40.0 40.0 .6
		Sound metalsqueak2 40.0 40.0 .6
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

