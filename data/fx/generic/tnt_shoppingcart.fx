
FxInfo

#####  ShoppingCart w/ TNT   #######################################

Condition
	On 	Time  
	Time 	0

	Event
		EName	Prime					#existing event name, not totally clear
		Type	Local
		At	WepR					#location for geometry placement, can be any bone name, "ROOT" or "ORIGIN"
		Geom	WepR_ShoppingCart			#call to geometry fx
	End
	
	Event
		EName	Prime					#existing event name, not totally clear
		Type	Local
		At	WepR					#location for geometry placement, can be any bone name, "ROOT" or "ORIGIN"
		Geom	ShoppingCart_TNT			#call to geometry fx

	End

End

End		