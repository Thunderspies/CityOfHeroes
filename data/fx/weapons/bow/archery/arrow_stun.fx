#############################################################
## Header
#############################################################

FxInfo

ClampMinScale 0.875 0.875 0.875
ClampMaxScale 1.125 1.125 1.125

Flags InheritAnimScale InheritAlpha DontSuppress

LifeSpan 300

#############################################################

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	WepR
		Bhvr	:HandRotateFlame.bhvr
		Geom	BluntArrow
		Sound arrowout2 50 50 .25
		LifeSpan 16
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
		Geom	aBluntArrow
		Magnet	Target
		LookAt	Target
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		Type	start
		At	T_Chest
		Bhvr	:StunArrowHit.bhvr
		Geom	BluntArrowHit
		LifeSpan 70
	End

	Event
		Ename 	prime
		Type	Destroy
	End
End

#############################################################

End