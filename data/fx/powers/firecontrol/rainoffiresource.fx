############################################################
##	Fireweapon
###########################################################
FxInfo

Input  
	Inpname	WepR
End

Input  
	Inpname	WepL
End

Condition
	On 	Time
	Time 	0

	Event
		EName	InitialSwordFire
		Type	Local 
		At	WepR
		Part1	:FireHandz.part
		Sound Firehand_loop 100.0 100.0 .65
	
	End


	Event
		EName	InitialSwordFire1
		Type	Local
		At	WepL
		Part1	:FireHandz.part
		Sound Ignite2 100.0 100.0 .88
	
	End


End

End

##############################################################

