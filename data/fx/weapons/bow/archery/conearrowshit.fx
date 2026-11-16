#############################################################
## HEADER
#############################################################

FxInfo

ClampMinScale 0.875 0.875 0.875
ClampMaxScale 1.125 1.125 1.125

Lifespan 60

#############################################################

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
		Sound	arrowhit2 50 50 .5
	End
End

Condition
	On		Time
	Time		1
	Repeat		1

	Event
		Type	Local
		At	ArrowHitHookup
		Bhvr	:ArrowsHit.bhvr
		Geom	NormalArrowHit
		Lifespan 44
	End
End

Condition
	On		Time
	Time		1
	Chance		0.5

	Event
		Type	Local
		At	ArrowHitHookup
		Bhvr	:ArrowsHit.bhvr
		Geom	NormalArrowHit
		Lifespan 44
	End
End

Condition
	On		Time
	Time		1
	Chance		0.5

	Event
		Type	Local
		At	ArrowHitHookup
		Bhvr	:ArrowsHit.bhvr
		Geom	NormalArrowHit
		Lifespan 44
	End
End

Condition
	On		Time
	Time		1
	Chance		0.5

	Event
		Type	Local
		At	ArrowHitHookup
		Bhvr	:ArrowsHit.bhvr
		Geom	NormalArrowHit
		Lifespan 44
	End
End

#############################################################

End