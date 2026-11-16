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

End

Condition
	On	Time
	Time	7

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
 			EndScale	.5 .5 0.2
			Trackrate	8.0
		End
		geom	Tintable_FireBlast1

		POther	Target
		PMagnet	Target
		Magnet	Target
		LookAt	Target
	End
End


Condition
	On	Time
	Time	3

	Event
		Ename	Flash
		Type	Start
		At	OffsetAnchor
		AltPiv	1
		Part	:Muzzle_Flash_Drone.part
		Part	:Muzzle_Flash_Core_Drone.part
		Part	:Muzzle_Shockwave_Big_Drone.part
		Part	:Muzzle_Flash_Shockwave_Drone.part
		Lifespan	5
	End

	Event
		Ename 	Prime
		Type	Start
		At	OffsetAnchor
		AltPiv	1
		BhvrOverride
			TrackRate		7.0
		End
		Part	:Projectile_Ball_Drone.part
		Part	:Projectile_Tendril_Drone.part
		Part	:Projectile_Trail_Ember_Drone.part
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
		Part	:Projectile_Head_Rings2_Drone.part
	End
End

Condition
	On	Time
	Time	10

	Event
		Ename	Flash
		Type	Start
		At	OffsetAnchor
		AltPiv	1
		Part	:Muzzle_Flash_Drone.part
		Part	:Muzzle_Flash_Core_Drone.part
		Part	:Muzzle_Shockwave_Big_Drone.part
		Part	:Muzzle_Flash_Shockwave_Drone.part
		Lifespan	5
	End

	Event
		Ename 	Prime2
		Type	Start
		At	OffsetAnchor
		AltPiv	1
		BhvrOverride
			TrackRate		7.0
		End
		Part	:Projectile_Ball_Drone.part
		Part	:Projectile_Tendril_Drone.part
		Part	:Projectile_Trail_Ember_Drone.part
		Magnet	Target
		LookAt	Target
	End

	Event
		EName	Rings2
		Type	Local
		At	Prime2
		BhvrOverride
			PyrRotate		0 90 90
		End
		Part	:Projectile_Head_Rings2_Drone.part
	End
End

Condition
	On	Time
	Time	17

	Event
		Ename	Flash
		Type	Start
		At	OffsetAnchor
		AltPiv	1
		Part	:Muzzle_Flash_Drone.part
		Part	:Muzzle_Flash_Core_Drone.part
		Part	:Muzzle_Shockwave_Big_Drone.part
		Part	:Muzzle_Flash_Shockwave_Drone.part
		Lifespan	5
	End

	Event
		Ename 	Prime3
		Type	Start
		At	OffsetAnchor
		AltPiv	1
		BhvrOverride
			TrackRate		7.0
		End
		Part	:Projectile_Ball_Drone.part
		Part	:Projectile_Tendril_Drone.part
		Part	:Projectile_Trail_Ember_Drone.part
		Magnet	Target
		LookAt	Target
	End

	Event
		EName	Rings3
		Type	Local
		At	Prime3
		BhvrOverride
			PyrRotate		0 90 90
		End
		Part	:Projectile_Head_Rings2_Drone.part
	End
End

Condition
	On	Time
	Time	24

	Event
		Ename	Flash
		Type	Start
		At	OffsetAnchor
		AltPiv	1
		Part	:Muzzle_Flash_Drone.part
		Part	:Muzzle_Flash_Core_Drone.part
		Part	:Muzzle_Shockwave_Big_Drone.part
		Part	:Muzzle_Flash_Shockwave_Drone.part
		Lifespan	5
	End

	Event
		Ename 	Prime4
		Type	Start
		At	OffsetAnchor
		AltPiv	1
		BhvrOverride
			TrackRate		7.0
		End
		Part	:Projectile_Ball_Drone.part
		Part	:Projectile_Tendril_Drone.part
		Part	:Projectile_Trail_Ember_Drone.part
		Magnet	Target
		LookAt	Target
	End

	Event
		EName	Rings4
		Type	Local
		At	Prime4
		BhvrOverride
			PyrRotate		0 90 90
		End
		Part	:Projectile_Head_Rings2_Drone.part
	End
End

#############################################################

Condition
	On	PrimeHit

	Event
		EName	Prime
		Type	Destroy
	End

	Event
		EName	Rings
		Type	Destroy
	End
End

Condition
	On	Prime2Hit

	Event
		EName	Prime2
		Type	Destroy
	End

	Event
		EName	Rings2
		Type	Destroy
	End
End

Condition
	On	Prime3Hit

	Event
		EName	Prime3
		Type	Destroy
	End

	Event
		EName	Rings3
		Type	Destroy
	End
End

Condition
	On	Prime4Hit
	Event
		EName	Prime4
		Type	Destroy
	End

	Event
		EName	Rings4
		Type	Destroy
	End
End

#############################################################

End