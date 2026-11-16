#########################################################
##	
##

FxInfo

LifeSpan  1400

##################################

Condition
	On 	Time
	Time 	33

	Event

		Type	local
		At	origin
		Sound RobotLaserRed2 90 90 .8
	End

End

Condition
	On 	Time
	Time 	33

	Event
		ENAME   p1
		Type	Local 
		At	LarmR
		
		Geom	LazerHookup
	End

	Event
		Type	Local
		At	p1
		Altpiv 1
#		BHVR	:BlastOffset.bhvr
		Part	:BlastOriginStar.part
		Part	:BlastOriginEnergy.part
		Part	:BlastOriginEnergy.part
		
		LifeSpan	5
	End

	Event
		EName 	Prime
		Type	Start
		At	p1
		Altpiv 1
		
		#Part	:VillianSniper_Tracer.part
		#Part	:VillianSniper_TracerCore.part

		Part	:VillianSniper_Head.part
		Part	:VillianSniper_HeadCore.part
		
		#Part	:EnergyBlast.part
		#Part	:EnergyBlastCore.part

		BHVR	:VillianSniperlTracer.bhvr

		Magnet	WepR
		#Lookat	T_Chest

	End

End


Condition
	On 	Time
	Time 	37

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

	Event
		EName 	Prime1
		Type	Start
		At	p1
		Altpiv 2
		
		#Part	:VillianSniper_Tracer.part
		#Part	:VillianSniper_TracerCore.part

		Part	:VillianSniper_Head.part
		Part	:VillianSniper_HeadCore.part
		
		#Part	:EnergyBlast.part
		#Part	:EnergyBlastCore.part

		BHVR	:VillianSniperlTracer.bhvr

		Magnet	WepR
		#Lookat	T_Chest

	End

End


Condition
	On 	Time
	Time 	41
	
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

	Event
		EName 	Prime2
		Type	Start
		At	p1
		Altpiv 3

		#Part	:VillianSniper_Tracer.part
		#Part	:VillianSniper_TracerCore.part

		Part	:VillianSniper_Head.part
		Part	:VillianSniper_HeadCore.part
		
		#Part	:EnergyBlast.part
		#Part	:EnergyBlastCore.part

		BHVR	:VillianSniperlTracer.bhvr

		Magnet	WepR
		#Lookat	T_Chest

	End
End


Condition
	On 	Time
	Time 	45
	
	Event
		Type	Local
		At	p1
		Altpiv 4
#		BHVR	:BlastOffset.bhvr
		Part	:BlastOriginStar.part
		Part	:BlastOriginEnergy.part
		Part	:BlastOriginEnergy.part
		LifeSpan	5
	End

	Event
		EName 	Prime3
		Type	Start
		At	p1
		Altpiv 4
		
		#Part	:VillianSniper_Tracer.part
		#Part	:VillianSniper_TracerCore.part

		Part	:VillianSniper_Head.part
		Part	:VillianSniper_HeadCore.part
		
		#Part	:EnergyBlast.part
		#Part	:EnergyBlastCore.part

		BHVR	:VillianSniperlTracer.bhvr

		Magnet	WepR
		#Lookat	T_Chest

	End
End

Condition
	On 	Time
	Time 	45
	
	Event
		Type	Local
		At	p1
		Altpiv 1
#		BHVR	:BlastOffset.bhvr
		Part	:BlastOriginStar.part
		Part	:BlastOriginEnergy.part
		Part	:BlastOriginEnergy.part
		LifeSpan	5
	End

	Event
		EName 	Prime4
		Type	Start
		At	p1
		Altpiv 1
		
		#Part	:VillianSniper_Tracer.part
		#Part	:VillianSniper_TracerCore.part

		Part	:VillianSniper_Head.part
		Part	:VillianSniper_HeadCore.part
		
		#Part	:EnergyBlast.part
		#Part	:EnergyBlastCore.part

		BHVR	:VillianSniperlTracer.bhvr

		Magnet	WepR
		#Lookat	T_Chest

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