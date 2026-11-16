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
		
		Bhvr	:GeomFadeinBlue.bhvr
		Geom	PoisonGlassBall

		LifeSpan 60
	End

	Event
		EName	Dust
		Type	Local
		At	wepR
		
		Part	:StunStream.part
		Part	:StunStreamRings.part
		Part	:PowerUpMainStreamRays.part
		Part	:PowerUpMainStreamRaysBright.part

		LifeSpan 70
	End
	
	Event
		EName	SoulSwirls
		Type	Local
		At	wepR
		
		childFx	:InterdementionalSwirls.fx

		LifeSpan 70
	End
	
	Event
		EName	SoulSwirls
		Type	Local
		At	wepR
		
		childFx	:InterdementionalSwirls2.fx

		LifeSpan 70
	End

End


End

	