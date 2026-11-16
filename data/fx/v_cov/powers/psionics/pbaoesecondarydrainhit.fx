#############################################################
## PBAOESecondaryDrainHit.fx
#############################################################

FxInfo
LifeSpan	100

###########################################################

Input
	Inpname	Hips
End

###########################################################

Condition
	On	Time
	Time	0

	Event
		EName	Prime
		Type	start
		At	Root

		BHVR	:ProjectileSlow.bhvr
		Part	:EnergyRingProjectile.part
		Part	:EnergyRingTrail.part
		Part	:EnergyRingTrailGlow.part
		Part	:EnergyRingTrailGlowFlat.part

		Magnet	T_Root
		#Lookat	T_Root

	End

	Event
		EName	Prime2
		Type	start
		At	head

		BHVR	:ProjectileSlow.bhvr
		Part	:BlastStreaksLight.part
		Part	:ScrambleThoughtsTenticles.part
		Part	:ScrambleThoughtsTenticles.part
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
		Part	:EnergyFlashLight.part
		LifeSpan	7
	End

	Event
		EName	coreR
		Type	start
		At	T_Root
		Part	:EnergyUpLight.part
		Part	:EnergyUp2Light.part
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