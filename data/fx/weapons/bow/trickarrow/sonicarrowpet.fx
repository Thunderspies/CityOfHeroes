#########################################################
##	template

FxInfo

##############################################################################
##Rings
##############################################################################

Condition
	On 	cycle
	Time 	17

	Event
		EName 	arrow
		Type	start
		At	root

		#Part	:SonicCoreRings.part
		Part	:SonicRingsOut2.part
		Part	:SonicRingsOut3.part

		LifeSpan	15
	End

End

##############################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	arrow
		Type	start
		At	root

		Sound SonicArrow_loop 100 100 .95
	End

End

End

