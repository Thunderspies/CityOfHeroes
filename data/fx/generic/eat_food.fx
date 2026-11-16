
FxInfo

####################
#  EAT Food - FX  #
####################

Condition
	On 	Time  
	Time 	0

	Event
		EName	Prime					#existing event name, not totally clear
		Type	Local
		At	WepL					#location for geometry placement, can be any bone name, "ROOT" or "ORIGIN"
		Geom	GEO_Hamburger				#call to geometry fx
		Geom	GEO_HotDog				#call to geometry fx
		Geom	GEO_Sandwich				#call to geometry fx
	End
	
End

End		