#########################################################
##	iceweapon
########################################################
FxInfo

Input  
	Inpname	Chest
End

#Lifespan 50

Condition
	On 	Time
	Time 	0

	Event
		EName 	Mallet
		Type	start 
		At	Chest
		Geom	TwistBubble
		Sound PunchHit1 100.0 100.0 .8
	End

End


End
