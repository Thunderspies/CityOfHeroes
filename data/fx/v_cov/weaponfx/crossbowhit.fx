#########################################################
##	template

FxInfo
LifeSpan	105

Input
	Inpname chest
End


Condition
	On Time 
	Time 0
	
	Event
		EName	Prime
		Type 	Local
		At	chest
		geom	Arachnos_Blade_Shooting
		Sound arrowhit 50 50 .67
	End

End

Condition
	On 	Time
	Time	105
	
	Event
		Ename 	All
		Type	Destroy
	End

End



End	

	