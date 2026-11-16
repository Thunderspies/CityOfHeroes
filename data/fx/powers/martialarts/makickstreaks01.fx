#########################################################
##	KickStreaks
########################################################
FxInfo

Input  
	Inpname	FootR
End

Condition
	On 	Time
	Time 	13

	Event
		EName	KickStreak
		Type	local
		At	FootL
		Geom	swoosh
		bhvr	behaviors/Claws.bhvr
		Sound Whoosh5 100.0 100.0 .6
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

