#########################################################
##	Claws Strike
########################################################
FxInfo

Input  
	Inpname	Mystic
End

Condition
	On 	Time
	Time 	15

	Event
		EName	Streak
		Type	local
		At	Mystic
		Geom	swoosh
		bhvr	behaviors/Claws.bhvr
		# Sound Claw2b 100.0 100.0 .6
	End

End


Condition
	On 	Time
	Time 	22

	Event
		EName	All
		Type	Destroy
		
	End
	

End

End
##################################

