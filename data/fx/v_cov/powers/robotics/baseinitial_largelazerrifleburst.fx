#########################################################
##	
##

FxInfo

LifeSpan  300

##################################

Condition
	On 	Time
	Time 	43


	Event
		ENAME   p1
		Type	Local 
		At	WepR
		Geom	centerdummy
		Sound laserRifleBurst 80 80 .88
	End

	Event
		EName 	Prime
		Type	Start
		At	p1
		Altpiv 1
		
		Part	:LargeVillianSniper_Tracer.part
		Part	:LargeVillianSniper_Tracer.part
		Part	:LargeVillianSniper_Tracer.part

		Part	:LargeVillianSniper_Head.part
		Part	:LargeVillianSniper_Trail.part
		
		Part	:LargeEnergyBlast.part
		Part	:LargeEnergyBlastHead.part

		BHVR	:VillianSniperlTracer.bhvr

		Magnet	Target
		Lookat	Target

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