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
	Time	12
	Event
		Type	Local
		At	Root
		Sound 	DUST_PlasmaStream_Attack_01 100 100 .8
	End
	Event
		Type	Local
		At	Root
		Sound 	Plasma_Spray 100 100 .8
	End

End


Condition
	On 	Time
	Time 	0

	Event
		ENAME   GunOffset
		Type	Local
		At	WepR
		Geom	FlameThrower
	End
End

Condition
	On	Time
	Time	12

	Event
		Ename	Flash
		Type	Local
		At	GunOffset
		AltPiv	1
		BhvrOverride
			PositionOffset	0.1 0.0 0.0
		End
		Part	:Muzzle_Flash_Drone.part
		Part	:Muzzle_Flash_Core_Drone.part
		Part	:Muzzle_Flash_Shockwave_Drone.part
	End

	Event
		Ename 	Prime
		Type	Local
		At	GunOffset
		AltPiv	1
		BhvrOverride
			PositionOffset	-0.5 0.0 0.0
		End
		Part	:Spray_Plasma_Jet_DUST.part
		Part	:Spray_Plasma_Body_DUST.part
		Part	:Spray_Plasma_Smoke_DUST.part

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