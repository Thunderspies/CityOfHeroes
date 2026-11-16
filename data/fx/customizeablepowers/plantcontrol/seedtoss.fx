#############################################################
## SeedToss.fx
#############################################################

FxInfo

LifeSpan  200

#############################################################

Condition
	On 	Time
	Time 	14

	Event
		EName 	Prime
		Type	start
		At	WepR
		bhvr	CustomizeablePowers\PlantControl\seedTossLobbprojectile.bhvr
		Part	CustomizeablePowers\PlantControl\EntanglePlantSeedDust.part
		Part	CustomizeablePowers\PlantControl\EntanglePlantSeeds_Dark.part
		Part	CustomizeablePowers\PlantControl\EntanglePlantSeeds_Black.part
		Magnet	T_Chest
		LookAt	T_Chest
		Sound mushroomtoss 60 60 0.7
	End

	Event
		Type	local
		At	Prime
		Part	CustomizeablePowers\PlantControl\EntanglePlantSeeds_Local.part
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
		Type	start
		At	T_Root
		Part	CustomizeablePowers\PlantControl\EntanglePlantSeeds_Dark.part
		Part	CustomizeablePowers\PlantControl\EntanglePlantSeeds_Black.part
		Part	CustomizeablePowers\PlantControl\SporeDustScatterLight.part
		LifeSpan  30
	End

	Event
		Type	local
		At	T_root
		Part	CustomizeablePowers\PlantControl\EntanglePlantSeeds_DarkFlat.part
		Part	CustomizeablePowers\PlantControl\EntanglePlantSeeds_BlackFlat.part
		LifeSpan 20
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