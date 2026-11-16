#########################################################
##	
##

FxInfo

LifeSpan  400

##################################

Condition
	On 	Time
	Time 	23

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
		At	P1
		Altpiv	1
		#BHVR	:WepROffset.bhvr
		Part	:BlastOriginStar.part
		Part	:BlastOriginEnergy.part
		Part	:BlastOriginEnergyRed.part
		LifeSpan	10
	End

End

Condition
	On 	Time
	Time 	8


	Event
		Type	Local 
		At	LarmR
		Sound robotLaserRifleBurst5b 110 110 .92
	End

End

Condition
	On 	Time
	Time 	33

	Event
		EName 	Prime
		Type	Start
		At	p1
		Altpiv 1
		
		Part	:VillianSniper_Head.part
		Part	:VillianSniper_HeadCore.part

		BHVR	:VillianSniperlTracer.bhvr

		Magnet	targetDummy
		Lookat	targetDummy

	End
	

End

Condition
	On 	Time
	Time 	33

	Event
		Type	Local
		At	P1
		Altpiv	1
		#BHVR	:WepROffset.bhvr
		Part	:BlastOriginStar.part
		Part	:BlastOriginEnergy.part
		Part	:BlastOriginEnergyRed.part
		LifeSpan	10
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
		
		Part	:VillianSniper_Head.part
		Part	:VillianSniper_HeadCore.part

		BHVR	:VillianSniperlTracer.bhvr

		Magnet	targetDummy
		Lookat	targetDummy

	End
	

End


Condition
	On 	Time
	Time 	39

	Event
		EName 	Prime2
		Type	Start
		At	p1
		Altpiv 1
		
		Part	:VillianSniper_Head.part
		Part	:VillianSniper_HeadCore.part

		BHVR	:VillianSniperlTracer.bhvr

		Magnet	targetDummy
		Lookat	targetDummy

	End
	

End

#
#Condition
#	On 	Time
#	Time 	17
#
#	Event
#		Type	Start
#		At	WepR
#		
#		ChildFx	:2ndMechLargeLazerRifleBurstChild.fx
#
#		LifeSpan	300
#
#		Magnet	Target
#		Lookat	Target
#	End
#	
#End
#
#
#Condition
#	On 	Time
#	Time 	23
#
#	Event
#		Type	Start
#		At	WepR
#		
#		ChildFx	:2ndMechLargeLazerRifleBurstChild.fx
#
#		LifeSpan	300
#
#		Magnet	Target
#		Lookat	Target
#	End
#	
#End
#
#Condition
#	On 	Time
#	Time 	27
#
#	Event
#		Type	Start
#		At	WepR
#		
#		ChildFx	:2ndMechLargeLazerRifleBurstChild.fx
#
#		LifeSpan	300
#
#		Magnet	Target
#		Lookat	Target
#	End
#	
#End
#
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
End				