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
		Type	Local
		At	p1
		Altpiv 1
#		BHVR	:BlastOffset.bhvr
		Part	:BlastOriginStar.part
		Part	:BlastOriginEnergy.part
		Part	:BlastOriginEnergy.part
		Sound RobotLaserRed2 90 90 .8
		LifeSpan	5
	End

	Event
		EName 	Prime
		Type	Start
		At	p1
		Altpiv 1
		
		Part	:VillianSniper_Tracer.part
		Part	:VillianSniper_TracerCore.part

		Part	:VillianSniper_Head.part
		
		Part	:EnergyBlast.part
		Part	:EnergyBlastHead.part

		BHVR	:VillianSniperlTracer.bhvr

		Magnet	Target
		Lookat	Target

	End

End


#Condition
#	On 	Time
#	Time 	41
#
#	Event
#		EName 	Prime1
#		Type	Start
#		At	p1
#		Altpiv 1
#		
#		Part	:VillianSniper_Tracer_red.part
#		Part	:VillianSniper_Tracer_red.part
#		Part	:VillianSniper_Tracer_red.part
#
#		Part	:VillianSniper_Head_red.part
#		
#		Part	:EnergyBlast_red.part
#		Part	:EnergyBlastHead_red.part
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
#	Time 	45
#	
#	Event
#		EName 	Prime2
#		Type	Start
#		At	p1
#		Altpiv 1
#		
#		Part	:VillianSniper_Tracer_red.part
#		Part	:VillianSniper_Tracer_red.part
#		Part	:VillianSniper_Tracer_red.part
#
#		Part	:VillianSniper_Head_red.part
#		
#		Part	:EnergyBlast_red.part
#		Part	:EnergyBlastHead_red.part
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
#	Time 	49
#	
#	Event
#		EName 	Prime3
#		Type	Start
#		At	p1
#		Altpiv 1
#		
#		Part	:VillianSniper_Tracer_red.part
#		Part	:VillianSniper_Tracer_red.part
#		Part	:VillianSniper_Tracer_red.part
#
#		Part	:VillianSniper_Head_red.part
#		
#		Part	:EnergyBlast_red.part
#		Part	:EnergyBlastHead_red.part
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