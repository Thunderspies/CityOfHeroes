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
		EName	arrowglass
		Type	Local
		At	WepR
		BHVR	Weapons\Bow\Archery\HandRotateGlass.bhvr
		Geom	LiquidArrow
		Sound Arrowout5 50 50 .2
		LifeSpan 17
	End

	Event
		Type	Local
		At	arrowglass
		BHVR	:ArrowGlassOffset.bhvr
		Geom	LiquidArrowGlassExploding
		LifeSpan 17
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
		Bhvr	Weapons\Bow\Archery\projectile.bhvr
		Geom	aLiquidArrow
		Sound Crossbow1 100.0 100.0 .7
		Magnet	T_root
		LookAt	T_Root
	End

	Event
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
		EName   RadiusFxScale
		Type	PositOnly
		At	T_Root
		Bhvr	powers\firecontrol\Inferno.bhvr

	End

	Event
		EName 	Prime1
		Type	start
		At	RadiusFxScale
		bhvr    :Infernobubble.bhvr
		Part	:PoisonYellow2.part
		Part	:PoisonGlows.part
	End

	Event
		EName	Prime
		Type	Destroy
	End
End

#############################################################

End

