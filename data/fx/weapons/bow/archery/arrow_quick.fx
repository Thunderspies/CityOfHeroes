#############################################################
## Header
#############################################################

FxInfo

Flags InheritAnimScale InheritAlpha DontSuppress

LifeSpan 300

#############################################################

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	WepR
		Sound arrowout2 50 50 .25
	End
End

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	WepR
		Bhvr	:HandRotate.bhvr
		Geom	NormalArrow
		LifeSpan 12
	End
End

#############################################################

Condition
	On 	Time
	Time 	22

	Event
		EName 	Prime
		Type	start
		At	WepL
		Bhvr	:projectile.bhvr
		Geom	aNormalArrow
		Magnet	Target
		LookAt	Target
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		Ename 	Prime
		Type	Destroy
	End
End

#############################################################

End