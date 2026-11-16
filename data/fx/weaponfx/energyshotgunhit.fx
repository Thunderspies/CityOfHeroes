#########################################################
##	template

FxInfo
LifeSpan	100

Input
	Inpname Chest
End


Condition
	On Time 
	Time 45
	
	Event
		EName	EnergyGunHit
		Type 	Local
		At	Chest
		Part1	:EnergyGunHit.part
	End

End

Condition
	On 	Time
	Time	100
	
	Event
		Ename 	all
		Type	Destroy
	End

End



End	

	