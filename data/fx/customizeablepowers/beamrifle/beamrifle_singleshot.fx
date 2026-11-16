#############################################################
## BeamRifle_SingleShot.fx
#############################################################

FxInfo
Lifespan 300

#########################################################

Condition
	On 	Time
	Time	10
	Event
		Type	Local
		At	Root
		Sound 	BR_SingleShot_Attack_01 100 100 .8
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
#			PositionOffset	0.25 0.0 -0.1
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
	On 	Time
	Time 	10

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
		Ename	Incendiary2
		Type	Local
		At	Prime
		Bhvr	:Tracer_Fade.bhvr
		Part	:Smoke_Trail01.part
		Part	:Tracer_Trail01.part
		Lifespan 10
	End

	Event
		Ename	MuzzleFlash
		Type	Local
		At	Prime
		Bhvr	:Tracer_Fade.bhvr
		ChildFX	:Child_SingleShot.fx
		Part	:PreciseBeam_Small.part
		Part	:Beam_Small.part
		Part	:Beam_Small_Additive.part
		LookAt	Target
		Pother	Target
		Lifespan 10
	End
End

Condition
	On 	Time
	Time 	10


	Event
		Ename	MuzzleFlash2
		Type	Local
		At	Prime
		Bhvr	:Tracer_Fade.bhvr
		Part	:Projectile_Trail_Electricity.part
		Part	:Projectile_Trail_Electricity_Blur.part
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
