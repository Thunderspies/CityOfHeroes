
FxInfo

#######################
#  TEABAG EMOTE - FX  #
#######################

Condition
	On 	Time  
	Time 	0

	Event
		EName	Prime					#existing event name, not totally clear
		Type	Local
		At	WepR					#location for geometry placement, can be any bone name, "ROOT" or "ORIGIN"
		Geom	GEO_Teabag				#call to geometry fx
	End
	
	Event
		EName	Prime					#existing event name, not totally clear
		Type	Local
		At	WepL					#location for geometry placement, can be any bone name, "ROOT" or "ORIGIN"
		Geom	GEO_Teacup				#call to geometry fx
	End

End

Condition
	On 	Time  
	Time 	10
	
	Event
		EName	Prime					#existing event name, not totally clear
		Type	Local
		At	WepL					#location for geometry placement, can be any bone name, "ROOT" or "ORIGIN"
		BhvrOverride
			PositionOffset 0.0 0.25 0.00
		End
		Part1	:teabag.part
	End


End

End		