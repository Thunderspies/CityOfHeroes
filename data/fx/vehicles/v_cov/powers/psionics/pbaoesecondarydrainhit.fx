#########################################################
##	cot_waterfall_med.fx
##
FxInfo
	
LifeSpan	100

###########################################################
Condition
	On	Time
	Time	0

	Event
		EName	Prime
		Type	start
		At	T_Root

		BHVR	:Projectile.bhvr
		Part	:EnergyRingProjectile.part
		Part	:EnergyRingTrail.part
		Part	:EnergyRingTrailGlow.part
		Part	:EnergyRingTrailGlowFlat.part
		
		Magnet	Root
		#Lookat	T_Root

	End


End
	
Condition
	On	PrimeHit

	
	Event
		EName	coreR
		Type	start
		At	Root
		Part	:EnergyFlash.part
		LifeSpan	7
	End

	Event
		EName	coreR
		Type	start
		At	Root
		Part	:EnergyUp.part
		Part	:EnergyUp2.part
		LifeSpan	20
	End

	Event
		Ename	Prime
		Type	Destroy
	End
End


End			