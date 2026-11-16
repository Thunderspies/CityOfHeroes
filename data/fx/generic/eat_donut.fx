
FxInfo

####################
#  EAT DONUT - FX  #
####################

Condition
	On 	Time  
	Time 	0

	Event
		EName	Prime					#existing event name, not totally clear
		Type	Local
		At	WepL					#location for geometry placement, can be any bone name, "ROOT" or "ORIGIN"
		Geom	GEO_Donut				#call to geometry fx
	End
	
End

End