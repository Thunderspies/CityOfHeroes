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

	Event
		Type	local
		At	chest

		Part	:LtBlastAOETrailBlack.part
		Part	:LtBlastAOETrail.part
		Part	:LtBlastAOETrailLight.part

	End

End

End