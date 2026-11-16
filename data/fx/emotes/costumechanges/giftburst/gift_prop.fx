FxInfo

#############################################################
## Gift_Burst1.fx
#############################################################



Lifespan	35

Condition
	On 	Time
	Time 	10

	Event
		EName	Gift_TopGift_Bottom
		Type	Local
		At	Mystic
		Geom	GEO_HolidayGift1_Bottom
		BhvrOverride
			scale			5 5 5
		End
	End

	Event
		EName	Gift_Top
		Type	Local
		At	Mystic
		Geom	GEO_HolidayGift1_Top
		BhvrOverride
			scale			5 5 5
			PositionOffset		0 2 0
		End
	End

End