#########################################################
##	Fireweapon
########################################################
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
		Part1	CustomizeablePowers\FireControl\FireHands.part
		Sound firering 100 100 .7
	
	End


	Event
		EName	InitialSwordFire1
		Type	Local 
		At	WepL
		Part1	CustomizeablePowers\FireControl\FireHands.part
	
	End

End


Condition
	On 	Time
	Time 	45

	Event
		EName	All
		Type	Destroy
	End

End

End

##################################

