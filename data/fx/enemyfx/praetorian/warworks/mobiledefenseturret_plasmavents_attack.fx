#############################################################
## Drone_Idle.fx
#############################################################

FxInfo
Lifespan 95

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
		At	Hips
		Type	Local
		Geom	FX_WWMobileDefenseTurret_Offsets
	End

	Event
		EName	OffsetAnchor1
		Type	Local
		At	OffsetAnchor
		AltPiv	1
		Part	:Spray_Plasma_Jet_Small_Drone.part
		Part	:Spray_Plasma_Body_Small_Drone.part
		Part	:Spray_Plasma_Smoke_Small_Drone.part
		Lifespan 90
	End
End

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	OffsetAnchor
		AltPiv	4
		BhvrOverride
			PositionOffset	0.0 0.1 0.0
		End
		Part	:Spray_Plasma_Jet_SmallR_Drone.part
		Part	:Spray_Plasma_Body_SmallR_Drone.part
		Part	:Spray_Plasma_Smoke_SmallR_Drone.part
		Lifespan 95
	End

	Event
		Type	Local
		At	OffsetAnchor
		AltPiv	5
		BhvrOverride
			PositionOffset	0.0 0.1 0.0
		End
		Part	:Spray_Plasma_Jet_SmallL_Drone.part
		Part	:Spray_Plasma_Body_SmallL_Drone.part
		Part	:Spray_Plasma_Smoke_SmallL_Drone.part
		Lifespan 95
	End
End

#############################################################

End