#########################################################
##	Fire_Ball
##
FxInfo

Flags    InheritAlpha

Condition
	
	Event
		Ename	FireBallHit
		Type	posit
		At	Hair
		Bhvr	:fireOffset.bhvr
		Part	:HeadFire.part
		Part	:HeadFire2.part
		Part	:Spark1.part
		Sound torch1_loop 140 140 .3
	End

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


			