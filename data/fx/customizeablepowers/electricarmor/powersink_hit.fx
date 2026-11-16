#############################################################
## PowerSink_Hit.fx
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
		Type	Local
		At	Chest
		Part	:ElectricityGlow3.part
		Part	:ElectricityGlowStar3.part
		Part	:ElectricityArch4a.part
		Part	:ElectricityArchSmalla.part
		Sound elecfollow3 70 70 0.8
		Lifespan 23
	End

	Event
		Type	Local
		At	Chest
		Part	:ElectricityArchContinuing.part
		Part	:ElectricityArchContinuing2.part
		Sound suckup4 70 70 0.66
		Lifespan 120
	End
End

## REVERSE PROJECTILE ###########################################################

Condition
	On	Time
	Time	9

	Event
		EName 	Prime
		Type	Start
		At	chest
		Bhvr	behaviors\ConsumeProjectile.bhvr
		Part	:ElectricityStar.part
		Part	:ElectricityControl01a.part
		Part	:ElectricityControl02a.part
		Part	:ElectricityBallTaila.part
		Part	:ElectricityBallTail2a.part
		Part	:ElectricityBallTailWhite.part
		Part	:ElectricityBallTail2aWhite.part
		Part	:ElectricityRinga.part
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