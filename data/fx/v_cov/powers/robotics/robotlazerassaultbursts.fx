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
		BHVR	:BlastOffset.bhvr
		Part	:BlastOriginStar.part
		Part	:BlastOriginEnergy.part
		Part	:BlastOriginEnergy.part
		LifeSpan	120
	End

	Event
		EName 	Prime
		Type	Start
		At	WepR
		
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

	Event
		ENAME   p1
		Type	Local 
		At	WepR
		ChildFx	:RobotLazerBurstRed.fx
	End

End

Condition
	On 	Time
	Time 	38


	Event
		EName 	Prime1
		Type	Start
		At	WepR
		
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

Condition
	On 	Time
	Time 	43

	Event
		EName 	Prime2
		Type	Start
		At	WepR
		
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

	Event
		ENAME   p1
		Type	Local 
		At	WepR
		ChildFx	:RobotLazerBurstRed.fx
	End

End

Condition
	On 	Time
	Time 	48

	Event
		EName 	Prime3
		Type	Start
		At	WepR
		
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

Condition
	On 	Time
	Time 	51

	Event
		EName 	Prime4
		Type	Start
		At	WepR
		
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

Condition
	On 	Time
	Time 	58

	Event
		ENAME   p1
		Type	Local 
		At	WepR
		ChildFx	:RobotLazerBurstRed.fx
	End

End

Condition
	On 	Time
	Time 	68


	Event
		ENAME   p1
		Type	Local 
		At	WepR
		ChildFx	:RobotLazerBurstRed.fx
	End

End

Condition
	On 	Time
	Time 	83


	Event
		ENAME   p1
		Type	Local 
		At	WepR
		ChildFx	:RobotLazerBurstRed.fx
	End

End

Condition
	On 	Time
	Time 	98


	Event
		ENAME   p1
		Type	Local 
		At	WepR
		ChildFx	:RobotLazerBurstRed.fx
	End

End


Condition
	On 	Time
	Time 	118


	Event
		ENAME   p1
		Type	Local 
		At	WepR
		ChildFx	:RobotLazerBurstRed.fx
	End

End

#Condition
#	On 	Time
#	Time 	140
#
#
#	Event
#		ENAME   p1
#		Type	Local 
#		At	WepR
#		ChildFx	:RobotLazerBurstRed.fx
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	153
#
#
#	Event
#		ENAME   p1
#		Type	Local 
#		At	WepR
#		ChildFx	:RobotLazerBurstRed.fx
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	160
#
#
#	Event
#		ENAME   p1
#		Type	Local 
#		At	WepR
#		ChildFx	:RobotLazerBurstRed.fx
#	End
#
#End

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

Condition
	On	Prime4Hit
	
	Event
		Ename	Prime4
		Type	Destroy
	End

End

End				