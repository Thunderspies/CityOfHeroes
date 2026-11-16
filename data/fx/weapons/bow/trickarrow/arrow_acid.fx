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
	Time 	9

	Event
		EName 	arrowglass
		Type	Local
		At	WepR
		BHVR	Weapons\Bow\Archery\HandRotateGlass2.bhvr
		Geom	LiquidArrow
		Sound Arrowout5 50 50 .3
		LifeSpan 39
	End

	Event
		EName 	arrowglass1
		Type	Local
		At	arrowglass
		BHVR	:ArrowGlassOffset.bhvr
		Geom	LiquidArrowGlassExploding
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
		BHVR	Weapons\Bow\Archery\projectile.bhvr
		Geom	aLiquidArrow
		Sound Crossbow1 100.0 100.0 .7
		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	P
		Type	Local
		At	Prime
		BHVR	:ArrowGlassOffset.bhvr
		Geom	LiquidArrowGlassExploding
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		EName	arrowHit
		Type	PositOnly
		At	T_chest
		BHVR	Weapons\Bow\Archery\ArrowHit.bhvr
		Geom	NormalArrowHit
		Sound arrowhit 40 40 .44
		Lifespan 44
	End

	Event
		EName 	arrow
		Type	PositOnly
		At	T_Root
		ChildFx	:Arrow_AcidChildFx.fx
		Sound Spewhiss2 90 90 .66
		LifeSpan 20
	End

	Event
		Ename 	Prime
		Type	Destroy
	End

	Event
		Ename 	P
		Type	Destroy
	End

	Event
		EName 	arrowglass1
		Type	Destroy
	End

	Event
		EName 	arrowglass
		Type	Destroy
	End
End

#############################################################

End