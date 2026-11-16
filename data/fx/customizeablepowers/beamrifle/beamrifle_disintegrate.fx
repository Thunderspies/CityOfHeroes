#############################################################
## Child_SingleShot.fx
#############################################################

FxInfo

Lifespan 100

#############################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	BR_Disintegrate_Attack_01 100 100 .8
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
		Ename	ChargeUp
		Type	Local
		At	WeaponOffset
		AltPiv	1
		Bhvr	:GrowInPlace.bhvr
		BhvrOverride
			Spin			90 0 0
		End
	End

	Event
		Type	Local
		At	ChargeUp
		Part	:Muzzle_Glow_Core_Fast.part
		PArt	:BeamCharge_Arcs_Medium.part
		Lifespan 15
	End
End

Condition
	On	Time
	Time	22

	Event
		Ename	BeamEnd
		Type	Local
		At	Target
	End

	Event
		Type	Local
		At	WeaponOffset
	#	AltPiv	1
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:MuzzleFlash_Continuous.part
		Part	:Muzzle_Glow_Core_Continuous.part
		Part	:Muzzle_Flash_Glow.part
		Lifespan 3
	End
End



####################

Condition
	On	Time
	Time	25

	Event
		Type	Local
		At	WeaponOffset
	#	AltPiv	1
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Muzzle_Glow_Core_Fast.part
		PArt	:BeamCharge_Arcs_Medium.part
		Lifespan 20
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
		PMagnet	BeamEnd
		Lifespan 3
	End

	Event
		Type	Local
		At	WeaponOffset
	#	AltPiv	1
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:Muzzle_Glow_LargeCore_Fast.part
		PArt	:BeamCharge_Arcs_Large.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	28

	Event
		Type	Local
		At	WeaponOffset
	#	AltPiv	1
		Part	:Beam_Small_Continuous.part
		Part	:Beam_Small_Continuous_Add.part
		Part	:Beam_Flare03.part
		PMagnet	BeamEnd
		LookAt	BeamEnd
		Pother	BeamEnd
		Lifespan 20
	End

	Event
		Type	Local
		At	WeaponOffset
	#	AltPiv	1
		Part	:Beam_Small_Continuous_Electricity.part
		Part	:Beam_Small_Continuous_Electricity_Blur.part
		LookAt	BeamEnd
		Pother	BeamEnd
		PMagnet	BeamEnd
		Lifespan 20
	End

	Event
		Type	Local
		At	BeamEnd
		Part	:Beam_Flare1.Part
		Lifespan 20
	End
End

#############################################################

End