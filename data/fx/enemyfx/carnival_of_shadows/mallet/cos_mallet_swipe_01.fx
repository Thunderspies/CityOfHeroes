#########################################################
##	template

FxInfo

Input
	Inpname Mystic
End

Input
	Inpname Origin
End

Lifespan 33


Condition
	On 	Time
	Time 	13

	Event
		Type	local
		At	Origin
		Sound Whoosh5 100.0 100.0 .75
	End

	Event
		EName	Prime
		Type	Local
		At	WepR
		Geom	GEO_WepR_Mallet
	End
End

	
End	

	