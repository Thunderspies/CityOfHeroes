#########################################################
##	jack
##
FxInfo
Flags    InheritAlpha	
LifeSpan 0

Input  
	InpName	Waist
End


Condition
	On 	Time
	Time 	1

	Event
		EName 	Prime
		Type	Local 
		At	Waist
		Anim	arachnos_minion_sideguns ##ent type

	End
End

End	



			