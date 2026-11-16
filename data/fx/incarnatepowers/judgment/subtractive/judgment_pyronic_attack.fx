#############################################################
## Judgment_Pyronic_Attack.fx
#############################################################

FxInfo
LifeSpan 300

#############################################################


Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		ChildFX	IncarnatePowers\Incarnate_Gather.fx
		Lifespan	30
	End
End

Condition
	On 	Time
	Time 	18

	Event
		Type	Local
		At	Root
		Sound 	Incarnate_PyronicJudgement_Attack_01 100 100 .8
	End

	Event
		EName 	Prime
		Type	Start
		At	WepR
		Bhvr	behaviors\projectile1.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		150
			PhysForcePowerJitter	20
		End
		Part1   :judgment_pyronic_projectilemiddle.part
		Part2   :judgment_pyronic_projectile.part
		Magnet	Target
		LookAt	Target
		Lifespan 30
	End
End

#############################################################

Condition
	On	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
	End

	Event
		Type	Start
		At	T_Root
		ChildFX	:Judgment_Pyronic_Hit.fx
	End
End

#############################################################

End