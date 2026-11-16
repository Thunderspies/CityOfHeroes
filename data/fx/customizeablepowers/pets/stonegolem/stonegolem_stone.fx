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
		BhvrOverride
			TintGeom	1
		End
		Part	CustomizeablePowers\Pets\StoneGolem\GollemStoneArmorRocks.part
		Part	CustomizeablePowers\Pets\StoneGolem\GollemStoneArmorDirt01.part
		Part	CustomizeablePowers\Pets\StoneGolem\GollemStoneArmorDirt02.part
		geom	Tintable_LGollemLegLeft
	End

	Event
		Type	Local
		At	LLEGR
		Bhvr 	behaviors\StoneArmorGollem.bhvr
		BhvrOverride
			TintGeom	1
		End
		Part	CustomizeablePowers\Pets\StoneGolem\GollemStoneArmorRocks.part
		Part	CustomizeablePowers\Pets\StoneGolem\GollemStoneArmorDirt01.part
		Part	CustomizeablePowers\Pets\StoneGolem\GollemStoneArmorDirt02.part
		geom	Tintable_LGollemLegRight
	End

	Event
		Type	Local
		At	UlegR
		Bhvr 	behaviors\StoneArmorGollem.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_UGollemLegRight
	End

	Event
		Type	Local
		At	ULEGL
		Bhvr 	behaviors\StoneArmorGollem.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_UGollemLegLeft
	End

	Event
		Type	Local
		At	Hips
		Bhvr 	behaviors\StoneArmorGollem.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FX_GollemHips
	End

	Event
		Ename	Heada
		Type	Local
		At	CHEST
		Bhvr 	behaviors\StoneArmorGollem.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_HeadChest
	End

	Event
		Ename	eye1
		Type	Local
		At	Heada
		altpiv	1
		Part	Powers\EarthControl\Pupil2.part
		Part	CustomizeablePowers\Pets\StoneGolem\Eyes.part
	End

	Event
		Ename	eye2
		Type	Local
		At	Heada
		altpiv	2
		Part	Powers\EarthControl\Pupil2.part
		Part	CustomizeablePowers\Pets\StoneGolem\Eyes.part
	End

	Event
		Type	Local
		At	LARMR
		Bhvr 	behaviors\StoneArmorGollem.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_LGollemArmRight

	End

	Event
		Type	Local
		At	LARML
		Bhvr 	behaviors\StoneArmorGollem.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_LGolemArmLeft
	End

	Event
		Type	Local
		At	UARML
		Bhvr 	behaviors\StoneArmorGollem.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_UGolemArmLeft
		Part	CustomizeablePowers\Pets\StoneGolem\GollemStoneArmorRocks.part
		Part	CustomizeablePowers\Pets\StoneGolem\GollemStoneArmorDirt01.part
		Part	CustomizeablePowers\Pets\StoneGolem\GollemStoneArmorDirt02.part
	End

	Event
		Type	Local
		At	UARMR
		Bhvr 	behaviors\StoneArmorGollem.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_UGolemArmRight
		Part	CustomizeablePowers\Pets\StoneGolem\GollemStoneArmorRocks.part
		Part	CustomizeablePowers\Pets\StoneGolem\GollemStoneArmorDirt01.part
		Part	CustomizeablePowers\Pets\StoneGolem\GollemStoneArmorDirt02.part
	End
End

#############################################################

Condition
	On 	death

	Event
		Type	local
		At	hips
		Part	CustomizeablePowers\Pets\StoneGolem\RocksScatter2.part
		Part	CustomizeablePowers\Pets\StoneGolem\RocksScatter.part
		Lifespan 19
	End


	Event
		Type	local
		At	hips
		Part	CustomizeablePowers\Pets\StoneGolem\StoneArmordirtExplosion.part
		Part	CustomizeablePowers\Pets\StoneGolem\StoneArmordirtExplosion1.part
		Lifespan 7
	End
End

#############################################################

End