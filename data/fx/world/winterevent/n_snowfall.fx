#############################################################
## Ashes_Heavy.fx
#############################################################

FxInfo
Flags DontSuppress
#############################################################


Condition
	On	Time
	Time	0

	Event
		EName	AshAnchor
		Type	StartPositOnly
		CAMERASPACE
		Flags	OneAtATime
		BhvrOverride
			PositionOffset	0.0 25.0 0.0
		End
		Part	:N_SnowFall_Flakes.part
		Part	:N_SnowFall_FrostyMist.part
	End

	Event
		Ename	GroundPatches
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0.0 0.3 0.0
		End
		Part	:N_SnowFall_SnowPatch.part
	End

End

#############################################################

End


