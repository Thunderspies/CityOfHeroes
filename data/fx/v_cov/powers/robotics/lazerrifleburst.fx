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
		
		Part	:VillianSniper_Tracer.part
		Part	:VillianSniper_Tracer.part
		Part	:VillianSniper_Tracer.part

		Part	:VillianSniper_Head.part
		
		Part	:EnergyBlast.part
		Part	:EnergyBlastHead.part

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