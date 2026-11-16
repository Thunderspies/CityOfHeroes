#############################################################
## Seed_Indicator.fx
#############################################################

FxInfo
#############################################################

Condition
	On 	Time
	Time 	0
	Event
		Ename	Purple
		Type	PositOnly
		At	Chest
		ChildFX	:Seed_Indicator_Purple.fx
	End

	Event
		Ename	Yellow
		Type	PositOnly
		At	Chest
		ChildFX	:Seed_Indicator_Yellow.fx
	End

	Event
		Ename	Green
		Type	PositOnly
		At	Chest
		ChildFX	:Seed_Indicator_Green.fx
	End

	Event
		Ename	Blue
		Type	PositOnly
		At	Chest
		ChildFX	:Seed_Indicator_Blue.fx
	End

	Event
		Ename	red
		Type	PositOnly
		At	Chest
		ChildFX	:Seed_Indicator_Red.fx
	End
End

#############################################################
End