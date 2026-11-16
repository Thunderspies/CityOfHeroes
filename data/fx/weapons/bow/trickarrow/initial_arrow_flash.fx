#########################################################
#	template

FxInfo

Flags InheritAnimScale InheritAlpha

LifeSpan 90


### arrow in hand  ########################

Condition
	On	Time
	Time	32

	Event	
		Type	Local
		At	WepR
		Sound arrowout2 50 50 .25
	End
End

Condition
	On	Time
	Time	45

	Event	
		Type	Local
		At	WepR
		Sound arrowbeeps 50 50 .2
	End
End


Condition
	On	Time
	Time	82

	Event	
		Type	Local
		At	WepR
		Sound Crossbow1 100.0 100.0 .7
	End
End


Condition
	On	Time
	Time	86

	Event	
		Type	Local
		At	WepR
		Sound FlashArrowHit 100 100 .8
	End
End
Condition
	On 	Time
	Time 	35

	Event
		EName	Arrow
		Type	Local
		At	WepR
		BHVR	Weapons/Bow/Archery/HandRotateIce.bhvr
		Geom	FlashDazzleArrow
		LifeSpan 46
	End
	
	Event
		EName	FlashDazzleGlass
		Type	Local
		At	Arrow
		altpiv	1
		Geom	FlashDazzleGlass
		LifeSpan 46
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

		LifeSpan 46
	End

End

###  projectile arrow  ##############################

Condition
	On 	Time
	Time 	81

	
	Event
		EName 	Prime
		Type	start
		At	WepL
		Bhvr	Weapons/Bow/Archery/projectile.bhvr
		Geom	aFlashDazzleArrow
		#Sound 	Crossbow1 100.0 30.0 .7

		Magnet	Target
		LookAt	Target
	End
	
	Event
		EName	FlashDazzleGlass1
		Type	Local
		At	Prime
		altpiv	1
		Geom	FlashDazzleGlass
		LifeSpan 46
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

		LifeSpan 46
	End
	
End

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

###############################################################################################################

	Event
		EName	Fire2
		Type	Local
		At	T_chest
			
		BHVR	Weapons/Bow/Archery/ArrowHit.bhvr
		Geom	NormalArrowHit
		
		Lifespan	44


	End

	Event	
		Type	Local
		At 	T_chest
		

	End

	Event	
		ENAME	HandGlow
		Type	Local
		At	T_root
		part	:FlashBangHandGlow.part
		LifeSpan 7	
	End

	Event	
		ENAME	ThunderShake1
		Type	Local
		At	T_root
		part	:FlashBangHandGlow.part
		LifeSpan 10	
	End

	Event	
		ENAME	ThunderShake1
		Type	Local
		At	T_root
		part	:FlashBangGlows.part
		LifeSpan 20	
	End

	Event	
		ENAME	ThunderShake1
		Type	Local
		At	T_root
		part	:FlashBangCrazyGlows.part
		LifeSpan 30	
	End

	Event	
		ENAME	Flash
		Type	Local
		At	T_root
		part	:FlashBangFlash.part
		LifeSpan 10	
	End
	
	
	Event	
		ENAME	Flash
		Type	Local
		At	T_root
		
		Bhvr    Behaviors/Fade.bhvr

		#ChildFx	:arrow_flashPrimeHitChild.fx

		part	:FlashBangFlashFlat.part
		LifeSpan 15	
	End

	Event	
		ENAME	ThunderShake1
		Type	Local
		At	T_root
		Bhvr	Behaviors/CameraShake01.bhvr
		LifeSpan 1
		
		
	End

End

End			