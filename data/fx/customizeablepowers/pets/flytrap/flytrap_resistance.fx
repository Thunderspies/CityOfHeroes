#############################################################
## FlyTrap_Resistance.fx
#############################################################

FxInfo

Flags InheritAlpha InheritAnimScale

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	local
		At	Root
		Bhvr	CustomizeablePowers\PlantControl\VineFade.bhvr
		Part	CustomizeablePowers\PlantControl\VineDust.part
		Part	CustomizeablePowers\PlantControl\VineDust2.part
		Sound tree_emerge 70 70 0.7
		LifeSpan 50
	End

	Event
		Type	Local
		At	Hips
		part	CustomizeablePowers\PlantControl\Flytrap_01.part
	End

	Event
		Type	Local
		At	Chest
		Bhvr	V_COV\EnemyPowers\Plants\Flytrap\V_Flytrap_Flies.bhvr
		Part1	CustomizeablePowers\PlantControl\Flytrap_Flies.part
		Sound flies_loop 31 31 0.13
	End
End

#############################################################

Condition
	On	Time
	Time	33

	Event
		Type	Local
		At	Origin
		Sound creatureroar1 70 70 0.82
	End
End

#############################################################

End