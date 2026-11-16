#########################################################
##	
##

FxInfo

LifeSpan  400

##################################
Condition
	On 	Time
	Time 	39

	Event

		Type	local
		At	origin
		Sound pulserifle2 80 80 .85
	End

End
Condition
	On 	Time
	Time 	39

	Event
		ENAME   p1
		Type	Local 
		At	WepR
		Geom	newrobotics
		#Geom	Mastermind_Military_Rifle
		
	End

	Event
		Type	Local
		At	p1
		Altpiv 2
#		BHVR	:BlastOffset.bhvr
		Part	:BlastOriginStar.part
		Part	:BlastOriginEnergy.part
		Part	:BlastOriginEnergy.part
		LifeSpan	5
	End

End

Condition
	On 	Time
	Time 	42

	Event
		EName 	Prime
		Type	Start
		At	p1
		Altpiv 2
		
#		Part	:VillianSniper_Tracer.part
#		Part	:VillianSniper_TracerCore.part

		Part	:VillianSniper_Head.part
		Part	:VillianSniper_HeadCore.part
		
#		Part	:EnergyBlast.part
#		Part	:EnergyBlastHead.part

		BHVR	:VillianSniperlTracer.bhvr

		Magnet	Target
		Lookat	Target

	End

End

#
#Condition
#	On 	Time
#	Time 	43
#
#	Event
#		EName 	Prime1
#		Type	Start
#		At	p1
#		Altpiv 2
#		
#		Part	:VillianSniper_Tracer.part
#		Part	:VillianSniper_Tracer.part
#		Part	:VillianSniper_Tracer.part
#
#		Part	:VillianSniper_Head.part
#		
#		Part	:EnergyBlast.part
#		Part	:EnergyBlastHead.part
#
#		BHVR	:VillianSniperlTracer.bhvr
#
#		Magnet	Target
#		Lookat	Target
#
#	End
#
#End
#
#
#Condition
#	On 	Time
#	Time 	42
#	
#	Event
#		EName 	Prime2
#		Type	Start
#		At	p1
#		Altpiv 2
#		
#		Part	:VillianSniper_Tracer.part
#		Part	:VillianSniper_Tracer.part
#		Part	:VillianSniper_Tracer.part
#
#		Part	:VillianSniper_Head.part
#		
#		Part	:EnergyBlast.part
#		Part	:EnergyBlastHead.part
#
#		BHVR	:VillianSniperlTracer.bhvr
#
#		Magnet	Target
#		Lookat	Target
#
#	End
#End
#
#
#Condition
#	On 	Time
#	Time 	46
#	
#	Event
#		EName 	Prime3
#		Type	Start
#		At	p1
#		Altpiv 2
#		
#		Part	:VillianSniper_Tracer.part
#		Part	:VillianSniper_Tracer.part
#		Part	:VillianSniper_Tracer.part
#
#		Part	:VillianSniper_Head.part
#		
#		Part	:EnergyBlast.part
#		Part	:EnergyBlastHead.part
#
#		BHVR	:VillianSniperlTracer.bhvr
#
#		Magnet	Target
#		Lookat	Target
#
#	End
#End
#
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