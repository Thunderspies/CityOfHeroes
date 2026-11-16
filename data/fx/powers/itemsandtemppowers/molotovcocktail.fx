#########################################################
##	jack
##

FxInfo



Flags    InheritAlpha


LifeSpan		200



Condition
	On	Time
	Time	10

	Event
		Type	local
		At	Origin
		Sound molotovtoss 90 90 .77
	End
End


Condition
	On 	Time
	Time 	10

	Event
		EName 	RockinHand
		Type	local
		At	WepR
		Geom	GEO_WepR_MolotovCocktaill
		BHVR	:MolotovCocktail.bhvr
		LifeSpan	25
	End
	
	Event
		Type	local
		At	RockinHand
		Altpiv	1
		
		Part	:fireHand.part
		Part	:fireHand.part
		Part	:fireHand.part
		Part	:HandSparks.part
		Part	:HandSmoke.part

		LifeSpan	25
	End
End

Condition
	On 	Time
	Time 	35

	Event
		EName 	Prime
		Type	start
		At	WepR
		Geom	GEO_WepR_MolotovCocktaill
		Bhvr	:MolotovCocktailProjectile.bhvr
		Magnet	T_Chest
				
	End
	
	Event
		EName 	P
		Type	local
		At	Prime
		Altpiv	1
		
		Part	:fire.part
		Part	:fire.part
		Part	:fire.part
		Part	:fire.part
		Part	:Sparks.part
		Part	:Smoke.part
		
	End

End

Condition
	On 	PrimeHit
	
	Event
		EName 	Prime
		Type	Destroy
		
	End
	
	Event
		EName 	P
		Type	Destroy
		
	End

End

End		