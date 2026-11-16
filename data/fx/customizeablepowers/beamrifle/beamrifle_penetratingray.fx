#############################################################
## BeamRifle_SingleShot.fx
#############################################################

FxInfo
Lifespan 300

#########################################################

Condition
	On 	Time
	Time	7
	Event
		Type	Local
		At	Root
		Sound 	BR_PenetratingRay_Attack_01 100 100 .8
	End
End

Condition
	On 	Time
	Time 	0

	Event
		ENAME   WeaponOffset
		Type	Local
		At	C_Chest
	End


	Event
		Type	Local
		At	WeaponOffset
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PYRRotate		0.0 90.0 0.0
		End
		Lifespan 10
		Part	:Muzzle_Smoke01.part
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	WeaponOffset
		Part	:Muzzle_Glow_LargeCore_Fast.part
		PArt	:BeamCharge_Arcs_Large.part
		Lifespan 10
	End
End

#############################################

Condition
	On 	Time
	Time 	12

	Event
		Ename	Prime
		Type	Start
		At	WeaponOffset
		Bhvr	:Tracer_Fade.bhvr
		BhvrOverride
			TrackRate		6.0
		End
		LookAt	Target
		Magnet	Target
		#Geom	GEO_FX_TestSphere
		Lifespan 300
	End

	Event
		Ename	Incendiary2
		Type	Local
		At	Prime
		Bhvr	:Tracer_Fade.bhvr
		Part	:Smoke_Trail01.part
		Lifespan 10
	End

	Event
		Type	Local
		At	Prime
		Bhvr	:Tracer_Fade.bhvr
		BhvrOverride
			PositionOffset	0 0 2
		End
		Part	:BeamSnipeFlash_01.part
		Lifespan 10
	End

	Event
		Type	Local
		At	WeaponOffset
		Part	:Muzzle_Flash_MD_Shockwave.part
		Lifespan 10
	End

	Event
		Type	Local
		At	Prime
		Bhvr	:Tracer_Fade.bhvr
		Part	:Tracer_Trail07.part
		Lifespan 10
	End

	Event
		Ename	MuzzleFlash
		Type	Local
		At	Prime
		Bhvr	:Tracer_Fade.bhvr
		Part	:PreciseBeam05.part
		Part	:BeamShot_Large.part
		Part	:BeamShot_Large_Additive.part
		LookAt	Target
		Pother	Target
		Lifespan 10
	End
End

## _Medium

Condition
	On 	Time
	Time 	12

	Event
		Ename	Prime2
		Type	Start
		At	WeaponOffset
		Bhvr	behaviors\Braz\Projectile_Fast_01.bhvr
		BhvrOverride
			TrackRate		6.0
		End
		LookAt	Target
		Magnet	Target
		#Geom	GEO_FX_TestSphere
		Lifespan 300
	End

	Event
		Ename	MuzzleFlash2
		Type	Local
		At	Prime2
		Bhvr	:Tracer_Fade.bhvr
		Part	:Beam_Large_Electricity.part
		Part	:Beam_Large_Electricity_Blur.part
		LookAt	Target
		Pother	Target
		Lifespan 10
	End
End


#############################################################

Condition
	On 	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
	End

	Event
		Ename	Prime2
		Type	Destroy
	End

	Event
		Ename	Incendiary1
		Type	Destroy
	End

	Event
		Ename	Incendiary2
		Type	Destroy
	End
End

#############################################################

End
