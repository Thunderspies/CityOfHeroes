#########################################################
##	TemperatureControl
########################################################

FxInfo

LifeSpan	205

Input  
	Inpname	root
End

Input  
	Inpname	head
End
##########################
Condition
	On 	Time
	Time 	0

	Event
		EName	core
		Type	Local
		At	head
		Geom	LocalPivot01
		Part1	POWERS\MentalPowers\HeadDizzies.part
		Part2	POWERS\MentalPowers\HeadDizzyRings.part
		part3	POWERS\MentalPowers\MindGlowHit2.part
		Sound mentalhit 80 80 .8
	End


End


Condition
	On 	Time
	Time	250
	
	Event
		Ename 	all
		Type	Destroy
	End

End



End