#########################################################
##	template

FxInfo

Input  
	Inpname	Root
End

Input
	Inpname Waist
End

Input
	Inpname Chest
End

Input
	Inpname Head
End
#########################################################

Condition
	On 	Time
	Time 	1

	Event
		Type	Local
##		Bhvr	behaviors/speed.bhvr
		At	Root
		Anim	FX_teleattack
	End
End

Condition
	On 	Time
	Time 	90
	
	Event
		Type	Destroy
		EName	All
	End
End	

End		