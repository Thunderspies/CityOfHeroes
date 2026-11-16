#############################################################
## Gollem.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	LLEGL
		Bhvr 	behaviors\StoneArmorGollem.bhvr
		Part	Powers\EarthControl\GollemStoneArmorRocks.part
		Part	Powers\EarthControl\GollemStoneArmorDirt01.part
		Part	Powers\EarthControl\GollemStoneArmorDirt02.part
		geom	LGollemLegLeft
	End

	Event
		Type	Local
		At	LLEGR
		Bhvr 	behaviors\StoneArmorGollem.bhvr
		Part	Powers\EarthControl\GollemStoneArmorRocks.part
		Part	Powers\EarthControl\GollemStoneArmorDirt01.part
		Part	Powers\EarthControl\GollemStoneArmorDirt02.part
		geom	LGollemLegRight
	End

	Event
		Type	Local
		At	UlegR
		Bhvr 	behaviors\StoneArmorGollem.bhvr
		geom	UGollemLegRight
	End

	Event
		Type	Local
		At	ULEGL
		Bhvr 	behaviors\StoneArmorGollem.bhvr
		geom	UGollemLegLeft
	End

	Event
		Type	Local
		At	Hips
		Bhvr 	behaviors\StoneArmorGollem.bhvr
		geom	FX_GollemHips
	End

	Event
		Ename	Heada
		Type	Local
		At	CHEST
		Bhvr 	behaviors\StoneArmorGollem.bhvr
		geom	HeadChest
	End

	Event
		Ename	eye1
		Type	Local
		At	Heada
		altpiv	1
		Part	Powers\EarthControl\Pupil2.part
		Part	Powers\EarthControl\Eyes.part
	End

	Event
		Ename	eye2
		Type	Local
		At	Heada
		altpiv	2
		Part	Powers\EarthControl\Pupil2.part
		Part	Powers\EarthControl\Eyes.part
	End

	Event
		Type	Local
		At	LARMR
		Bhvr 	behaviors\StoneArmorGollem.bhvr
		geom	LGollemArmRight

	End

	Event
		Type	Local
		At	LARML
		Bhvr 	behaviors\StoneArmorGollem.bhvr
		geom	LGolemArmLeft
	End

	Event
		Type	Local
		At	UARML
		Bhvr 	behaviors\StoneArmorGollem.bhvr
		geom	UGolemArmLeft
		Part	Powers\EarthControl\GollemStoneArmorRocks.part
		Part	Powers\EarthControl\GollemStoneArmorDirt01.part
		Part	Powers\EarthControl\GollemStoneArmorDirt02.part
	End

	Event
		Type	Local
		At	UARMR
		Bhvr 	behaviors\StoneArmorGollem.bhvr
		geom	UGolemArmRight
		Part	Powers\EarthControl\GollemStoneArmorRocks.part
		Part	Powers\EarthControl\GollemStoneArmorDirt01.part
		Part	Powers\EarthControl\GollemStoneArmorDirt02.part
	End
End

#############################################################

Condition
	On 	death

	Event
		Type	local
		At	hips
		Part	Powers\EarthControl\RocksScatter2.part
		Part	Powers\EarthControl\RocksScatter.part
		Lifespan 19
	End


	Event
		Type	local
		At	hips
		Part	Powers\EarthControl\StoneArmordirtExplosion.part
		Part	Powers\EarthControl\StoneArmordirtExplosion1.part
		Lifespan 7
	End
End

#############################################################

End