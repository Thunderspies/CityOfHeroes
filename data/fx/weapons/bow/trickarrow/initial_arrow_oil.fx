#########################################################
##	template

FxInfo

Flags InheritAnimScale InheritAlpha

LifeSpan 1000

#### arrow in hand  ########################

Condition
	On	Time
	Time	33

	Event	
		Type	Local
		At	WepR
		Sound arrowout4 50 50 .39
	End
End



Condition
	On 	Time
	Time 	35

	Event
		EName	arrowglass
		Type	Local
		At	WepR
		BHVR	Weapons/Bow/Archery/HandRotateGlass.bhvr
		Geom	LiquidArrow
		LifeSpan 23
	End
	
	Event
		Type	Local
		At	arrowglass
		BHVR	:ArrowGlassOffset.bhvr
		Geom	ArrowGlassOil
		
		LifeSpan 23
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
		Bhvr	Weapons/Bow/Archery/projectile.bhvr
		Geom	aLiquidArrow
		Sound Crossbow1 100.0 100.0 .7

		Magnet	Target
		LookAt	Target
	End
	
	Event
		EName	PrimeGlass
		Type	Local
		At	Prime
		BHVR	:ArrowGlassOffset.bhvr
		Geom	ArrowGlassOil
		
	End
	
End

Condition
	On 	PrimeHit

	Event
		EName   RadiusFxScale
		Type	local
		At	T_Root
		Bhvr	powers/firecontrol/Inferno.bhvr
	End


	Event
		Type	start 
		At	RadiusFxScale
		bhvr    :Infernobubble.bhvr
		Part	:OilSplash.part
		Part	:OilSplashTechnocolors.part
		Part	:OilSplashTechnocolors2.part
		
	End

	Event
		Ename 	Prime
		Type	Destroy
	End

	Event
		Ename 	PrimeGlass
		Type	Destroy
	End

End

End

		