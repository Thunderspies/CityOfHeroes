#############################################################
## HEADER
#############################################################

FxInfo

Flags InheritAnimScale InheritAlpha DontSuppress

LifeSpan 1000

#### arrow in hand  ########################

Condition
	On	Time
	Time	15

	Event
		Type	Local
		At	WepR
		Sound arrowout 50 50 .32
	End
End

Condition
	On	Time
	Time	110

	Event
		Type	Local
		At	WepR
		Sound manyarrowsfly 90.0 90.0 .7
	End
End

Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	WepR
		Bhvr	:HandRotatea.bhvr
		Geom	NormalArrow
		LifeSpan 94
	End

	Event
		Type	Local
		At	WepR
		Bhvr	:HandRotate1a.bhvr
		Geom	NormalArrow
		LifeSpan 94
	End

	Event
		Type	Local
		At	WepR
		Bhvr	:HandRotate2a.bhvr
		Geom	NormalArrow
		LifeSpan 94
	End

	Event
		Type	Local
		At	WepR
		Bhvr	:HandRotate3a.bhvr
		Geom	NormalArrow
		LifeSpan 94
	End

	Event
		Type	Local
		At	WepR
		Bhvr	:HandRotate4a.bhvr
		Geom	NormalArrow
		LifeSpan 94
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		Ename 	All
		Type	Destroy
	End
End

#############################################################

End