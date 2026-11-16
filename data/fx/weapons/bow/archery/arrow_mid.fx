#############################################################
## Header
#############################################################

FxInfo

Flags InheritAnimScale InheritAlpha DontSuppress

LifeSpan 300

## PRE-FIRE ###########################################################

Condition
	On 	Time
	Time 	8

	Event
		Type	Local
		At	WepR
		Bhvr	:HandRotate.bhvr
		Geom	NormalArrow
		LifeSpan 16
	End
End

## ARROW PROJECTILE ###########################################################

Condition
	On 	Time
	Time 	24

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

## CLEAN UP ###########################################################

Condition
	On 	PrimeHit

	Event
		Ename 	Prime
		Type	Destroy
	End
End

#############################################################

End

