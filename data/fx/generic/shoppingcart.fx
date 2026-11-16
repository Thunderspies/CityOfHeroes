
FxInfo

#####  Cement Bucket   #######################################

Condition
	On 	Time  
	Time 	0

	Event
		EName	Prime					#existing event name, not totally clear
		Type	Local
		At	WepR					#location for geometry placement, can be any bone name, "ROOT" or "ORIGIN"
		Geom	WepR_ShoppingCart			#call to geometry fx
		Sound ShoppingCartRoll_Loop 60 60 .4
	End
	
	Event
		EName	Prime					#existing event name, not totally clear
		Type	Local
		At	WepR					#location for geometry placement, can be any bone name, "ROOT" or "ORIGIN"
		Geom	WepR_SHPCRTContents01			#call to geometry fx
		Geom	WepR_SHPCRTContents02			#call to geometry fx
		Geom	WepR_SHPCRTContents03			#call to geometry fx
		Geom	WepR_SHPCRTContents04			#call to geometry fx
		Bhvr	Behaviors/ShpCrtContents.bhvr
		Sound flies_loop 30 30 .25
	End

End

End		