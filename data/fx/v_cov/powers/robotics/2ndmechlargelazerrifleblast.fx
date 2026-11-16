#########################################################
##	
##

FxInfo

LifeSpan  400

##################################

Condition
	On 	Time
	Time 	0

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
	Time 	33

	Event
		Type	Local
		At	p1
		altpiv	3
		Part	:BlastOriginStar.part
		Part	:BlastOriginEnergy.part
		Part	:BlastOriginEnergyRed.part
		Sound RobotLaserRifleBurst3 90 90 .8
		LifeSpan	10
	End

	Event
		EName 	Prime
		Type	Start
		At	p1
		Altpiv 3
		
		Part	:LargeVillianSniper_Head.part
		Part	:LargeVillianSniper_HeadCore.part
		
		Part	:LargeEnergyBlast.part
		Part	:LargeEnergyBlastCore.part


		BHVR	:VillianSniperlTracer.bhvr

		Magnet	targetDummy
		Lookat	targetDummy

	End
	
End

Condition
	On 	Time
	Time 	36

	
	Event
		Type	Local
		At	p1
		altpiv	3
		Part	:BlastOriginStar.part
		Part	:BlastOriginEnergy.part
		Part	:BlastOriginEnergyRed.part
		LifeSpan	10
	End

	Event
		EName 	Prime1
		Type	Start
		At	p1
		Altpiv 3
		
		Part	:LargeVillianSniper_Head.part
		Part	:LargeVillianSniper_HeadCore.part
		
		Part	:LargeEnergyBlast.part
		Part	:LargeEnergyBlastCore.part


		BHVR	:VillianSniperlTracer.bhvr

		Magnet	targetDummy
		Lookat	targetDummy

	End
	
End

Condition
	On 	Time
	Time 	40

	
	Event
		Type	Local
		At	p1
		altpiv	3
		Part	:BlastOriginStar.part
		Part	:BlastOriginEnergy.part
		Part	:BlastOriginEnergyRed.part
		LifeSpan	10
	End

	Event
		EName 	Prime2
		Type	Start
		At	p1
		Altpiv 3
		
		Part	:LargeVillianSniper_Head.part
		Part	:LargeVillianSniper_HeadCore.part
		
		Part	:LargeEnergyBlast.part
		Part	:LargeEnergyBlastCore.part


		BHVR	:VillianSniperlTracer.bhvr

		Magnet	targetDummy
		Lookat	targetDummy

	End
	
End

Condition
	On 	Time
	Time 	45

	
	Event
		Type	Local
		At	p1
		altpiv	3
		Part	:BlastOriginStar.part
		Part	:BlastOriginEnergy.part
		Part	:BlastOriginEnergyRed.part
		LifeSpan	10
	End

	Event
		EName 	Prime3
		Type	Start
		At	p1
		Altpiv 3
		
		Part	:LargeVillianSniper_Head.part
		Part	:LargeVillianSniper_HeadCore.part
		
		Part	:LargeEnergyBlast.part
		Part	:LargeEnergyBlastCore.part


		BHVR	:VillianSniperlTracer.bhvr

		Magnet	targetDummy
		Lookat	targetDummy

	End
	
End

#
#Condition
#	On 	Time
#	Time 	23
#
#	Event
#		Type	Start
#		At	p1
#		altpiv	3
#		
#		ChildFx	:2ndMechLargeLazerRifleBlastChild.fx
#
#		LifeSpan	300
#
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
#		At	p1
#		altpiv	3
#		
#		ChildFx	:2ndMechLargeLazerRifleBlastChild.fx
#
#		LifeSpan	300
#
#	End
#	
#End

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