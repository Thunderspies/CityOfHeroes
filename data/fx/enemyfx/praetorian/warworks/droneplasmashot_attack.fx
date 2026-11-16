#############################################################
## DronePlasmaBeam_Attack.fx
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
		Ename	ConeOffset
		Type	Start
		At	Root
		BhvrOverride
			Scale		60 60 60
		End
		GEOM	FX_ConeOffsets
	End

	Event
		Ename	TargetOffset
		Type	Start
		At	ConeOffset
		AltPiv	1
	End

	Event
		EName	OffsetAnchor
		Type	Local
		Geom	FX_WWDefenseTurret_Offsets
	End

	Event
		Ename	Flash
		Type	Local
		At	OffsetAnchor
		AltPiv	1
		Part	:Muzzle_Flash_Drone.part
		Part	:Muzzle_Flash_Core_Drone.part
		Part	:Muzzle_Flash_Shockwave_Drone.part
	End
End

Condition
	On	Time
	Time	2

	Event
		Ename 	Prime
		Type	Start
		At	OffsetAnchor
		AltPiv	1
		BhvrOverride
			PyrRotate		0 0 90
			TrackRate		6.0
		End
		Part	:Projectile_Trail_Ember_Drone_Bright.part
		Part	:Projectile_Trail_Smoke_Drone.part
		Part	:Projectile_Trail_Glow_Drone.part
		Part	:Projectile_Trail_Glow_Short_Drone.part
		Magnet	TargetOffset
		LookAt	TargetOffset
	End

	Event
		EName	Rings
		Type	Local
		At	Prime
		BhvrOverride
			PyrRotate		0 90 90
		End
		Part	:Projectile_Head_Rings_Drone.part
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

#############################################################

End