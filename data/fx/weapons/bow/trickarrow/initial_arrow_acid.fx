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
		Sound Arrowout5 50 50 .3
	End
End





Condition
	On 	Time
	Time 	35

	Event
		EName 	arrowglass
		Type	Local
		At	WepR
		BHVR	Weapons/Bow/Archery/HandRotateGlass2.bhvr
		Geom	LiquidArrow
		LifeSpan 46
	End
	
	Event
		EName 	arrowglass1
		Type	Local
		At	arrowglass
		BHVR	:ArrowGlassOffset.bhvr
		Geom	LiquidArrowGlassExploding
		
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
		BHVR	Weapons/Bow/Archery/projectile.bhvr
		Geom	aLiquidArrow
		Sound Crossbow1 100.0 100.0 .7

		Magnet	T_hips
		LookAt	T_hips
	End
	
	Event
		EName 	P
		Type	Local
		At	Prime
		BHVR	:ArrowGlassOffset.bhvr
		Geom	LiquidArrowGlassExploding
		
		
	End

End

Condition
	On 	PrimeHit
	
	Event
		EName	arrowHit
		Type	Local
		At	T_chest
		Sound arrowhit 40 40 .44
		BHVR	Weapons/Bow/Archery/ArrowHit.bhvr
		Geom	NormalArrowHit
		
		Lifespan	44


	End

	Event
		Type	start
		At	T_Root
		
		BHVR	Behaviors/RootToChestAdjustment.bhvr
		Sound Spewhiss2 90 90 .66
		Part	:Acid.part
		Part	:AcidYellow.part
		
		LifeSpan 8

	End

	
	Event
		Type	start
		At	T_Root

		Part	:AcidDrops.part
		Part	:AcidDropsDark.part
		
		LifeSpan 20

	End

	Event
		EName 	arrow
		Type	start
		At	T_Root

		Part	:AcidFlat.part
		Part	:AcidFlatDark.part
		Part	:AcidBubbles.part
		Part	:AcidBubblesdark.part
		Part	:AcidBubble.part
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

End			