
FxInfo

#####  Jackhammer   #######################################

Condition
	On 	Time  
	Time 	0

	Event
		EName	Prop					#existing event name, not totally clear
		Type	Local
		At	WepR					#location for geometry placement, can be any bone name, "ROOT" or "ORIGIN"
		Geom	WepR_Jackhammer				#call to geometry fx
	End
End


Condition
	On	Time
	Time	0

	Event	
		Type	Local
		At	WepR
#		Sound	jackhammer2_loop 42 40 .55
	End
End



End		