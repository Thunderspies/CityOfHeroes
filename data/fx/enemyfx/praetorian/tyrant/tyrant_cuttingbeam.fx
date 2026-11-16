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
		BhvrOverride
			PositionOffset 0 0.25 0.5
		End
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
		Ename	GroundAnchor
		#Type	Local
		At	Root
		Type CREATE
  		Inherit	Rotation Position
  		Update	Rotation Position
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
		Ename	LookAtTarget1
		Type	Local
		At	GroundAnchor
		LookAt	HeadOffset
		BhvrOverride
			InitialVelocity 1 0 6
		End
	End

	Event
		Ename	LookAtTarget2
		Type	Local
		At	GroundAnchor
		LookAt	HeadOffset
		BhvrOverride
			InitialVelocity 0.3 0 6
		End
	End

	Event
		Ename	LookAtTarget3
		Type	Local
		At	GroundAnchor
		LookAt	HeadOffset
		BhvrOverride
			InitialVelocity -0.3 0 6
		End
	End

	Event
		Ename	LookAtTarget4
		Type	Local
		At	GroundAnchor
		LookAt	HeadOffset
		BhvrOverride
			InitialVelocity -1 0 6
		End
	End

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
		#Part	CustomizeablePowers\BeamRifle\Muzzle_Glow_Core_Fast.part
		Lifespan 15
	End


	Event
		Type	Local
		At	WeaponOffset
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PyrRotate		0 90 0
		End
		#Part	CustomizeablePowers\BeamRifle\MuzzleFlash_Gas03.part
		PMagnet	BeamEnd
		Lifespan 20
	End
End

Condition
	On	Time
	Time	25

########## BEAM 1

	Event
		Ename	SweepTargetL
		Type	Local
		At	LookAtTarget1
		BhvrOverride
			#PositionOffset	-12 0 0
			#InitialVelocity	1.5 0.0 0.0
		End
		Lifespan	18
	End

	Event
		Ename	SweepTargetL2
		Type	Local
		At	LookAtTarget1
		BhvrOverride
			#PositionOffset	-12 0 0
			#InitialVelocity	1.5 0.0 0.0
		End
		Lifespan	16
	End

	Event
		Ename	LaserSweepFlareL
		Type	Local
		At	SweepTargetL
		BhvrOverride
			#PositionOffset	-1.8 0 2
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

######## BEAM 2

	Event
		Ename	SweepTargetLa
		Type	Local
		At	LookAtTarget2
		BhvrOverride
			#PositionOffset	-12 0 0
			#InitialVelocity	1.5 0.0 0.0
		End
		Lifespan	18
	End

	Event
		Ename	SweepTargetL2a
		Type	Local
		At	LookAtTarget2
		BhvrOverride
			#PositionOffset	-12 0 0
			#InitialVelocity	1.5 0.0 0.0
		End
		Lifespan	16
	End

	Event
		Ename	LaserSweepFlareLa
		Type	Local
		At	SweepTargetLa
		BhvrOverride
			#PositionOffset	-1.8 0 2
		End
		Part	CustomizeablePowers\BeamRifle\Beam_HitTrail.part
		Part	CustomizeablePowers\BeamRifle\Beam_Flare1.Part
		Lifespan	30
	End

	Event
		Ename	LaserSweepLa
		Type	Local
		At	SweepTargetLa
		Part	CustomizeablePowers\BeamRifle\Tracer_Trail05.part
		Part	CustomizeablePowers\BeamRifle\Beam_Small_Continuous.part
		Part	CustomizeablePowers\BeamRifle\Beam_Small_Continuous_Add.part
		Part	CustomizeablePowers\BeamRifle\Beam_Small_Continuous_Electricity.part
		Part	CustomizeablePowers\BeamRifle\Beam_Small_Continuous_Electricity_Blur.part
		Part	CustomizeablePowers\BeamRifle\Beam_Flare03.part
		pOther	HeadOffset
		Lifespan	18
	End

######## BEAM 3

	Event
		Ename	SweepTargetLb
		Type	Local
		At	LookAtTarget3
		BhvrOverride
			#PositionOffset	-12 0 0
			#InitialVelocity	1.5 0.0 0.0
		End
		Lifespan	18
	End

	Event
		Ename	SweepTargetL2b
		Type	Local
		At	LookAtTarget3
		BhvrOverride
			#PositionOffset	-12 0 0
			#InitialVelocity	1.5 0.0 0.0
		End
		Lifespan	16
	End

	Event
		Ename	LaserSweepFlareLb
		Type	Local
		At	SweepTargetLb
		BhvrOverride
			#PositionOffset	-1.8 0 2
		End
		Part	CustomizeablePowers\BeamRifle\Beam_HitTrail.part
		Part	CustomizeablePowers\BeamRifle\Beam_Flare1.Part
		Lifespan	30
	End

	Event
		Ename	LaserSweepLb
		Type	Local
		At	SweepTargetLb
		Part	CustomizeablePowers\BeamRifle\Tracer_Trail05.part
		Part	CustomizeablePowers\BeamRifle\Beam_Small_Continuous.part
		Part	CustomizeablePowers\BeamRifle\Beam_Small_Continuous_Add.part
		Part	CustomizeablePowers\BeamRifle\Beam_Small_Continuous_Electricity.part
		Part	CustomizeablePowers\BeamRifle\Beam_Small_Continuous_Electricity_Blur.part
		Part	CustomizeablePowers\BeamRifle\Beam_Flare03.part
		pOther	HeadOffset
		Lifespan	18
	End

######## BEAM 4

	Event
		Ename	SweepTargetLc
		Type	Local
		At	LookAtTarget4
		BhvrOverride
			#PositionOffset	-12 0 0
			#InitialVelocity	1.5 0.0 0.0
		End
		Lifespan	18
	End

	Event
		Ename	SweepTargetL2c
		Type	Local
		At	LookAtTarget4
		BhvrOverride
			#PositionOffset	-12 0 0
			#InitialVelocity	1.5 0.0 0.0
		End
		Lifespan	16
	End

	Event
		Ename	LaserSweepFlareLc
		Type	Local
		At	SweepTargetLc
		BhvrOverride
			#PositionOffset	-1.8 0 2
		End
		Part	CustomizeablePowers\BeamRifle\Beam_HitTrail.part
		Part	CustomizeablePowers\BeamRifle\Beam_Flare1.Part
		Lifespan	30
	End

	Event
		Ename	LaserSweepLc
		Type	Local
		At	SweepTargetLc
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

#	Event
#		Type	Start
#		At	SweepTargetL2
#		Part	CustomizeablePowers\BeamRifle\Beam_Small_AfterGlow.part
#		Pother	HeadOffset
#		Lifespan 10
#	End
End

#########################################################

End