#########################################################
##	onfire
##
FxInfo  

LifeSpan	140


Condition
	Time 	0

	Event
		EName	VaultHookup
		Type	start
		At	origin	
		Part	:FireballBurnHitGlowlower.part
		
		LifeSpan	8

	End

	Event
		EName	VaultHookup
		Type	start
		At	origin

		Part	:FireballBurnHitGlow.part
		Part	:FireballBurnHitGlow2.part
		Part	:FireballBurnHitEmbers.part
		LifeSpan	2
	End

	Event
		EName	VaultHookup
		Type	start
		At	origin
		Part	:FireballBurnHitScorch.part
		Part	:FireballBurnHitGlowStar.part
		Part	:FireballBurnHitGlowStarCore.part
		
		LifeSpan	10

	End	

	Event
		EName	VaultHookup
		Type	start
		At	origin
		Part	:FireballBurnHitDecal.part
		
		LifeSpan	1

	End

	Event
		EName	VaultHookup
		Type	start
		At	origin	
		Part	:Steam.part
		
		LifeSpan	23

	End


End


End			
