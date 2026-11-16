#############################################################
## Header
#############################################################

FxInfo

ClampMaxScale	1 1 1

Flags InheritAnimScale InheritAlpha DontSuppress

LifeSpan 300

#############################################################

Condition
	On 	Time
	Time 	8

	Event
		Type	Local
		At	WepR
		Bhvr	:HandRotateExplosive.bhvr
		Geom	ExplosiveArrow
		LifeSpan 16
	End
End

#############################################################

Condition
	On 	Time
	Time 	24

	Event
		EName 	Prime
		Type	start
		At	WepL
		Bhvr	:projectile.bhvr
		Geom	aExplosiveArrow
		Magnet	Target
		LookAt	Target
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		Type	PositOnly
		At	Target
		ChildFx	WEAPONS\Bow\Archery\ExplosiveArrow.fx
		lifeSpan 70
	End

	Event
		Ename 	Prime
		Type	Destroy
	End
End

#############################################################

End