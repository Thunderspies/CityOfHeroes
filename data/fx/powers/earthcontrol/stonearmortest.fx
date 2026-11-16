#########################################################
##	template

FxInfo

Input  
	Inpname	Root
End




#########################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	Monkey
		Type	Local
		At	Root
		Anim	FX_stonearmor
	End

	
End


Condition
	On 	Time
	Time 	82
	
	Event
		Type	Destroy
		EName	All
	End
End	

End		