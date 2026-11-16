#############################################################
## Peacebringer - Quantum Shield
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Hips1
		Type	local
		At	root
		BHVR	:OuterCircleRotate.bhvr
		part	:OutterMezShellFrontFacing.part
		part	:SpinningRingCore.part
	End

	Event
		EName	Hips1
		Type	local
		At	root
		BHVR	:OuterCircleRotateInverse.bhvr
		part	:OutterMezShellFrontFacing.part
		part	:SpinningRingCore.part
	End
End

#############################################################

End