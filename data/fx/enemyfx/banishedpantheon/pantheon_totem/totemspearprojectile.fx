#########################################################
##	template

FxInfo

Flags    InheritAlpha

LifeSpan	100
#########################################################
######Hand Starts #####################################
#########################################################
Condition
	On	Time
	Time	12

	Event	
		Type	Local
		At	origin
		Sound totemspear2 110 110 1.0
	End
End

Condition
	On 	Time
	Time 	15

	Event
		At	WepR
		Type	Local

		BHVR	:SpearSpinHand.bhvr
		Geom	GEO_WepR_Totem_Spear
		LifeSpan	48

	End

End


Condition
	On 	Time
	Time 	25

	Event
		At	WepR
		Type	Local

		BHVR	:SpearSpinHandR.bhvr
		Geom	GEO_WepR_Totem_Spear
		LifeSpan	38

	End
	
	Event
		At	WepR
		Type	Local

		BHVR	:SpearSpinHandL.bhvr
		Geom	GEO_WepR_Totem_Spear
		LifeSpan	38

	End
	
	Event
		Ename	PrimeTargetOrigin
		At	WepR
		Type	local
		Bhvr	:DummyOffset.bhvr
		Geom	ConeProjectileTarget
		Lookat	Target

	End
	
	Event
		Ename	PrimeTarget
		At	PrimeTargetOrigin
		Type	START
		altpiv	1
		Bhvr	:DummyOffset1.bhvr
		#Part	:Totem_SpearHitGlow.part

	End
	
End

###########################################################################
##Hand spears
#############################################################

Condition
	On 	Time
	Time 	60

		Event
		Ename	Prime
		At	WepR
		Type	start

		BHVR	:SpearSpin.bhvr
		Geom	GEO_WepR_Totem_Spear

		Magnet	PrimeTarget
		Lookat	PrimeTarget

		LifeSpan	10

	End
	
	Event
		At	Prime
		Type	Local

		Part	:PantheonTotem_SpearGlow.part
		Part	:PantheonTotem_SpearGlow.part
		Part	:PantheonTotem_SpearGlow.part

		LifeSpan	10

	End
	
	Event
		Ename	Prime1
		At	WepR
		Type	start

		BHVR	:SpearSpin1.bhvr
		Geom	GEO_WepR_Totem_Spear

		Magnet	PrimeTarget
		Lookat	PrimeTarget

		LifeSpan	10

	End
	
	Event
		At	Prime1
		Type	Local

		Part	:PantheonTotem_SpearGlow.part
		Part	:PantheonTotem_SpearGlow.part
		Part	:PantheonTotem_SpearGlow.part

		LifeSpan	10

	End

	Event
		Ename	Prime2
		At	WepR
		Type	start

		BHVR	:SpearSpin2.bhvr
		Geom	GEO_WepR_Totem_Spear

		Magnet	PrimeTarget
		Lookat	PrimeTarget

		LifeSpan	10

	End
	
	Event
		At	Prime2
		Type	Local

		Part	:PantheonTotem_SpearGlow.part
		Part	:PantheonTotem_SpearGlow.part
		Part	:PantheonTotem_SpearGlow.part

		LifeSpan	10

	End


	
	Event
		Ename	Prime3
		At	WepR
		Type	start

		BHVR	:SpearSpin3.bhvr
		Geom	GEO_WepR_Totem_Spear


		Magnet	PrimeTarget
		Lookat	PrimeTarget

		LifeSpan	10
	End
	
	Event
		At	Prime3
		Type	Local

		Part	:PantheonTotem_SpearGlow.part
		Part	:PantheonTotem_SpearGlow.part
		Part	:PantheonTotem_SpearGlow.part

		LifeSpan	10

	End


	Event
		Ename	Prime4
		At	WepR
		Type	start

		BHVR	:SpearSpin4.bhvr
		Geom	GEO_WepR_Totem_Spear

		Magnet	PrimeTarget
		Lookat	PrimeTarget

		LifeSpan	10

	End

	Event
		At	Prime4
		Type	Local

		Part	:PantheonTotem_SpearGlow.part
		Part	:PantheonTotem_SpearGlow.part
		Part	:PantheonTotem_SpearGlow.part

		LifeSpan	10

	End


End


Condition
	On	PrimeHit
		
	Event
		EName	Prime
		Type	Destroy
	End
	
	Event
		EName	PPrime4
		Type	Destroy
	End
	
	Event
		EName	PPrime3
		Type	Destroy
	End
End

Condition
	On	Prime4Hit
		
	Event
		EName	Prime4
		Type	Destroy
	End
End

Condition
	On	Prime1Hit
		
	Event
		EName	Prime1
		Type	Destroy
	End
End

Condition
	On	Prime2Hit
		
	Event
		EName	Prime2
		Type	Destroy
	End
End

Condition
	On	Prime3Hit
		
	Event
		EName	Prime3
		Type	Destroy
	End
End

End
