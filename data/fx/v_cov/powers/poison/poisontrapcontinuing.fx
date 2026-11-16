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
		Part	V_COV\Powers\Poison\LtBlastAOETrailBlack.part
		Part	V_COV\Powers\Poison\LtBlastAOETrail.part
		Part	V_COV\Powers\Poison\LtBlastAOETrailLight.part
	End
End

#############################################################

End