#########################################################
##	
##

FxInfo

LifeSpan  400

##################################

Condition
	On 	Time
	Time 	35


	Event
		ENAME   p1
		Type	Local 
		At	WepR
		Geom	centerdummy
	End

	Event
		EName 	Prime2
		Type	Start
		At	p1
		Altpiv 1
		
		Part	:HeavyVillianSniper_Tracer.part
		Part	:HeavyVillianSniper_Tracer.part
		Part	:HeavyVillianSniper_Tracer.part

		Part	:HeavyVillianSniper_Head.part
		
		#BHVR	Behaviors/GenericParticleFade.bhvr

		Magnet	Target
		Lookat	Target

	End
	
	Event
		Type	Start
		At	p1
		Altpiv 1
		
		Part	:HeavyEnergyBlast3.part
		Part	:HeavyEnergyBlast2.part
		Part	:HeavyEnergyBlast3.part
		Part	:HeavyEnergyBlast2.part
		Part	:HeavyEnergyBlastHead2.part


		Magnet	Target
		Lookat	Target
		LifeSpan	40
	End

	Event
		Type	Start
		At	p1
		Altpiv 1
		
		Part	:HeavyEnergyBlastCore.part


		Magnet	Target
		Lookat	Target
		LifeSpan	30
	End

	Event
		EName 	Prime
		Type	Start
		At	p1
		Altpiv 1
		

		Part	:HeavyEnergyBlastHeadStreaks.part

		Part	:HeavyEnergyBlast.part
		Part	:HeavyEnergyBlastHead.part

		BHVR	:VillianSniperlTracer.bhvr

		Magnet	Target
		Lookat	Target

	End

End

##################################


Condition
	On	PrimeHit
	
	Event
		Type	start
		At	T_Chest
		Part	:HeavyEnergyBlastHitStreaks.part
		Part	:HeavyEnergyBlastHitStreaks2.part
		Part	:HeavyEnergyBlastFlash.part
		LifeSpan	5
	End
	
	Event
		Ename	Prime
		Type	Destroy
	End
	

	Event
		Ename	Prime2
		Type	Destroy
	End

End

End				