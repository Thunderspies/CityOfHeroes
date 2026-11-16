#########################################
#  FX file Header:  POLICE BAND SCANNER #
#########################################

FxInfo

Condition
	On 	Time  
	Time 	20

	Event
		Type	Local
		At	WepL
		Sound PPD1 50 50 .4
	End

End




Condition
	On 	Time  
	Time 	20

	Event
		EName	One					#existing event name, not totally clear
		Type	Local
		At	WepL					#location for geometry placement, can be any bone name, "ROOT" or "ORIGIN"
		Geom	GEO_Police_Scanner			#call to geometry fx
	End

End

Condition
	On 	Time  
	Time 	41

	Event
		EName	One					#existing event name, not totally clear
		Type	Local
		At	Mystic					#location for geometry placement, can be any bone name, "ROOT" or "ORIGIN"
		Geom	GEO_PoliceScannerHologram_Interlaced	#call to geometry fx
		Sound PPD1_loop 50 50 .35
	End

	Event
		EName	Two					#existing event name, not totally clear
		Type	Local
		At	Mystic					#location for geometry placement, can be any bone name, "ROOT" or "ORIGIN"
		Geom	GEO_PoliceScannerHologram_Beam		#call to geometry fx
		bhvr	Behaviors/PoliceScannerHologram_Beam.bhvr
	End

	Event
		EName	Three					#existing event name, not totally clear
		Type	Local
		At	Mystic					#location for geometry placement, can be any bone name, "ROOT" or "ORIGIN"
		Geom	GEO_PoliceScannerHologram_Alert		#call to geometry fx
	End

End

End