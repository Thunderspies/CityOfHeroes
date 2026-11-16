#############################################################
## Child_SingleShot.fx
#############################################################

FxInfo

Lifespan 100

#############################################################

Condition
	On 	Time
	Time	15
	Event
		Type	Local
		At	Root
		Sound 	BR_LancerShot_Attack_01 100 100 .8
	End
End

Condition
	On	Time
	Time	15

	Event
		ENAME   WeaponOffset
		Type	Local
		At	C_Chest
	End

#	Event
#		ENAME   WeaponOffset
#		Type	Local
#		At	WepR
#		Geom	FlameThrower
#		Bhvr	:Tracer_Fade.bhvr
#		BhvrOverride
#			Alpha		1
#			PositionOffset	0.25 0.0 -0.3
#		End
#	End

	Event
		Type	Local
		At	WeaponOffset
	#	AltPiv	1
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PYRRotate		0.0 90.0 0.0
		End
		Part	:MuzzleFlash_Continuous.part
		Part	:Muzzle_Glow_Core_Continuous.part
		Part	:Muzzle_Flash_Glow.part
		LookAt	Target
		Lifespan 3
	End
End



####################

Condition
	On	Time
	Time	15

	Event
		Type	Local
		At	WeaponOffset
	#	AltPiv	1
		Bhvr	behaviors/GenericParticleFade.bhvr
		PArt	:BeamCharge_Arcs_Medium.part
		Lifespan 5
	End

	Event
		Type	Local
		At	WeaponOffset
	#	AltPiv	1
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PYRRotate		0.0 90.0 0.0
		End
		Part	:MuzzleFlash_Gas03.part
		PMagnet	Target
		Lifespan 5
	End
End

Condition
	On	Time
	Time	15

	Event
		Type	Local
		At	WeaponOffset
	#	AltPiv	1
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:Tracer_Trail06.part
		Part	:PreciseBeam03.part
		Part	:Beam_Long_Continuous.part
		Part	:Beam_Long_Continuous_Add.part
		Part	:Beam_Flare03.part
		PMagnet	Target
		LookAt	Target
		Pother	Target
		Lifespan 20
	End

	Event
		Type	Local
		At	WeaponOffset
	#	AltPiv	1
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:Beam_Long_Continuous_Electricity.part
		Part	:Beam_Long_Continuous_Electricity_Blur.part
		LookAt	Target
		Pother	Target
		PMagnet	Target
		Lifespan 20
	End

	Event
		Type	Local
		At	WeaponOffset
	#	AltPiv	1
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:Muzzle_Glow_LargeCore_Fast.part
		PArt	:BeamCharge_Arcs_Large.part
		Lifespan 15
	End
End


#############################################################

End