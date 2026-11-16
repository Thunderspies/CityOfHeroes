#############################################################
## Header
#############################################################

FxInfo

Flags InheritAnimScale InheritAlpha DontSuppress

LifeSpan 300

#############################################################

#Condition
#	On	Time
#	Time	15
#
#	Event
#		Type	Local
#		At	WepR
#		Sound	arrowout3 50 40 .25
#	End
#End

Condition
	On 	Time
	Time 	9

	Event
		EName 	arrow
		Type	Local
		At	WepR
		Bhvr	:HandRotateFlame.bhvr
		Geom	HighDamageArrow
		LifeSpan 39
	End

	Event
		EName 	fire
		Type	Local
		At	arrow
		altpiv	1
		Part	:fire.part
		Part	:fire.part
		Part	:fire.part
		LifeSpan 39
	End

End

#############################################################

Condition
	On 	Time
	Time 	48

	Event
		EName 	Prime
		Type	start
		At	WepL
		Bhvr	:projectile.bhvr
		Geom	aHighDamageArrow
		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	fire1
		Type	Local
		At	Prime
		altpiv	1
		Part	:fire2.part
		Part	:fire2.part
		Part	:fire2.part
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