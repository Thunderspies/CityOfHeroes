#########################################################
##	TemperatureControl
########################################################

FxInfo

##########################
Condition
	On 	Time
	Time 	0

	Event
		EName	core
		Type	Local
		At	head
		Geom	LocalPivot01
		bhvr	:Offset.bhvr
	End

	Event
		EName	core3
		Type	Local
		At	core
		  Altpiv   4
		part	:SonicRingCore.part
		part	:SonicRing.part		
		
	
	End

End




End