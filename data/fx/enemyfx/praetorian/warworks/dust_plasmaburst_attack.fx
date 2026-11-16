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
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	PPD_PlasmaBurst_Hit_01 100 100 .8
	End
End


Condition
	On 	Time
	Time 	0

	Event
		Ename	GunOffset
		Type	Local
		At	WepR
		Geom	FlameThrower
	End


End

Condition
	On	Time
	Time	3

	Event
		Ename	Flash
		Type	Start
		At	GunOffset
		AltPiv	1
		Part	:Muzzle_Flash_Drone.part
		Part	:Muzzle_Flash_Core_Drone.part
		Part	:Muzzle_Flash_Shockwave_Drone.part
	End

	Event
		Ename 	Prime
		Type	Start
		At	GunOffset
		AltPiv	1
		BhvrOverride
			PyrRotate		0 0 90
			TrackRate		6.0
		End
		Part	:Projectile_Trail_Ember_Drone_Bright.part
		Part	:Projectile_Trail_Smoke_Drone.part
		Part	:Projectile_Trail_Glow_Drone.part
		Part	:Projectile_Trail_Glow_Short_Drone.part
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