FxInfo

##############################
##  Open Holiday Gift - FX  ##
##############################

Lifespan	150

Condition
	On 	Time  
	Time 	10

	Event
		EName	Gift_Top
		Type	Local
		At	Mystic
		Geom	GEO_HolidayGift2_Bottom
		bhvr	Behaviors/HolidayGift.bhvr
	End

	Event
		EName	Gift_Bottom
		Type	Local
		At	WepL
		Geom	GEO_HolidayGift2_Top
		bhvr	Behaviors/HolidayGift.bhvr
	End

End

Condition
	On 	Time
	Time 	70

	Event
		Ename	Center
		Type	Local 
		At	Mystic
		ChildFX	fx/Explosions/Confetti/XmasGiftExplosion.fx
	End