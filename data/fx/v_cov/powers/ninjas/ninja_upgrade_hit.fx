
FxInfo

Flags	InheritAnimScale

Lifespan 90

#######  wep in ground   ############################

Condition
	On	Time
	Time	0

	Event
		EName	prop_dirt
		Type	Local
		At	Mystic
		Geom	GEO_WepL_Katana_Point
		Lifespan	25
	End
End


####### Pickup wep  #################################


Condition
	On	Time
	Time	25

	Event
		EName	prop
		Type	Local
		At	WepL
		Geom	GEO_WepL_Katana_3	
	End
End



End