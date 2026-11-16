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
		Part	:ChantryLightWiloWisp01.part
		Part	:ChantryLightWiloWisp02.part
		Part	:ChantryLightRays.part
		Part	:ChantryLightRays2.part
		Part	:ChantryLightStreaks.part
		Part	:ChantryLightRings.part
		#Part	:ChantryLightSpark.part
		Part	:ChantryLightStar.part

	End

	
End


End
