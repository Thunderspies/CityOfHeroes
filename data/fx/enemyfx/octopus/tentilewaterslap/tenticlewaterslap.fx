#########################################################
##	cot_waterfall_med.fx
##
FxInfo

LifeSpan	60

###########################################################

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	Root
		Sound octopusSplash1 100 100 .89
		Sound octopusSplash2 100 100 .9
	End

End



Condition
	On	Time
	Time	15
	
	Event
		Ename	PrimeOriginOffset
		Type	local
		At	root
		BHVR	:OffsetTarget.bhvr
		Geom	centerdummy
		
				
	End


	Event
		Ename	PrimeTarget
		Type	local
		At	root
		BHVR	:OffsetTarget2.bhvr
		Geom	centerdummy
		
				
	End

	Event
		Ename	Prime
		Type	start
		At	PrimeOriginOffset
		BHVR	:OffsetRings.bhvr
		Part	:SplashbodyCore.part
		Part	:SplashbodyCore2.part
		Part	:SplashbodyMist.part
		Part	:SplashbodyCore3.part
		magnet  PrimeTarget
				
	End

End


Condition
	On	PrimeHit
	
	Event
		Ename	Prime
		Type	Destroy
	End

End

End			