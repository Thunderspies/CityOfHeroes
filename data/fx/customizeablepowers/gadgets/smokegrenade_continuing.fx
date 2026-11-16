#############################################################
## SmokeGrenade_Continuing.FX
#############################################################

FxInfo

#############################################################

Input
	Inpname	Hips
End

######################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	StoneDustRingHit
		Type	local
		At	chest
		Part	:TearGasContinuing.part
		Sound soot_loop 60 60 .45
	End
End

######################################################################

End

