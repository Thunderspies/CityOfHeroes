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
		Sound 	BR_PiercingBeam_Attack_01 100 100 .8
	End
End

Condition
	On	Time
	Time	50

	Event
		ENAME   BeamAnchor
		Type	Local
		At	C_Chest
	End

	Event
		Type	Local
		At	BeamAnchor
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			#PositionOffset		-0.25 0.3 2
		End
		Part	:MuzzleFlash_Continuous.part
		Part	:Muzzle_Glow_Core_Continuous.part
		Part	:Muzzle_Flash_Glow.part
		Lifespan 3
	End

End

###################################### CHARGE UP

Condition
	On 	Time
	Time 	0

	Event
		ENAME   WeaponOffset
		Type	Local
		At	C_Chest
	End
End

Condition
	On 	Time
	Time 	5


	Event
		Type	Local
		At	WeaponOffset
		Part	:Muzzle_Glow.part
		Part	:Muzzle_Glow_Core.part
		Lifespan 30

	End
End

Condition
	On	Time
	Time	15

	Event
		Type	Local
		At	WeaponOffset
		Bhvr	behaviors/GenericParticleFade.bhvr
		PArt	:BeamCharge_Arcs_Subtle.part
		PMagnet	WepL
		Lifespan 15

	End
End

Condition
	On	Time
	Time	30

	Event
		Type	Local
		At	WeaponOffset
		Bhvr	behaviors/GenericParticleFade.bhvr
		PArt	:BeamCharge_Arcs.part
		PMagnet	WepL
		Lifespan 15

	End
End
Condition
	On 	Time
	Time 	45

	Event
		EName 	Flash
		Type	Local
		At	WeaponOffset
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Muzzle_Flash.part
		Part	:Muzzle_Flash_Flash.part
		PArt	:BeamCharge_Arcs.part
		Part	:Muzzle_Flash_Shockwave.part
		Lifespan 5
	End
End

######################################	BEAM

Condition
	On	Time
	Time	50

	Event
		Type	Local
		At	WeaponOffset
		Bhvr	behaviors/GenericParticleFade.bhvr
		PArt	:BeamCharge_Arcs_Medium.part
		Lifespan 5
	End

	Event
		Type	Local
		At	WeaponOffset
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:MuzzleFlash_Gas03.part
		PMagnet	Target
		Lifespan 5
	End
End

Condition
	On	Time
	Time	50

	Event
		Type	Local
		At	BeamAnchor
		Part	:Tracer_Trail08.part
		Part	:Beam_Wide_Continuous.part
		Part	:Beam_Wide_Continuous_Add.part
		Part	:Beam_Flare03.part
		Part	:Beam_Wide_Continuous_Electricity.part
		Part	:Beam_Wide_Continuous_Electricity_Blur.part
		LookAt	Target
		Pother	Target
		PMagnet	Target
		Lifespan 12
	End
End

Condition
	On	Time
	Time	55

	Event
		Type	PositOnly
		At	Target
		Part	:Beam_Long_ContinuousOffset_Electricity.part
		LookAt	WeaponOffset #BeamAnchor
		Pother	WeaponOffset #BeamAnchor
		PMagnet	WeaponOffset #BeamAnchor
	End
End

Condition
	On	Time
	Time	60
	Event
		Type	Local
		At	BeamAnchor
		Part	:Beam_Long_Continuous.part
		Part	:Beam_Long_Continuous_Add.part
		Part	:Beam_Long_Continuous_Electricity.part
		Part	:Beam_Long_Continuous_Electricity_Blur.part
		LookAt	Target
		Pother	Target
		PMagnet	Target
		Lifespan 10
	End

	Event
		Type	PositOnly
		At	Target
		Part	:Beam_Long_ContinuousOffset_Electricity.part
		LookAt	WeaponOffset #BeamAnchor
		Pother	WeaponOffset #BeamAnchor
		PMagnet	WeaponOffset #BeamAnchor
	End
End



Condition
	On	Time
	Time	65

	Event
		Type	PositOnly
		At	Target
		Part	:Beam_Long_ContinuousOffset_Electricity.part
		LookAt	WeaponOffset #BeamAnchor
		Pother	WeaponOffset #BeamAnchor
		PMagnet	WeaponOffset #BeamAnchor
	End
End


#############################################################

End