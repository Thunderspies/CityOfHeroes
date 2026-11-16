#############################################################
## Drone_Idle.fx
#############################################################

FxInfo

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
	End

	Event
		EName	OffsetAnchor2
		Type	Local
		At	OffsetAnchor
		AltPiv	2
	End

	Event
		EName	TargetOffset
		Type	Local
		At	OffsetAnchor1
		BhvrOverride
			PositionOffset	0 0 1.35
		End
	End

	Event
		Type	Local
		At	OffsetAnchor1
		BhvrOverride
			PositionOffset	0.0 0.0 0.2
		End
		Part	:Muzzle_Glow_Highlight_Drone_Big.part
	End

	Event
		Type	Local
		At	OffsetAnchor1
		BhvrOverride
			PositionOffset	0.0 0.0 -0.2
		End
		Part	:Muzzle_Glow_Drone_Big.part
		POther	TargetOffset
		LookAt	TargetOffset
	End

	Event
		Type	Local
		At	OffsetAnchor2
		BhvrOverride
			PositionOffset	0.0 0.2 0.1
		End
		Part	:Exhaust_Drone.part
	End

	Event
		Type	Local
		At	OffsetAnchor
		AltPiv	4
		Part	:Exhaust_Side_Drone.part
	End

	Event
		Type	Local
		At	OffsetAnchor
		AltPiv	5
		BhvrOverride
			PositionOffset	0.0 0.2 0.1
		End
		Part	:Exhaust_SideL_Drone.part
	End
End

#############################################################

End