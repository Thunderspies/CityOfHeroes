#########################################################
##	
########################################################
FxInfo

Input  
	Inpname	Root
End

Input
	InpName Origin
End

Lifespan	90
##################################################################

Condition
	On	Time
	Time	0
	Event
		EName 	Player_Box
		Type	Local
		At	Root 
		Geom	Temp_Power

	End


	Event
		Type Local
		At origin
		Sound Whistle1 100 100 .7
	End

End


End
##################################

