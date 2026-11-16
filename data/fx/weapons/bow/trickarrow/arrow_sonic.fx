#############################################################
## Header
#############################################################

FxInfo

Flags InheritAnimScale InheritAlpha DontSuppress

ClampMaxScale 1 1 1

LifeSpan 300

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		EName	Sonic
		Type	Local
		At	WepR
		BHVR	Weapons\Bow\Archery\HandRotate.bhvr
		Geom	SonicConeArrow
		Sound Arrowout6 50 50 .54
		LifeSpan 17
	End

	Event
		Type	Local
		At	Sonic
		Altpiv	1
		BHVR	:SonicOffset.bhvr
		Part	:SonicArrowRing.part
		Part	:SonicArrowGlowRing.part
		LifeSpan 10
	End
End

#############################################################

Condition
	On 	Time
	Time 	27

	Event
		EName 	Prime
		Type	start
		At	WepL
		BHVR	Weapons\Bow\Archery\projectile.bhvr
		Geom	aSonicConeArrow
		Sound Crossbow1 100.0 100.0 .7
		Magnet	Target
		LookAt	Target
	End

	Event
		Type	Local
		At	Prime
		Altpiv	1
		BHVR	:SonicOffsetProjectile.bhvr
		Part	:SonicArrowRing.part
		Part	:SonicArrowGlowRing.part
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		EName 	arrow
		Type	local
		At	T_root
		childFx	WEAPONS\Bow\TrickArrow\SonicArrowHit.fx
	End

	Event
		Ename 	Prime
		Type	Destroy
	End
End

#############################################################

End