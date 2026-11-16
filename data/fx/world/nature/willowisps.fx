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
		Part	:WiloWisp01.part
		Part	:WiloWisp02.part
		Part	:WilloWispLightRays.part
		Part	:WilloWispLightRays2.part
		Part	:WilloWispLightStreaks.part
		Part	:WilloWispRings.part
		Part	:WilloWispSpark.part
		Part	:WilloWispStar.part
	End

	
End


End
