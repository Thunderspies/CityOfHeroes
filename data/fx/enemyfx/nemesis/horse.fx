#########################################################
##	This is the actual Horse 
########################################################

FxInfo

Input  
	Inpname	Root
End

Input  
	Inpname	Head
End

Input  
	Inpname	Mystic
End


Flags    InheritAlpha


Condition
	On 	Time
	Time 	1

	Event
		Type	Start
		At	Head
		Anim	FX_Horse
	End
End



End		