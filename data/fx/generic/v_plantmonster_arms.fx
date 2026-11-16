#########################################################
##	jack
##
FxInfo
	
Flags  InheritAlpha   InheritAnimScale

LifeSpan 0

Input  
	InpName	hips
End


Condition
	On 	Time
	Time 	1

	Event
		EName 	Prime
		Type	Local 
		At	hips
		Anim	V_Plant_Monster_Arms ##ent type

	End
End

End	



			