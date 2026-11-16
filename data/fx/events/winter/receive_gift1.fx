FxInfo

#############################################################
## Offer_Gift1.fx
#############################################################

Lifespan	150

Condition
	On 	Time
	Time 	2

	Event
		EName	Gift_Bottom
		Type	Local
		At	WepR
		Geom	GEO_HolidayGift1_Bottom
	End

	Event
		EName	Gift_Top
		Type	Local
		At	WepR
		Geom	GEO_HolidayGift1_Top
		BhvrOverride
			PositionOffset		0 0.4 0
	End

End