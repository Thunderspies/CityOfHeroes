#############################################################
## Dark_Continuing.fx
#############################################################

FxInfo

########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		ENAME   Tendrills
		Type	Local
		At	chest
		Part	:ChillOfNightTendril.part
		Part	:ChillOfNightTendril_Subtractive.part
		Sound miasma_loop 55 55 .55
	End

	Event
		EName 	Hit
		Type	local
		At	chest
		part	:DefenceHitMist.part
		part	:DefenceHitMist2.part
		Part	:DefenceHitMist2_Subtractive.part
	End
End

#############################################################

End