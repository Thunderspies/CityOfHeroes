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

End

Condition
	On 	Time
	Time 	20


	Event
		Type	Local 
		At	LarmR
		Sound robotLaserRifleBurst2 110 110 .92
	End

End


Condition
	On 	Time
	Time 	60

	Event
		Type	Local
		At	p1
		Altpiv 2

		Part	:BlastOriginStar.part
		Part	:BlastOriginEnergy.part
		Part	:BlastOriginEnergy.part
		LifeSpan	5
	End

	Event
		Type	Local
		At	p1
		Altpiv 3

		Part	:BlastOriginStar.part
		Part	:BlastOriginEnergy.part
		Part	:BlastOriginEnergy.part
		LifeSpan	5
	End

End

Condition
	On 	Time
	Time 	65

	Event
		EName 	Prime
		Type	Start
		At	p1
		Altpiv 2
		

		Part	:VillianSniper_Head.part
		Part	:VillianSniper_HeadCore.part


		BHVR	:VillianSniperlTracerOffset1.bhvr

		Magnet	targetDummy
		Lookat	targetDummy

	End
	
	Event
		EName 	Prime1
		Type	Start
		At	p1
		Altpiv 3
		

		Part	:VillianSniper_Head.part
		Part	:VillianSniper_HeadCore.part

		BHVR	:VillianSniperlTracerOffset.bhvr

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


Condition
	On	Prime1Hit
	
	Event
		Ename	Prime1
		Type	Destroy
	End

End

End				