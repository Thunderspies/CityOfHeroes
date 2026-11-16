#############################################################
## EntanglePlantSeedThrow.fx
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On	Time
	Time	18

	Event
		Type	start
		At	Root
		Sound graniteexplo 68 68 0.7
	End

	Event
		Ename	Prime
		Type	start
		At	WepR
		BHVR	CustomizeablePowers\PlantControl\seedTossLobbprojectile.bhvr
		Part	CustomizeablePowers\PlantControl\EntanglePlantSeeds_Dark.part
		Part	CustomizeablePowers\PlantControl\EntanglePlantSeeds_Black.part
		Part	CustomizeablePowers\PlantControl\EntanglePlantSeedDust.part
		Part	CustomizeablePowers\PlantControl\EntanglePlantSeeds_DarkLocal.part
		Part	CustomizeablePowers\PlantControl\EntanglePlantSeeds_BlackLocal.part
		Magnet	T_Root
	End

	Event
		Ename	Prime1
		Type	start
		At	root
		BHVR	CustomizeablePowers\PlantControl\seedTossprojectile.bhvr
		Part	CustomizeablePowers\PlantControl\EntanglePlantSeeds_DarkFlata.part
		Part	CustomizeablePowers\PlantControl\EntanglePlantSeeds_BlackFlata.part
		Magnet	T_Root
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		Ename	vines
		Type	posit
		At	T_root
		Bhvr	CustomizeablePowers\PlantControl\VineFade.bhvr
		Part	CustomizeablePowers\PlantControl\VineDust.part
		Part	CustomizeablePowers\PlantControl\VineDust2.part
		LifeSpan 23
	End

	Event

		Type	local
		At	T_root
		Part	CustomizeablePowers\PlantControl\EntanglePlantSeeds_DarkFlat.part
		Part	CustomizeablePowers\PlantControl\EntanglePlantSeeds_BlackFlat.part
	End

	Event
		Ename 	Prime
		Type	Destroy
	End
End

#############################################################

Condition
	On 	Prime1Hit

	Event
		Ename 	Prime1
		Type	Destroy
	End
End

#############################################################

End