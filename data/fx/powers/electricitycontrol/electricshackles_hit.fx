#############################################################
## JoltingChain_Hit.fx
#############################################################

FxInfo

#############################################################

Input
	Inpname	Chest
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	HandGlow1
		Type	Local
		At	Chest
		Part	:Body_Sparks.part
		Part	:Hand_Ring_Burst.part
		Part	:Hit_Lightning.part
		Part	:Hand_Ring_Burst.part
		Lifespan 30
		Sound	elecfry 80 70 .8
	End
End

#############################################################

End
