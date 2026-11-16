#############################################################
## DronePlasmaBeam_Attack.fx
#############################################################

FxInfo
LifeSpan 90

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
		Type	Local
		At	OffsetAnchor
		AltPiv	1
		Part	:Spray_Plasma_Jet_Drone.part
		Part	:Spray_Plasma_Body_Drone.part
		Part	:Spray_Plasma_Smoke_Drone.part

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