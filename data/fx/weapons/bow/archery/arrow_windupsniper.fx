#############################################################
## HEADER
#############################################################

FxInfo

Flags InheritAnimScale InheritAlpha DontSuppress

LifeSpan 300

######## arrow projectile  ##########################

Condition
	On 	Time
	Time 	3

	Event
		EName 	Prime
		Type	start
		At	WepL
		Bhvr	:projectile.bhvr
		Geom	aNormalArrow
		Sound bowtwang3 100.0 100.0 .7
		Magnet	Target
		LookAt	Target
	End
End

Condition
	On 	Time
	Time 	0
	Event
		Type	Local
		At	WepR
		Bhvr	:ArrowGlowOffset.bhvr
		Part	:AimFlash.part
		Part	:AimFlashCircle.part
		Part	:AimFlashRing.part
		Part	:AimFlashRing.part
		Part	:AimFlashStars.part
		Sound arrowsniper2 50 50 .14
		LifeSpan 1
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