#########################################################
##	
##

FxInfo

LifeSpan  300

##################################

Condition
	On 	Time
	Time 	80

	Event

		Type	local
		At	origin
		Sound laserRifleBurst 80 80 .88
	End

End


Condition
	On 	Time
	Time 	80


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