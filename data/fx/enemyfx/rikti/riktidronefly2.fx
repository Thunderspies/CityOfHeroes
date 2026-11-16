#########################################################
##	Martial Arts Hit FX
########################################################
FxInfo

Input  
	Inpname	Origin
End

LifeSpan	120


Condition
	On 	Time
	Time 	0

	Event
		Type	local 
		At	Origin
		Sound riktidronewhoosh1 100.0 100.0 .7
		
	End

End

End
