#############################################################
## Child_SingleShot.fx
#############################################################

FxInfo

Lifespan 300

#############################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	BR_Overcharge_Attack_01 100 100 .8
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
		Part	:Muzzle_Rings_Plasma.part
		PMagnet	WepL
		Lifespan 45
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
		Lifespan 120

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
		Lifespan 40
	End
End

#############################################################

Condition
	On 	Time
	Time 	50

	Event
		Ename	Prime
		Type	Start
		At	WeaponOffset
		Bhvr	:Tracer_Fade.bhvr
		BhvrOverride
			TrackRate		3.0
		End
		LookAt	Target
		Magnet	Target
		Lifespan 300
	End

	Event
		Ename	MainBeam
		Type	Local
		At	WeaponOffset
		Part	:PreciseBeam09.part
		Part	:Beam_Large.part
		Part	:Beam_Large_Additive.part
		Part	:BeamCharge_Large.part
		Part	:BeamCharge_Large_Blur.part
		LookAt	Target
		Pother	Target
		Lifespan 20
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
		Type	Local
		At	T_Root
		ChildFX	:BeamRifle_Overcharge_Hit.fx
	End
End

###############################################################

Condition
	On 	Time
	Time 	70

	Event
		Type	PositOnly
		At	P2
		Part	:Muzzle_Smoke01.part
		Lifespan 90
	End
End

###############################################################

End