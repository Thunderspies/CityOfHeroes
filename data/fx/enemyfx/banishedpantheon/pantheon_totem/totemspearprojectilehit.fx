#########################################################
##	template

FxInfo

LifeSpan	90

Condition
	On 	Time
	Time 	0

	Event
		At	Chest
		Type	local
		BHVR	Behaviors/GenericParticleFade.bhvr
		Part	:Totem_SpearHitGlow.part
		Sound totemspearhit 70 70 .95

	End

	Event
		At	Chest
		Type	local

		BHVR	:SpearHit.bhvr
		Geom	GEO_WepR_Totem_Spear
		LifeSpan	50
	End

End


Condition
	On 	Time
	Time 	2

	Event
		At	Chest
		Type	local

		BHVR	:SpearHit2.bhvr
		Geom	GEO_WepR_Totem_Spear
		LifeSpan	50

	End

End

Condition
	On 	Time
	Time 	0

		Event
		Ename	Prime
		At	T_wepR
		Type	start

		BHVR	:SpearSpina.bhvr
		Geom	GEO_WepR_Totem_Spear

		Magnet	Chest
		Lookat	Chest


	End
	
	Event
		Ename	a
		At	Prime
		Type	Local

		Part	:PantheonTotem_SpearGlow.part
		Part	:PantheonTotem_SpearGlow.part
		Part	:PantheonTotem_SpearGlow.part


	End
	
	Event
		Ename	Prime1
		At	T_wepR
		Type	start

		BHVR	:SpearSpin1a.bhvr
		Geom	GEO_WepR_Totem_Spear

		Magnet	Chest
		Lookat	Chest


	End
	
	Event
		Ename	1
		At	Prime1
		Type	Local

		Part	:PantheonTotem_SpearGlow.part
		Part	:PantheonTotem_SpearGlow.part
		Part	:PantheonTotem_SpearGlow.part


	End

	Event
		Ename	Prime2
		At	T_wepR
		Type	start

		BHVR	:SpearSpin2a.bhvr
		Geom	GEO_WepR_Totem_Spear

		Magnet	Chest
		Lookat	Chest


	End
	
	Event
		Ename	2
		At	Prime2
		Type	Local

		Part	:PantheonTotem_SpearGlow.part
		Part	:PantheonTotem_SpearGlow.part
		Part	:PantheonTotem_SpearGlow.part


	End


	
	Event
		Ename	Prime3
		At	T_wepR
		Type	start

		BHVR	:SpearSpin3a.bhvr
		Geom	GEO_WepR_Totem_Spear


		Magnet	Chest
		Lookat	Chest

	End
	
	Event
		Ename	3
		At	Prime3
		Type	Local

		Part	:PantheonTotem_SpearGlow.part
		Part	:PantheonTotem_SpearGlow.part
		Part	:PantheonTotem_SpearGlow.part


	End


	Event
		Ename	Prime4
		At	T_wepR
		Type	start

		BHVR	:SpearSpin4a.bhvr
		Geom	GEO_WepR_Totem_Spear

		Magnet	Chest
		Lookat	Chest


	End

	Event
		Ename	4
		At	Prime4
		Type	Local

		Part	:PantheonTotem_SpearGlow.part
		Part	:PantheonTotem_SpearGlow.part
		Part	:PantheonTotem_SpearGlow.part


	End


End
#################################################################################################

Condition
	On	PrimeHit
		
	Event
		EName	a
		Type	Destroy
	End
	
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
		EName	4
		Type	Destroy
	End
	
	Event
		EName	Prime4
		Type	Destroy
	End
End

Condition
	On	Prime1Hit
	
		
	Event
		EName	1
		Type	Destroy
	End
		
	Event
		EName	Prime1
		Type	Destroy
	End
End

Condition
	On	Prime2Hit
		
	Event
		EName	2
		Type	Destroy
	End
	
	Event
		EName	Prime2
		Type	Destroy
	End
End

Condition
	On	Prime3Hit
		
	Event
		EName	3
		Type	Destroy
	End
	
	Event
		EName	Prime3
		Type	Destroy
	End
End

End
