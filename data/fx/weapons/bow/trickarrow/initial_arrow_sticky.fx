#########################################################
##	template

FxInfo

Flags InheritAnimScale InheritAlpha

LifeSpan 1000

#### arrow in hand  ########################

Condition
	On	Time
	Time	32

	Event	
		Type	Local
		At	WepR
		Sound arrowout4b 50 50 .3
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
		LifeSpan 24
	End
	
	Event
		Type	Local
		At	arrowglass
		BHVR	:ArrowGlassOffset.bhvr
		Geom	LiquidArrowGlassPoison
		
		LifeSpan 24
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
		
		Magnet	T_root
		LookAt	T_Root

	End

	Event
		Type	Local
		At	Prime
		BHVR	:ArrowGlassOffset.bhvr
		Geom	LiquidArrowGlassPoison
		
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
		EName 	Prime1
		Type	start 
		At	RadiusFxScale
		bhvr    :stickyBubble.bhvr
		Part	:StickyYellow2.part
		Part	:StickyGlows.part
		LifeSpan	10
		
	End		
	

	Event 	
		EName	Prime
		Type	Destroy
	End

End

End

		