#########################################################
##	
########################################################
FxInfo

Input  
	Inpname	Root
End

Input  
	Inpname	LarmR
End

Input  
	Inpname	LarmL
End

Input  
	Inpname	Chest
End


Flags InheritAlpha

##################################################################

Condition
	On	Time
	Time	0
	Event
		EName 	Anim_Prop
		Type	Local
		At	Chest 
		Anim	FX_Titian_Doors_silver

	End

End




End
##################################

