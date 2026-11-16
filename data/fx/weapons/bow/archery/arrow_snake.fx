#############################################################
## HEADER
#############################################################

FxInfo

Flags InheritAnimScale InheritAlpha

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

#############################################################

Condition
	On 	Time
	Time 	9

	Event
		EName 	arrow
		Type	Local
		At	WepR
		Bhvr	:HandRotateFlame.bhvr
		Geom	FX_SnakeArrow
		LifeSpan 38
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
		Geom	FX_SnakeArrow_Projectile
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