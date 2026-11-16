#########################################################
##	Fire_Ball
##
FxInfo

Flags    InheritAlpha

Condition
	
	Event
		Ename	PumpkinEyes
		Type	local
		At	Head
		Geom	pumpkinheadeyes
	End

	Event
		Type	local
		At	PumpkinEyes
		altpiv	1
		
		Part	:PumpkinEyes.part
		Part	:PumpkinPupil.part
	End
	
	Event
		Type	local
		At	PumpkinEyes
		altpiv	2
		
		Part	:PumpkinEyes.part
		Part	:PumpkinPupil.part
	End

End

End	


			