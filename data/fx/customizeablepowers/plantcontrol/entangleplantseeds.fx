#############################################################
## EntanglePlantSeeds.fx
#############################################################

FxInfo

Lifespan 200

#############################################################

Condition
	On	Time
	Time	20

	Event
		Type	startpositOnly
		At	bendmystic
		Sound graniteexplo 88 88 0.77
	End

	Event
		Type	local
		At	bendmystic
		Part	CustomizeablePowers\PlantControl\PlantSeeds_Dark.part
		Part	CustomizeablePowers\PlantControl\PlantSeeds_Black.part
		Part	CustomizeablePowers\PlantControl\PlantSeedDust.part
		Lifespan 5
	End
End

#############################################################

End