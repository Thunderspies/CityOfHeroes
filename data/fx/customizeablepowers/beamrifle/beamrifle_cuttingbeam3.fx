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
		At	C_Chest
	End

	Event
		Ename	HeadOffset
		Type	Local
		At	WeaponOffset
		Bhvr	:Tracer_Fade.bhvr
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
		PArt	:BeamCharge_Arcs_Medium.part
		Lifespan 20
	End

	Event
		Type	Local
		At	WeaponOffset
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Muzzle_Glow_Core_Fast.part
		Lifespan 15
	End


	Event
		Type	Local
		At	WeaponOffset
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PyrRotate		0 90 0
		End
		Part	:MuzzleFlash_Gas03.part
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
			PositionOffset	12 0 0
			InitialVelocity	-1.5 0.0 0.0
		End
		Lifespan	18
	End

	Event
		Ename	SweepTargetL2
		Type	Local
		At	LookAtTarget
		BhvrOverride
			PositionOffset	12 0 0
			InitialVelocity	-1.5 0.0 0.0
		End
		Lifespan	16
	End

	Event
		Ename	LaserSweepFlareL
		Type	Local
		At	SweepTargetL
		BhvrOverride
			PositionOffset	1.8 0 2
		End
		Part	:Beam_HitTrail.part
		Part	:Beam_Flare1.Part
		Lifespan	30
	End

	Event
		Ename	LaserSweepL
		Type	Local
		At	SweepTargetL
		Part	:Tracer_Trail05.part
		Part	:Beam_Small_Continuous.part
		Part	:Beam_Small_Continuous_Add.part
		Part	:Beam_Small_Continuous_Electricity.part
		Part	:Beam_Small_Continuous_Electricity_Blur.part
		Part	:Beam_Flare03.part
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
		Part	:Beam_Small_AfterGlow.part
		Pother	HeadOffset
		Lifespan 10
	End
End

#########################################################

End