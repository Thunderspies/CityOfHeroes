#############################################################
## FX System Name
#############################################################

FxInfo

#############################################################

Condition
	On 	Time  
	Time 	0

	Event
		EName	Prime					#existing event name, not totally clear
		Type	Local
		At	Mystic					#location for geometry placement, can be any bone name, "ROOT" or "ORIGIN"
		Geom	GEO_Coin_02				#call to geometry fx
	End
	
End

#############################################################

End		