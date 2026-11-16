#########################################################
##	Template

FxInfo

Lifespan	200

Condition
	On 	Time
	Time 	0	

	Event
		EName	Chest
		Type	Local
		At	Chest
		part1	:Splash_Heal_02.part
		part1	:Splash_Heal_01.part
		Lifespan	3

	End

	Event
		EName	Chest
		Type	Local
		At	Chest
		part1	:Inward_Heal_01.part
		Sound coolanthit 90 90 1.0
		Lifespan	15
	End


End

Condition
	On	Time
	Time	0
	
	Event
		EName	Ring
		Type	Local
		At	Chest
		part1	:HealRingUp_01.part
		Bhvr	:OffsetBottom.bhvr
	End
End

Condition
	On	Time
	Time	0
	
	Event
		EName	Ring
		Type	Local
		At	Chest
		part1	:HealRingDown_01.part
		Bhvr	:OffsetTop.bhvr
	End
End

Condition
	On	Time
	Time	5
	
	Event
		EName	Ring
		Type	Local
		At	Chest
		part1	:HealRingUp_01.part
		Bhvr	:OffsetBottom.bhvr
	End
End

Condition
	On	Time
	Time	5
	
	Event
		EName	Ring
		Type	Local
		At	Chest
		part1	:HealRingDown_01.part
		Bhvr	:OffsetTop.bhvr
	End
End

Condition
	On	Time
	Time	10
	
	Event
		EName	Ring
		Type	Local
		At	Chest
		part1	:HealRingUp_01.part
		Bhvr	:OffsetBottom.bhvr
	End
End

Condition
	On	Time
	Time	10
	
	Event
		EName	Ring
		Type	Local
		At	Chest
		part1	:HealRingDown_01.part
		Bhvr	:OffsetTop.bhvr
	End
End

Condition
	On	Time
	Time	15
	
	Event
		EName	Ring
		Type	Local
		At	Chest
		part1	:HealRingUp_01.part
		Bhvr	:OffsetBottom.bhvr
	End
End

Condition
	On	Time
	Time	15
	
	Event
		EName	Ring
		Type	Local
		At	Chest
		part1	:HealRingDown_01.part
		Bhvr	:OffsetTop.bhvr
	End
End