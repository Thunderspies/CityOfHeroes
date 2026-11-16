#########################################################
##	TemperatureControl
########################################################

FxInfo

Input  
	Inpname	Origin
End

Input  
	Inpname	root
End


##########################
Condition
	Event
		Type	start 
		At	root
		Geom	ElectricalMeleeBubble
		bhvr	:BumperBubble.bhvr

		Part	:bumper1.part
		Part	:bumper2.part
		Part	:bumper3.part
		Part	:bumper4.part
		Part	:bumper5.part

	End

End


End