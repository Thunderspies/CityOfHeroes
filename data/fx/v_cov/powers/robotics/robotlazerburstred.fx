#########################################################
##	
##

FxInfo

LifeSpan  400

##################################

Condition
	On 	Time
	Time 	30

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
		At	LarmR
		
		Geom	LazerHookup
	End

	Event
		Type	Local
		At	p1
		Altpiv 3
#		BHVR	:BlastOffset.bhvr
		Part	:BlastOriginStar.part
		Part	:BlastOriginEnergy.part
		Part	:BlastOriginEnergy.part
		LifeSpan	5
	End

End

Condition
	On 	Time
	Time 	33

	Event
		EName 	Prime
		Type	Start
		At	p1
		Altpiv 3
		
		Part	:VillianSniper_Tracer.part
		Part	:VillianSniper_TracerCore.part

		Part	:VillianSniper_Head.part
		Part	:VillianSniper_HeadCore.part
		
		Part	:EnergyBlast.part
		Part	:EnergyBlastCore.part

		BHVR	:VillianSniperlTracer.bhvr

		Magnet	targetDummy
		Lookat	targetDummy

	End

End

##############################################################################

Condition
	On 	Time
	Time 	33

	Event
		Type	Local
		At	p1
		Altpiv 3
#		BHVR	:BlastOffset.bhvr
		Part	:BlastOriginStar.part
		Part	:BlastOriginEnergy.part
		Part	:BlastOriginEnergy.part
		Sound RobotLaserRed2 90 90 .8
		LifeSpan	5
	End

End

Condition
	On 	Time
	Time 	36

	Event
		EName 	Prime1
		Type	Start
		At	p1
		Altpiv 3
		
		Part	:VillianSniper_Tracer.part
		Part	:VillianSniper_TracerCore.part

		Part	:VillianSniper_Head.part
		Part	:VillianSniper_HeadCore.part
		
		Part	:EnergyBlast.part
		Part	:EnergyBlastCore.part

		BHVR	:VillianSniperlTracer.bhvr

		Magnet	targetDummy
		Lookat	targetDummy

	End

End

#########################################################################


Condition
	On 	Time
	Time 	36

	Event
		Type	Local
		At	p1
		Altpiv 3
#		BHVR	:BlastOffset.bhvr
		Part	:BlastOriginStar.part
		Part	:BlastOriginEnergy.part
		Part	:BlastOriginEnergy.part
		LifeSpan	5
	End

End

Condition
	On 	Time
	Time 	39

	Event
		EName 	Prime2
		Type	Start
		At	p1
		Altpiv 3
		
		Part	:VillianSniper_Tracer.part
		Part	:VillianSniper_TracerCore.part

		Part	:VillianSniper_Head.part
		Part	:VillianSniper_HeadCore.part
		
		Part	:EnergyBlast.part
		Part	:EnergyBlastCore.part

		BHVR	:VillianSniperlTracer.bhvr

		Magnet	targetDummy
		Lookat	targetDummy

	End

End


#################################


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

End				