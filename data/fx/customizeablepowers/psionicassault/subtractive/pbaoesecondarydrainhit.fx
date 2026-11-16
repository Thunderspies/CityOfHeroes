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
		At	Root

		BHVR	CustomizeablePowers\PsionicAssault\ProjectileSlow.bhvr
		Part	CustomizeablePowers\PsionicAssault\EnergyRingProjectile.part
		Part	CustomizeablePowers\PsionicAssault\EnergyRingTrail.part
		Part	CustomizeablePowers\PsionicAssault\EnergyRingTrailGlow.part
		Part	CustomizeablePowers\PsionicAssault\EnergyRingTrailGlowFlat.part
		
		Magnet	T_Root
		#Lookat	T_Root

	End

	Event
		EName	Prime2
		Type	start
		At	head

		BHVR	CustomizeablePowers\PsionicAssault\ProjectileSlow.bhvr
		Part	CustomizeablePowers\PsionicAssault\BlastStreaksLight.part
		Part	CustomizeablePowers\PsionicAssault\ScrambleThoughtsTenticles.part
		Part	CustomizeablePowers\PsionicAssault\ScrambleThoughtsTenticles.part
#		Part	:ScrambleThoughtsTenticles.part
		
		Magnet	T_Head

	End

End
	
Condition
	On	PrimeHit

	
	Event
		EName	coreR
		Type	start
		At	T_Root
		Part	CustomizeablePowers\PsionicAssault\EnergyFlashLight.part
		LifeSpan	7
	End

	Event
		EName	coreR
		Type	start
		At	T_Root
		Part	CustomizeablePowers\PsionicAssault\EnergyUpLight.part
		Part	CustomizeablePowers\PsionicAssault\EnergyUp2Light.part
		LifeSpan	20
	End

	Event
		Ename	Prime
		Type	Destroy
	End
End
	
Condition
	On	Prime2Hit

	Event
		Ename	Prime2
		Type	Destroy
	End
End

End			