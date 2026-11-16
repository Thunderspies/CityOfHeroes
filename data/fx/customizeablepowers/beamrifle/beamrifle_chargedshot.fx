#############################################################
## BeamRifle_SingleShot.fx
#############################################################

FxInfo
Lifespan 300

#########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	BR_ChargedShot_Attack_01 100 100 .8
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

#	Event
#		ENAME   WeaponOffset
#		Type	Local
#		At	WepR
#		Geom	FlameThrower
#		Bhvr	:Tracer_Fade.bhvr
#		BhvrOverride
#			Alpha		1
#			PositionOffset	0.25 0 -0.1
#		End
#	End

	Event
		ENAME   P2
		Type	Local
		At	WepL
		BhvrOverride
			Alpha		1
		End
		Geom	FlameThrower

	End

	Event
		Type	PositOnly
		At	P2
		Part	:Muzzle_Smoke01.part
		Lifespan 25
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Ename	ChargeUp
		Type	Local
		At	WeaponOffset
	#	AltPiv	1
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

#############################################

Condition
	On 	Time
	Time 	23

	Event
		Ename	Prime
		Type	Start
		At	WeaponOffset
	#	AltPiv	1
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
		Ename	Incendiary
		Type	Local
		At	Prime
		Bhvr	:Tracer_Fade.bhvr
		Part	:Smoke_Trail01.part
		Part	:Tracer_Trail02.part
		Lifespan 10
	End

	Event
		Ename	Beams
		Type	Local
		At	Prime
		Bhvr	:Tracer_Fade.bhvr
		ChildFX	:Child_SingleShot.fx
		Part	:PreciseBeam04.part
		Part	:Beam_Medium.part
		Part	:Beam_Medium_Additive.part
		LookAt	Target
		Pother	Target
		Lifespan 10
	End

	Event
		Ename	ChargeBeams
		Type	Local
		At	Prime
		Bhvr	:Tracer_Fade.bhvr
		Part	:Beam_Medium_Electricity.part
		Part	:Beam_Medium_Electricity_Blur.part
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
End

#############################################################

End
