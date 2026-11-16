#########################################################
##	
##

FxInfo

LifeSpan  400

##################################

Condition
	On 	Time
	Time 	33

	Event
		ENAME   targetDummy
		Type	Posit 
		At	t_chest
		Geom	Centerdummy
		Lookat	origin
	End

	Event
		ENAME   p1offset
		Type	local 
		At	targetDummy
		altpiv	1
		bhvr	:LazerOffset.bhvr
		#Sound	laserRifleBurst 80 70 .88
		
	End

	Event
		ENAME   p1
		Type	Local 
		At	WepR
		Geom	centerdummy
	End

	Event
		EName 	Prime
		Type	Start
		At	p1
		Altpiv 1
		
		Part	:LargeVillianSniper_Tracer_Red.part
		Part	:LargeVillianSniper_Tracer_Red.part
		Part	:LargeVillianSniper_Tracer_Red.part

		Part	:LargeVillianSniper_Head_Red.part
		Part	:LargeVillianSniper_Trail_Red.part
		
		Part	:LargeEnergyBlast_Red.part
		Part	:LargeEnergyBlastHead_Red.part

		BHVR	:VillianSniperlTracer.bhvr

		Magnet	targetDummy
		Lookat	targetDummy

	End
	
End

##################################


Condition
	On	PrimeHit
	
	Event
		Ename	Prime
		Type	Destroy
	End

End

End				