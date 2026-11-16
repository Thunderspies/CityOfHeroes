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
		bhvr	:seedTossLobbprojectile.bhvr
		Part	ENEMYFX\Croatoa\Tuatha\EntanglePlantSeedDust.part
		Part	ENEMYFX\Croatoa\Tuatha\EntanglePlantSeeds_01.part
		Part	ENEMYFX\Croatoa\Tuatha\EntanglePlantSeeds_Dark.part
		Part	ENEMYFX\Croatoa\Tuatha\EntanglePlantSeeds_Black.part
		Magnet	T_Chest
		LookAt	T_Chest
		Sound mushroomtoss 60 60 .7
	End

	Event

		Type	local
		At	Prime
		Part	ENEMYFX\Croatoa\Tuatha\EntanglePlantSeeds_Local.part
		Part	ENEMYFX\Croatoa\Tuatha\EntanglePlantSeeds_DarkLocal.part
		Part	ENEMYFX\Croatoa\Tuatha\EntanglePlantSeeds_BlackLocal.part
		Magnet	t_root
		#LifeSpan	10
	End

	Event
		Ename	Prime1
		Type	start
		At	root

		BHVR	Behaviors/seedTossprojectile.bhvr
		Part	ENEMYFX\Croatoa\Tuatha\EntanglePlantSeeds_01Flata.part
		Part	ENEMYFX\Croatoa\Tuatha\EntanglePlantSeeds_DarkFlata.part
		Part	ENEMYFX\Croatoa\Tuatha\EntanglePlantSeeds_BlackFlata.part
		Magnet	t_root
		#LifeSpan	10
	End

End

##################################


Condition
	On 	PrimeHit

	Event
		Type	start
		At	T_Root
		Part	:EntanglePlantSeeds_01.part
		Part	:EntanglePlantSeeds_Dark.part
		Part	:EntanglePlantSeeds_Black.part
		Part	:SporeDustScatterLight.part
		LifeSpan  30
	End

	Event

		Type	local
		At	T_root
		Part	:EntanglePlantSeeds_01Flat.part
		Part	:EntanglePlantSeeds_DarkFlat.part
		Part	:EntanglePlantSeeds_BlackFlat.part

		LifeSpan	20
	End

	Event
		Ename 	Prime
		Type	Destroy
	End

End


Condition
	On 	Prime1Hit


	Event
		Ename 	Prime1
		Type	Destroy
	End

End

End