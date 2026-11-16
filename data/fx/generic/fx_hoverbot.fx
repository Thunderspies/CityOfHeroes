#########################################################
##	
########################################################
FxInfo

Input  
	Inpname	Hips
End

Input  
	Inpname	Root
End

Flags	InheritAlpha

##################################################################

Condition
	On	Time
	Time	0
	Event
		EName 	Prop
		Type	Local
		At	Hips
		Anim	FX_hoverbot

	End

End

End


