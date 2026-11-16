#########################################################
## Umbral Aura - Twighlight Shield
#########################################################

FxInfo

## SOUND FX #######################################################

Condition
	On 	Time
	Time 	0
	Event

		Type	Local
		At	Origin
		Sound WarshadeBuild 70 70 .76
	End
End

Condition
	On 	Time
	Time 	12
	Event

		Type	Local
		At	Origin
		Sound green_loop 60 60 .2
	End
End

## EYE FX ######################################################

#Condition
#	On	Time
#	Time	0
#
#	Event
#		EName 	Eyes
#		Type	Local 
#		At	Root
#		ChildFX POWERS\CosmicPowers\Nictus\ActivationEyes\ActivationEyesContinuingFX.fx
#	End
#End

## SHIELD FX #######################################################

Condition
	On	Time
	Time	0

	Event
		Type	local
		At	root
		Bhvr 	:ResistMez.bhvr
		Geom	CosmicNictusShield
	End

	Event
		EName	Hips2
		Type	local
		At	root
		BHVR	:CoreOffset.bhvr
		part	:MezShellBLK.part
		part	:MezShell.part
		part	:MezShellPurple.part
	End

	Event
		EName	Hips1
		Type	local
		At	root
		BHVR	:OuterCircleRotate.bhvr
		#part	:OutterMezShellFrontFacing.part
		part	:OutterRingTendrilsPurple.part
		part	:OutterRingTendrilsBLK.part
		#part	:SpinningRingCore.part
	End

	Event
		EName	Hips1
		Type	local
		At	root
		BHVR	:OuterCircleRotateInverse.bhvr
		#part	:OutterMezShellFrontFacing.part
		part	:OutterRingTendrilsPurple.part
		part	:OutterRingTendrilsBLK.part
		#part	:SpinningRingCore.part
	End
End

#########################################################

End				