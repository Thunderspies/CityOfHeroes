#############################################################
## PoisonTrapContinuing.fx
#############################################################

FxInfo

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		Part	:LtBlastAOETrailBlack.part
		Part	:LtBlastAOETrail.part
		Part	:LtBlastAOETrailLight.part
	End
End

#############################################################

End