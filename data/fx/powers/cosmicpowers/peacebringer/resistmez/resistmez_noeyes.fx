#########################################################
##	FireAura
FxInfo



######################################################################
Condition
	On	Time
	Time	0

	Event
		Type	local
		At	root
		Bhvr 	:ResistMez.bhvr
		
		Geom	CosmicPeaceBringerShield
				
	End

	Event
		EName	Hips2
		Type	local
		At	root
		BHVR	:CoreOffset.bhvr
		part	:MezShell.part
		part	:MezShell.part
		#part	:OutterMezShell.part
				
	End

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

End				