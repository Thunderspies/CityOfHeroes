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
			#Part	:ChantryLightWiloWisp01.part
			#Part	:ChantryLightWiloWisp02.part
			#Part	:ChantryLightRays.part
			#Part	:ChantryLightRays2.part
			#Part	:ChantryLightGlows.part
		Part	:ChantryLightRays2.part
		Part	:LightRings.part
		Part	:ChantryLightSpark.part
		Part	:ChantryLightStarStreeks.part

	End

	
End


End
