#############################################################
## DronePlasmaShot_Attack.fx
#############################################################

FxInfo
LifeSpan 300

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	OffsetAnchor
		Type	Local
		Geom	FX_WWMobileDefenseTurret_Offsets
	End

	Event
		Ename	Flash
		Type	Start
		At	OffsetAnchor
		AltPiv	1
		Part	:Muzzle_Flash_Drone.part
		Part	:Muzzle_Flash_Core_Drone.part
		Part	:Muzzle_Shockwave_Big_Drone.part
		Part	:Muzzle_Flash_Shockwave_Drone.part
	End
End


Condition
	On	Time
	Time	3
	Event
		Ename 	Prime
		Type	Start
		At	OffsetAnchor
		AltPiv	1
		BhvrOverride
			TrackRate		6.0
		End
		Part	:Projectile_Head_Drone_Big.part
		Part	:Projectile_Trail_Glow_Small_Drone.part
		Part	:Projectile_Trail_Tendril_Drone.part

		Magnet	Target
		LookAt	Target
	End

	Event
		EName	Rings
		Type	Local
		At	Prime
		BhvrOverride
			PyrRotate		0 90 90
		End
		Part	:Projectile_Head_Rings_Drone_Small.part
	End
End

Condition
	On	Time
	Time	10



	Event
		EName 	BeamGeo
		Type	Start
		At	OffsetAnchor
		AltPiv	1
		Bhvr	behaviors/InfernoBoltBeamSmall.bhvr
		BhvrOverride
			StartColor	0 0 255
			FadeInLength	5
			FadeOutLength	10

			Scale		0.1 0.1 0.007
			ScaleRate	.05 .05 .05
 			EndScale	.3 .3 0.2
		End
		Part	:Beam_Glow_Drone.part
		geom	Tintable_FireBlast1
		POther	Target
		PMagnet	Target
		Magnet	Target
		LookAt	Target

		LifeSpan		10
	End
End

#############################################################

Condition
	On	PrimeHit

	Event
		EName	Prime
		Type	Destroy
	End

End

#############################################################

End