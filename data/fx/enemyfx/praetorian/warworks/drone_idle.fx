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
		Geom	FX_WWDefenseTurret_Offsets
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
			PositionOffset	0 0 0.5
		End
	End

	Event
		Ename 	Prime
		Type	Local
		At	OffsetAnchor1
		Part	:Muzzle_Glow_Highlight_Drone.part
	End

	Event
		Ename 	Prime
		Type	Local
		At	OffsetAnchor1
		BhvrOverride
			PositionOffset	0.0 0.0 -0.2
		End
		Part	:Muzzle_Glow_Drone.part
		POther	TargetOffset
		LookAt	TargetOffset
	End

	Event
		Ename 	Prime
		Type	Local
		At	OffsetAnchor2
		BhvrOverride
			PositionOffset	0.0 0.2 0.1
		End
		Part	:Exhaust_Drone.part

	End

End

#############################################################

End