#############################################################
## RobotLazerBursts.fx
#############################################################

FxInfo

LifeSpan  400

#############################################################

Condition
	On 	Time
	Time 	7

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
		Altpiv	2
		Part	:BlastOriginStar.part
		Part	:BlastOriginEnergy.part
		Part	:BlastOriginEnergy.part
		Sound pulserifle2 80 80 .85
		LifeSpan 5
	End
End

#############################################################

Condition
	On 	Time
	Time 	7

	Event
		EName 	Prime
		Type	Start
		At	p1
		Altpiv	2
		BHVR	:VillianSniperlTracer.bhvr
		Part	:VillianSniper_Head.part
		Part	:VillianSniper_HeadCore.part
		Magnet	Target
		Lookat	Target
	End
End

#############################################################

Condition
	On	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
	End
End

#############################################################

End