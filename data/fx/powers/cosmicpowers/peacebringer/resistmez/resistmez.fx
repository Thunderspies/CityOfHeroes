#############################################################
## Peacebringer - Quantum Shield
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Sound
		Type	local
		At	Chest
		bhvr	behaviors/soundFade3.bhvr
		Sound glow10b_loop 60 60 .4
		Flags	PowerLoopingSound
		Lifespan 220
	End

	Event
		Type	Posit
		At	Hips
		BHVR	:OuterCircleRotate.bhvr
		BhvrOverride
			PositionOffset		0 0 0
			PyrRotate		0 90 90
			Spin			0.05 0.0 0.0
			Scale			1 0.9 1
		End
		part	:OutterMezShellFrontFacing.part
		part	:SpinningRingCore.part
		Sound regen3 60 60 .7
	End

	Event
		Type	Posit
		At	Hips
		BHVR	:OuterCircleRotate.bhvr
		BhvrOverride
			PositionOffset		0 0 0
			PyrRotate		0 90 90
			Spin			-0.05 0.0 0.0
			Scale			1 0.9 1
		End
		part	:OutterMezShellFrontFacing.part
		part	:SpinningRingCore.part
	End
End

#############################################################

End