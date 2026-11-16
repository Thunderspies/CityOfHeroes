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
		Geom	Mastermind_Military_Rifle
		Sound pulserifle2 80 80 .85
	End

	Event
		Type	Local
		At	p1
		Altpiv 1

		Part	:BaseBlastOriginStar.part
		Part	:BaseBlastOriginEnergy.part
		Part	:BaseBlastOriginEnergy.part
		LifeSpan	15
	End

	Event
		EName 	Prime
		Type	Start
		At	p1
		Altpiv 1
		
		#Part	:BaseVillianSniper_Tracer.part
		#Part	:BaseVillianSniper_Tracer.part
		#Part	:BaseVillianSniper_Tracer.part

		Part	:BaseVillianSniper_Head.part
		
		Part	:BaseEnergyBlast.part
		Part	:BaseEnergyBlastHead.part

		BHVR	:VillianSniperlTracer.bhvr

		Magnet	Target
		Lookat	Target

	End

End


Condition
	On 	Time
	Time 	37

	Event
		EName 	Prime1
		Type	Start
		At	p1
		Altpiv 1
		
		#Part	:BaseVillianSniper_Tracer.part
		#Part	:BaseVillianSniper_Tracer.part
		#Part	:BaseVillianSniper_Tracer.part

		Part	:BaseVillianSniper_Head.part
		
		Part	:BaseEnergyBlast.part
		Part	:BaseEnergyBlastHead.part

		BHVR	:VillianSniperlTracer.bhvr

		Magnet	Target
		Lookat	Target

	End

End


Condition
	On 	Time
	Time 	41
	
	Event
		EName 	Prime2
		Type	Start
		At	p1
		Altpiv 1
		
		#Part	:BaseVillianSniper_Tracer.part
		#Part	:BaseVillianSniper_Tracer.part
		#Part	:BaseVillianSniper_Tracer.part

		Part	:BaseVillianSniper_Head.part
		
		Part	:BaseEnergyBlast.part
		Part	:BaseEnergyBlastHead.part

		BHVR	:VillianSniperlTracer.bhvr

		Magnet	Target
		Lookat	Target

	End
End


Condition
	On 	Time
	Time 	45
	
	Event
		EName 	Prime3
		Type	Start
		At	p1
		Altpiv 1
		
		#Part	:BaseVillianSniper_Tracer.part
		#Part	:BaseVillianSniper_Tracer.part
		#Part	:BaseVillianSniper_Tracer.part

		Part	:BaseVillianSniper_Head.part
		
		Part	:BaseEnergyBlast.part
		Part	:BaseEnergyBlastHead.part

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

Condition
	On	Prime1Hit
	
	Event
		Ename	Prime1
		Type	Destroy
	End

End

Condition
	On	Prime2Hit
	
	Event
		Ename	Prime2
		Type	Destroy
	End

End

Condition
	On	Prime3Hit
	
	Event
		Ename	Prime3
		Type	Destroy
	End

End

End				