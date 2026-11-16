#########################################################
## Rikti Heavy Assault Suit - Sweeping Laser Attack
#########################################################

FxInfo

LifeSpan	200

## SOUND FX #######################################################

Condition
	On 	Time
	Time	10
	Event
		Type	Local
		At	Root
		Sound 	BR_CuttingBeam_Attack_01 100 100 .8
	End
End

Condition
	On	Time
	Time	0

	Event
		ENAME   WeaponOffset
		Type	Local
		At	Head
	End

	Event
		Ename	HeadOffset
		Type	Local
		At	WeaponOffset
		Bhvr	CustomizeablePowers\BeamRifle\Tracer_Fade.bhvr
	End
End

# LASER SWEEP 1 #######################################################

Condition
	On	Time
	Time	0

	Event
		Ename	LookAtTarget
		Type	Start
		At	T_Chest
		LookAt	HeadOffset
	End

	Event
		Ename	LaserFlash
		Type	Local
		At	HeadOffset
		Lifespan	30
	End

	Event
		Ename	AfterGlowAnchor
		Type	Local
		At	HeadOffset
		Lifespan 1
	End
End

Condition
	On	Time
	Time	23

	Event
		Type	Local
		At	WeaponOffset
		Bhvr	behaviors/GenericParticleFade.bhvr
		PArt	CustomizeablePowers\BeamRifle\BeamCharge_Arcs_Medium.part
		Lifespan 20
	End

	Event
		Type	Local
		At	WeaponOffset
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	CustomizeablePowers\BeamRifle\Muzzle_Glow_Core_Fast.part
		Lifespan 15
	End


	Event
		Type	Local
		At	WeaponOffset
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PyrRotate		0 90 0
		End
		Part	CustomizeablePowers\BeamRifle\MuzzleFlash_Gas03.part
		PMagnet	BeamEnd
		Lifespan 20
	End
End

Condition
	On	Time
	Time	25

	Event
		Ename	SweepTargetL
		Type	Local
		At	LookAtTarget
		BhvrOverride
			PositionOffset	-12 0 0
			InitialVelocity	1.5 0.0 0.0
		End
		Lifespan	18
	End

	Event
		Ename	SweepTargetL2
		Type	Local
		At	LookAtTarget
		BhvrOverride
			PositionOffset	-12 0 0
			InitialVelocity	1.5 0.0 0.0
		End
		Lifespan	16
	End

	Event
		Ename	LaserSweepFlareL
		Type	Local
		At	SweepTargetL
		BhvrOverride
			PositionOffset	-1.8 0 2
		End
		Part	CustomizeablePowers\BeamRifle\Beam_HitTrail.part
		Part	CustomizeablePowers\BeamRifle\Beam_Flare1.Part
		Lifespan	30
	End

	Event
		Ename	LaserSweepL
		Type	Local
		At	SweepTargetL
		Part	CustomizeablePowers\BeamRifle\Tracer_Trail05.part
		Part	CustomizeablePowers\BeamRifle\Beam_Small_Continuous.part
		Part	CustomizeablePowers\BeamRifle\Beam_Small_Continuous_Add.part
		Part	CustomizeablePowers\BeamRifle\Beam_Small_Continuous_Electricity.part
		Part	CustomizeablePowers\BeamRifle\Beam_Small_Continuous_Electricity_Blur.part
		Part	CustomizeablePowers\BeamRifle\Beam_Flare03.part
		pOther	HeadOffset
		Lifespan	18
	End
End

Condition
	On	Cycle
	Time	1

	Event
		Type	Start
		At	SweepTargetL2
		Part	CustomizeablePowers\BeamRifle\Beam_Small_AfterGlow.part
		Pother	HeadOffset
		Lifespan 10
	End
End

#########################################################

End