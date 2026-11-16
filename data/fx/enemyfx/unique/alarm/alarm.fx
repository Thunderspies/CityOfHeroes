#########################################################
##	Fireweapon
########################################################
FxInfo

########################################################

Condition	

	Event
		EName 	light
		Type	Local 
		At		ROOT
		Sound sirenbeep_loop 200 200 10
	End

	Event
		EName 	light
		Type	Local 
		At		ROOT
		Part	:FlashingLight.part
		Lifespan 5
	End
End

Condition	
	On Cycle
	Time 15 
	Event
		EName 	light
		Type	Local 
		At		ROOT
		Part	:FlashingLight.part
		Lifespan 5
	End
End

End
##################################

