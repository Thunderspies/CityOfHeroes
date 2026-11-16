#########################################################
##	
##

FxInfo

LifeSpan	100

COndition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		Sound InterdimensionalHand 70 70 .8
	End
End


Condition
	On 	Time
	Time 	20

	Event
		EName	Dust
		Type	Local
		At	wepR
		
		Bhvr	:GeomFadeinRed.bhvr
		Geom	PoisonGlassBall

		LifeSpan 60
	End

	Event
		EName	Dust
		Type	Local
		At	wepR
		
		Part	:StunStream.part
		Part	:StunStreamRingsRed.part
		Part	:PowerUpMainStreamRays.part
		Part	:PowerUpMainStreamRaysBright.part

		LifeSpan 70
	End

	Event
		EName	SoulSwirls
		Type	Local
		At	WepR
		
		childFx	:BoostSwirlsReda.fx

		LifeSpan 70
	End
	
	Event
		EName	SoulSwirls
		Type	Local
		At	WepR
		
		childFx	:BoostSwirlsRed2a.fx

		LifeSpan 70
	End

End


End

	