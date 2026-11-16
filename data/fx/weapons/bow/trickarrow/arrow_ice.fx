#############################################################
## Header
#############################################################

FxInfo

Flags InheritAnimScale InheritAlpha DontSuppress


ClampMinScale 0.75 0.75 0.75
ClampMaxScale 1.25 1.25 1.25

LifeSpan 300

#############################################################

Condition
	On 	Time
	Time 	8

	Event
		EName	arrow
		Type	Local
		At	WepR
		BHVR	Weapons\Bow\Archery\HandRotateIce.bhvr
		Geom	IceArrow
		Sound Icearrow 60 60 .66
		LifeSpan 16
	End

	Event
		EName 	point1
		Type	Local
		At	arrow
		altpiv	1
		Part	:SnowSkatter.part
		Part	:ColdMist.part
		Part	:IceFlash.part
		Part	:IceFlashRing.part
		Part	:IceFlashStars.part
		LifeSpan 39
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
		Bhvr	Weapons\Bow\Archery\projectile.bhvr
		Geom	aIceArrow
		Sound Crossbow1 100.0 100.0 .7
		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	point1
		Type	Local
		At	Prime
		altpiv	1
		Part	:SnowSkatter2.part
		Part	:SnowSkatter2.part
		Part	:SnowSkatter2.part
		Part	:ColdMist2.part
		Part	:IceFlash.part
		Part	:IceFlashRing2.part
		Part	:IceFlashStars2.part
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		EName	Fire2
		Type	PositOnly
		At	T_chest
		BHVR	Weapons\Bow\Archery\ArrowHit.bhvr
		Geom	NormalArrowHit
		Lifespan 44
	End

	Event
		Ename 	Prime
		Type	Destroy
	End

	Event
		EName 	point1
		Type	Destroy
	End
End

#############################################################

End