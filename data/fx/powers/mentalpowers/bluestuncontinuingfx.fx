#########################################################
##	TemperatureControl
########################################################

FxInfo

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
		Part1	:HeadDizzies.part
		Part2	:HeadDizzyRings.part
		part3	:MindGlowHit2.part
		Sound mentalhit 80 80 .8
	End


End

End