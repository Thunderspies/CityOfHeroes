#########################################################
##	template

FxInfo

Flags InheritAnimScale InheritAlpha

LifeSpan 300


#### arrow in hand  ########################

Condition
	On	Time
	Time	32

	Event
		Type	Local
		At	WepR
		Sound Arrowout6 50 50 .54
	End
End


Condition
	On 	Time
	Time 	35

	Event
		Ename	Sonic
		Type	Local
		At	WepR
		BHVR	Weapons/Bow/Archery/HandRotate.bhvr
		Geom	SonicConeArrow
		
		LifeSpan 23
	End
	
	Event
		Type	Local
		At	Sonic
		Altpiv	1
		BHVR	:SonicOffset.bhvr
		Part	:SonicArrowRing.part
		Part	:SonicArrowGlowRing.part

		LifeSpan 7
	End

End

####  projectile arrow  ##############################

Condition
	On 	Time
	Time 	61

	
	Event
		EName 	Prime
		Type	start
		At	WepL
		BHVR	Weapons/Bow/Archery/projectile.bhvr
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

Condition
	On 	PrimeHit

	Event
		EName 	arrow
		Type	start
		At	root
		Part	:SonicCoreRingsOut.part
		Part	:SonicCoreRings.part
		Part	:SonicRingsOut.part
		Part	:SonicRingsOut2.part
		LifeSpan	2
	End


	Event
		EName 	arrow
		Type	start
		At	T_root
		Part	:SonicPetArrowRing2.part
		Part	:SonicPetArrowGlowRing2.part
	End

	Event
		Ename 	Prime
		Type	Destroy
	End

End

End			