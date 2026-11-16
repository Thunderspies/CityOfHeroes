#############################################################
## Child_CardsHandsSwirlOut.fx
#############################################################

FxInfo

Lifespan 150

#############################################################

Condition
	On	Cycle
	Time	2

	Event
		Type	Start
		At	Origin
		Bhvr	:CardsHandsFadeIn.bhvr
		Geom	TarotCard_Death
		Geom	TarotCard_TheTower
		Geom	TarotCard_TheFool
		Geom	TarotCard_TheHermit
		Geom	TarotCard_TheLovers
		Geom	TarotCard_TheMagician
		Geom	TarotCard_TheSun
		Geom	TarotCard_TheMoon
		Geom	TarotCard_WheelOfFortune
		Magnet	Waist
		Lifespan 45
		LifespanJitter 30
	End
End

#############################################################

End