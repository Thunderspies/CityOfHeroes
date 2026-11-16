#############################################################
## EntanglePlantSeedThrow.fx
#############################################################

FxInfo

LifeSpan	200

#############################################################

Condition
	On	Time
	Time	18

	Event
		Ename	P
		Type	start
		At	Root
		Sound graniteexplo 68 68 .7
	End

	Event

		Ename	Prime
		Type	start
		At	WepR
		BHVR	Behaviors\seedTossLobbprojectile.bhvr
		Part	:EntanglePlantSeeds_01.part
		Part	:EntanglePlantSeeds_Dark.part
		Part	:EntanglePlantSeeds_Black.part
		Part	:EntanglePlantSeedDust.part

		Magnet	t_root
		#LifeSpan	10
	End

	Event

		Type	local
		At	Prime
		Part	:EntanglePlantSeeds_Local.part
		Part	:EntanglePlantSeeds_DarkLocal.part
		Part	:EntanglePlantSeeds_BlackLocal.part
		Magnet	t_root
		#LifeSpan	10
	End

	Event
		Ename	Prime1
		Type	start
		At	root

		BHVR	Behaviors\seedTossprojectile.bhvr
		Part	:EntanglePlantSeeds_01Flata.part
		Part	:EntanglePlantSeeds_DarkFlata.part
		Part	:EntanglePlantSeeds_BlackFlata.part
		Magnet	t_root
		#LifeSpan	10
	End

End

Condition
	On 	PrimeHit

	Event
		Ename	vines
		Type	posit
		At	T_root
		Bhvr	:VineFade.bhvr
		Part	:VineDust.part
		Part	:VineDust2.part

		LifeSpan	23

	End

	Event

		Type	local
		At	T_root
		Part	:EntanglePlantSeeds_01Flat.part
		Part	:EntanglePlantSeeds_DarkFlat.part
		Part	:EntanglePlantSeeds_BlackFlat.part

		#LifeSpan	10
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


