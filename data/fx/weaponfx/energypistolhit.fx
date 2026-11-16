#########################################################
##	template

FxInfo
LifeSpan	115

Input
	Inpname chest
End


Condition
	On Time 
	Time 55
	
	Event
		EName	EnergyGunHit
		Type 	Local
		At	chest
		Part1	:EnergyGunHit.part
	End

End

Condition
	On 	Time
	Time	115
	
	Event
		Ename 	all
		Type	Destroy
	End

End



End	

	