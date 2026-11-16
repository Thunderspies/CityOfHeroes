#############################################################################
##Head dizzy
########################################################
FxInfo

Input  
	Inpname	origin
End

Condition
	On 	Time
	Time	0

	Event
		EName	core
		Type	start
		At	origin
		Part	:RedLightRays2.part
		Part	:RedLightRings.part
		Part	:RedLightSpark.part
		Part	:RedLightStarStreeks.part

	End

	
End


End
