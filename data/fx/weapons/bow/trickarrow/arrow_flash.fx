#############################################################
## Header
#############################################################

FxInfo

Flags InheritAnimScale InheritAlpha DontSuppress

ClampMaxScale	1 1 1

LifeSpan 300

#############################################################

Condition
	On 	Time
	Time 	5

	Event
		EName	Arrow
		Type	Local
		At	WepR
		BHVR	Weapons\Bow\Archery\HandRotateIce.bhvr
		Geom	FlashDazzleArrow
		Sound arrowout2 50 50 .25
		LifeSpan 19
	End

	Event
		EName	FlashDazzleGlass
		Type	Local
		At	Arrow
		altpiv	1
		Geom	FlashDazzleGlass
		LifeSpan 19
	End

	Event
		Type	Local
		At	FlashDazzleGlass
		altpiv	1
		Part	:FlashDazzleArch1.part
		Part	:FlashDazzleArch2.part
		Part	:FlashDazzleArch1.part
		Part	:FlashDazzleArch2.part
		Part	:RazzleFlash.part
		Part	:RazzleFlashRing.part
		Part	:RazzleStar.part
		Sound arrowbeeps3 50 50 .23
		LifeSpan 19
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
		Geom	aFlashDazzleArrow
		Sound Crossbow1 100.0 100.0 .7
		Magnet	Target
		LookAt	Target
	End

	Event
		EName	FlashDazzleGlass1
		Type	Local
		At	Prime
		altpiv	1
		Geom	FlashDazzleGlass
	End

	Event
		EName	FlashDazzleGlass2
		Type	Local
		At	FlashDazzleGlass1
		altpiv	1
		Part	:FlashDazzleArch1.part
		Part	:FlashDazzleArch2.part
		Part	:FlashDazzleArch1.part
		Part	:FlashDazzleArch2.part
		Part	:RazzleFlash.part
		Part	:RazzleFlashRing.part
		Part	:RazzleStar.part
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		EName	Prime
		Type	Destroy
	End

	Event
		EName	FlashDazzleGlass1
		Type	Destroy
	End

	Event
		EName	FlashDazzleGlass2
		Type	Destroy
	End

	Event
		EName	Fire2
		Type	PositOnly
		At	T_chest
		BHVR	Weapons\Bow\Archery\ArrowHit.bhvr
		Geom	NormalArrowHit
		Sound FlashArrowHit 100 100 .8
		Lifespan 44
	End

	Event
		ENAME	HandGlow
		Type	PositOnly
		At	T_root
		Bhvr	Behaviors\CameraShake01.bhvr
		part	:FlashBangHandGlow.part
		part	:FlashBangFlash.part
		LifeSpan 10
	End

	Event
		ENAME	ThunderShake1
		Type	PositOnly
		At	T_root
		Bhvr    Behaviors\Fade.bhvr
		part	:FlashBangFlashFlat.part
		part	:FlashBangGlows.part
		LifeSpan 15
	End

	Event
		ENAME	ThunderShake1
		Type	PositOnly
		At	T_root
		part	:FlashBangCrazyGlows.part
		LifeSpan 30
	End
End

#############################################################

End