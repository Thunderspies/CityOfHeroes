#########################################################
##	
##

FxInfo

LifeSpan  400

##################################

Condition
	On 	Time
	Time 	45


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