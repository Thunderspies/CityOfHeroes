#############################################################
## Veteran Reward Combat Pet - Blue Drone
#############################################################

FxInfo

Flags InheritAlpha

## OFFSETS ###########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	Hips
		Bhvr	behaviors\soundFade3.bhvr
		Sound targetdrone_loop 30 30 .3
		Lifespan 120
	End

	Event
		EName	DroneSpin
		Type	local
		At	Hips
		Bhvr	:DroneSpin.bhvr
	End

	Event
		EName	DroneOffset
		Type	local
		At	DroneSpin
		Bhvr	:DroneOffset.bhvr
		Anim	target_drone
	End
End

#############################################################

Condition
	On 	Time
	Time 	1

	Event
		Type	local
		At	DroneOffset
		Animpiv	Hair
		Part	:Blue_DroneGlow_01.part
	End
End

## LASERS ###########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Laser1Offset
		Type	local
		At	DroneOffset
		Animpiv	Hair
		BhvrOverride
			PositionOffset	0.0 0.0 5.0
		End
	End

	Event
		EName	Laser2Offset
		Type	local
		At	DroneOffset
		Animpiv	Hair
		BhvrOverride
			PositionOffset	0.0 0.0 -5.0
		End
	End

	Event
		EName	Laser3Offset
		Type	local
		At	DroneOffset
		Animpiv	Hair
		BhvrOverride
			PositionOffset	5.0 0.0 0.0
		End
	End

	Event
		EName	Laser4Offset
		Type	local
		At	DroneOffset
		Animpiv	Hair
		BhvrOverride
			PositionOffset	-5.0 0.0 0.0
		End
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	Laser1Offset
		Part1	:Blue_LaserBeam_01.part
		pOther	Laser2Offset
	End

	Event
		Type	local
		At	Laser3Offset
		Part1	:Blue_LaserBeam_01.part
		pOther	Laser4Offset
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName	light
		Type	local
		At	DroneOffset
		Animpiv	Eyes
		Part1	:Blue_LaserBeam_01.part
		Part	:Blue_LaserGlow_01.part
		Part	:Blue_LaserCore_01.part
	End

	Event
		EName	light
		Type	local
		At	DroneOffset
		animpiv	HandL
		Part1	:Blue_LaserBeam_01.part
		Part	:Blue_LaserGlow_01.part
		Part	:Blue_LaserCore_01.part
	End

	Event
		EName	light
		Type	local
		At	DroneOffset
		animpiv	HandR
		Part1	:Blue_LaserBeam_01.part
		Part	:Blue_LaserGlow_01.part
		Part	:Blue_LaserCore_01.part
	End

	Event
		EName	light
		Type	local
		At	DroneOffset
		animpiv	Back
		Part1	:Blue_LaserBeam_01.part
		Part	:Blue_LaserGlow_01.part
		Part	:Blue_LaserCore_01.part
	End
End
#############################################################

End