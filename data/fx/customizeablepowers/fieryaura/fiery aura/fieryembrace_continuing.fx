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

Input  
	Inpname	Origin
End



Condition
	On 	Time
	Time 	0

	Event
		EName	InitialSwordFire
		Type	Local 
		At	WepR
		Part1	CustomizeablePowers\FieryAura\FireHandz.part
		Sound Firehand_loop 100.0 100.0 .55
		
		#LifeSpan	60
	
	End


	Event
		EName	InitialSwordFire1
		Type	Local
		At	WepL
		Part1	CustomizeablePowers\FieryAura\FireHandz.part
		Sound Ignite2 100.0 100.0 .9
	
		#LifeSpan	60
	End

	


End

End

##############################################################

