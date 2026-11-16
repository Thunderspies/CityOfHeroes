#############################################################
## Header
#############################################################

FxInfo

ClampMinScale 0.875 0.875 0.875
ClampMaxScale 1.125 1.125 1.125

Lifespan 44

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	ArrowHitHookup
		Type	create
		Inherit	Position
		Update	Position
		At	CHEST
		LookAt	Target
	End

	Event
		EName	ArrowHit
		Type	create
		Inherit	Position rotation
		Update	Position
		At	ArrowHitHookup
		Bhvr	:ArrowsHitFade.bhvr
		Geom	NormalArrowHit
		Sound arrowhit2 80 80 .7
		Lifespan	44
	End
End

#############################################################

End