#############################################################
## LargeLazerRifleBurst.fx
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On 	Time
	Time 	7

	Event
		ENAME   p1
		Type	Posit
		At	T_Chest
		Geom	Centerdummy
		Lookat	origin
	End

	Event
		ENAME   p1offset
		Type	local
		At	p1
		altpiv	1
		bhvr	:LazerOffset.bhvr
	End

	Event
		ENAME   p1a
		Type	Local
		At	WepR
		Geom	newrobotics
	End

	Event
		Type	Local
		At	p1a
		Altpiv	2
		Part	:BlastOriginStar.part
		Part	:BlastOriginEnergy.part
		Part	:BlastOriginEnergy.part
		Sound laserRifleBurst 80 80 0.88
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
		At	p1a
		Altpiv	2
		BHVR	:VillianSniperlTracer.bhvr
		Part	:LargeVillianSniper_Head.part
		Part	:LargeVillianSniper_HeadCore.part
		Part	:VillianSniper_Head.part
		Part	:VillianSniper_HeadCore.part
		Magnet	p1
		Lookat	p1
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