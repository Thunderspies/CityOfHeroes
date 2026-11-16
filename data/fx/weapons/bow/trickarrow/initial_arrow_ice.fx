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
		Sound Icearrow 60 60 .66
	End
End



Condition
	On 	Time
	Time 	35

	Event
		EName	arrow
		Type	Local
		At	WepR
		BHVR	Weapons/Bow/Archery/HandRotateIce.bhvr
		Geom	IceArrow
		LifeSpan 46
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
		
		LifeSpan 46
		
	End

End

####  projectile arrow  ##############################

Condition
	On 	Time
	Time 	81

	
	Event
		EName 	Prime
		Type	start
		At	WepL
		Bhvr	Weapons/Bow/Archery/projectile.bhvr
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

Condition
	On 	PrimeHit

	
	Event
		EName	Fire2
		Type	PositOnly
		At	T_chest
			
		BHVR	Weapons/Bow/Archery/ArrowHit.bhvr
		Geom	NormalArrowHit
		
		Lifespan	44


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

End			