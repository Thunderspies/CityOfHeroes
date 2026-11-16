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
		BHVR	:mouthadj.bhvr
		Geom	pumpkinhead
	End

	Event
		Ename	PumpkinEyes
		Type	local
		At	Head
		BHVR	:mouthadj.bhvr
		Geom	pumpkinheadeyes
	End

	Event
		Type	local
		At	PumpkinEyes
		altpiv	2
		
		Part	:PumpkinEyes.part
		Part	:PumpkinPupil.part
	End
	
	Event
		Type	local
		At	PumpkinEyes
		altpiv	3
		
		Part	:PumpkinEyes.part
		Part	:PumpkinPupil.part
	End

End

End	


			