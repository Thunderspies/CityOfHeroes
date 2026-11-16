#############################################################
## Red_PowerSinkHit2.fx
#############################################################

FxInfo

Lifespan 200

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	HandGlow1
		Type	Local
		At	Chest
		Part	:Red_ElectricityGlow3.part
		Part	:Red_ElectricityGlowStar3.part
		Part	:Red_ElectricityArch4a.part
		Part	:Red_ElectricityArchSmalla.part
		Sound elecfollow3 70 70 0.8
		Lifespan 23
	End

	Event
		EName	continuingArchs
		Type	Local
		At	Chest
		Part	:Red_ElectricityArchContinuing.part
		Part	:Red_ElectricityArchContinuing2.part
		Sound suckup4 70 70 0.66
		Lifespan 120
	End
End

#############################################################

Condition
	On	Time
	Time	8.5

	Event
		EName 	Prime
		Type	Start
		At	chest
		Bhvr	behaviors\ConsumeProjectile.bhvr
		Part	:Red_ElectricityStar.part
		Part	:Red_ElectricityControl01a.part
		Part	:Red_ElectricityControl02a.part
		Part	:Red_ElectricityBallTaila.part
		Part	:Red_ElectricityBallTail2a.part
		Part	:Red_ElectricityBallTailWhite.part
		Part	:Red_ElectricityBallTail2aWhite.part
		Part	:Red_ElectricityArcha.part
		Part	:Red_ElectricityRinga.part
		Part	:Red_ElectricityArchInverta.part
		Magnet	T_Chest
		LookAt	T_Chest
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