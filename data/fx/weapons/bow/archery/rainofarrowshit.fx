#########################################################
##	RadiationAura
FxInfo

Lifespan	174

##############################################################################################

Condition
	On		Time
	Time		0

	Event
		EName	ArrowHitHookup
		Type	Create
		At	CHEST
		Inherit	Position
		Update	Position
		LookAt	Target
		Sound	arrowhit2 50 50 .8
	End
End

Condition
	On	Time
	Time	4
	Repeat	1

	Event
		Type	Local
		At	ArrowHitHookup
		Bhvr	:ArrowRainHit.bhvr
		Geom	NormalArrowHit
		Lifespan 44
	End

End



Condition
	Time 5
	Chance	.2


	Event
		Type	Local
		At	ArrowHitHookup
		Bhvr	:ArrowRainHit.bhvr
		Geom	NormalArrowHit
		Lifespan 44
	End

End

Condition
	Time 6
	Chance	.2


	Event
		Type	Local
		At	ArrowHitHookup
		Bhvr	:ArrowRainHit.bhvr
		Geom	NormalArrowHit
		Lifespan 44
	End

End

Condition
	Time 9
	Chance	.2


	Event
		Type	Local
		At	ArrowHitHookup
		Bhvr	:ArrowRainHit.bhvr
		Geom	NormalArrowHit
		Lifespan 44
	End

End

Condition
	Time 13
	Chance	.2


	Event
		Type	Local
		At	ArrowHitHookup
		Bhvr	:ArrowRainHit.bhvr
		Geom	NormalArrowHit
		Lifespan 44
	End
End



End