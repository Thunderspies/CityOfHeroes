
FxInfo

#####  GarbageBag Carriable   #######################################

Condition
	On 	Time  
	Time 	0

	Event
		EName	Prime					#existing event name, not totally clear
		Type	Local
		At	WepR					#location for geometry placement, can be any bone name, "ROOT" or "ORIGIN"
		Geom	WepR_GarbageBag				#call to geometry fx
		Sound PlasticBacRustle_Loop 50 50 .4
	End
	
End

End		