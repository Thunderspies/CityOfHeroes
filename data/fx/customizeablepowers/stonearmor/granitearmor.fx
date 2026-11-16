#############################################################
## GraniteArmor.fx
#############################################################

FxInfo

Flags IsArmor

#############################################################

condition
	On	Time
	Time	12

	Event
		Type	Local
		At	origin
		Sound stone_loop 60 60 0.26
	End
End

#############################################################

Condition
	On 	Time
	Time 	0
	Event
		Type	Local
		At	Root
		Bhvr	behaviors\fadeOut.bhvr
		BhvrOverride
			FadeOutLength		5
		End

		Flags	AdoptParentEntity
		Lifespan 10
	End
	Event
		Type	Local
		At	FootL
		Bhvr	CustomizeablePowers\StoneArmor\StoneArmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FX_StoneArmor_FootL
		Sound granite3 90 90 0.93
		LifeSpan 15
	End

	Event
		Type	Local
		At	FootR
		Bhvr	CustomizeablePowers\StoneArmor\StoneArmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FX_StoneArmor_FootR
		LifeSpan 15
	End
End

Condition
	On 	Time
	Time 	3

	Event
		Type	Local
		At	LLEGL
		Bhvr	CustomizeablePowers\StoneArmor\StoneArmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FX_StoneArmor_LLEGL
		Part	CustomizeablePowers\StoneArmor\StoneArmorRocks.part
		Part	CustomizeablePowers\StoneArmor\StoneArmorDirt01a.part
		Part	CustomizeablePowers\StoneArmor\StoneArmorDirt02a.part
		LifeSpan 15

	End

	Event
		Type	Local
		At	LLEGR
		Bhvr	CustomizeablePowers\StoneArmor\StoneArmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FX_StoneArmor_LLEGR
		Part	CustomizeablePowers\StoneArmor\StoneArmorRocks.part
		Part	CustomizeablePowers\StoneArmor\StoneArmorDirt01a.part
		Part	CustomizeablePowers\StoneArmor\StoneArmorDirt02a.part
		LifeSpan 15
	End
End

Condition
	On 	Time
	Time 	5

	Event
		Type	Local
		At	UlegR
		Bhvr	CustomizeablePowers\StoneArmor\StoneArmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FX_StoneArmor_ULEGR
		LifeSpan 15
	End

	Event
		Type	Local
		At	ULEGL
		Bhvr	CustomizeablePowers\StoneArmor\StoneArmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FX_StoneArmor_ULEGL
		LifeSpan 15
	End
End

Condition
	On 	Time
	Time 	8

	Event
		Type	Local
		At	LARMR
		Bhvr	CustomizeablePowers\StoneArmor\StoneArmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FX_StoneArmor_LARMR
		LifeSpan 7
	End

		Event
		Type	Local
		At	LARML
		Bhvr	CustomizeablePowers\StoneArmor\StoneArmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FX_StoneArmor_LARML
		LifeSpan 7
	End
End

Condition
	On 	Time
	Time 	8

	Event
		Type	Local
		At	CHEST
		Bhvr	CustomizeablePowers\StoneArmor\StoneArmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FX_StoneArmor_CHEST
		LifeSpan 7
	End

	Event
		Type	Local
		At	UARML
		Bhvr	CustomizeablePowers\StoneArmor\StoneArmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FX_StoneArmor_UARML
		Part	CustomizeablePowers\StoneArmor\StoneArmorRocks.part
		Part	CustomizeablePowers\StoneArmor\StoneArmorDirt01a.part
		Part	CustomizeablePowers\StoneArmor\StoneArmorDirt02a.part
		LifeSpan 3
	End

	Event
		Type	Local
		At	UARMR
		Bhvr	CustomizeablePowers\StoneArmor\StoneArmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FX_StoneArmor_UARMR
		Part	CustomizeablePowers\StoneArmor\StoneArmorRocks.part
		Part	CustomizeablePowers\StoneArmor\StoneArmorDirt01a.part
		Part	CustomizeablePowers\StoneArmor\StoneArmorDirt02a.part
		LifeSpan 3
	End
End

Condition
	On 	Time
	Time 	12

	Event
		ENAME	Heada
		Type	Local
		At	Head
		Bhvr	CustomizeablePowers\StoneArmor\StoneArmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FX_StoneArmor_HEAD
		LifeSpan 4
	End
End

#############################################################

Condition
	On	Time
	Time	5

	Event
		Ename	FinalDust
		Type	start
		At	chest
		Part3	CustomizeablePowers\StoneArmor\StoneArmordirtExplosionNew.part
		Part1	CustomizeablePowers\StoneArmor\StoneArmordirtExplosion1new.part
		Part3	CustomizeablePowers\StoneArmor\StoneArmordirtExplosionNew.part
		Part1	CustomizeablePowers\StoneArmor\StoneArmordirtExplosion1new.part
		Part3	CustomizeablePowers\StoneArmor\StoneArmordirtExplosionNew.part
		Part1	CustomizeablePowers\StoneArmor\StoneArmordirtExplosion1new.part
		LifeSpan 7
	End
End

Condition
	On	Time
	Time	10

	Event
		Ename	FinalRocks
		Type	start
		At	head
		Part1	CustomizeablePowers\StoneArmor\RocksScatter2.part
		Part2	CustomizeablePowers\StoneArmor\RocksScatter.part
		Part2	CustomizeablePowers\StoneArmor\RocksScatter.part
		LifeSpan 19
	End

	Event
		Ename	FinalDust
		Type	start
		At	head
		Part3	CustomizeablePowers\StoneArmor\StoneArmordirtExplosionNew.part
		Part1	CustomizeablePowers\StoneArmor\StoneArmordirtExplosion1new.part
		Part3	CustomizeablePowers\StoneArmor\StoneArmordirtExplosionNew.part
		Part1	CustomizeablePowers\StoneArmor\StoneArmordirtExplosion1new.part
		Part3	CustomizeablePowers\StoneArmor\StoneArmordirtExplosionNew.part
		Part1	CustomizeablePowers\StoneArmor\StoneArmordirtExplosion1new.part
		LifeSpan 7
	End
End

Condition
	On	Time
	Time	15

	Event
		Ename	FinalDust
		Type	start
		At	chest
		Part3	CustomizeablePowers\StoneArmor\StoneArmordirtExplosionNew.part
		Part1	CustomizeablePowers\StoneArmor\StoneArmordirtExplosion1new.part
		Part3	CustomizeablePowers\StoneArmor\StoneArmordirtExplosionNew.part
		Part1	CustomizeablePowers\StoneArmor\StoneArmordirtExplosion1new.part
		Part3	CustomizeablePowers\StoneArmor\StoneArmordirtExplosionNew.part
		Part1	CustomizeablePowers\StoneArmor\StoneArmordirtExplosion1new.part
		LifeSpan 7
	End
End

#############################################################

Condition
	On 	death

	Event
		Ename	FinalRocks
		Type	start
		At	chest
		Part1	CustomizeablePowers\StoneArmor\RocksScatter2.part
		Part2	CustomizeablePowers\StoneArmor\RocksScatter.part
		Part2	CustomizeablePowers\StoneArmor\RocksScatter.part
		Sound Graniteexplo 80 80 0.88
		LifeSpan 19
	End

	Event
		Ename	FinalDust
		Type	start
		At	chest
		Part3	CustomizeablePowers\StoneArmor\StoneArmordirtExplosionNew.part
		Part1	CustomizeablePowers\StoneArmor\StoneArmordirtExplosion1new.part
		Part3	CustomizeablePowers\StoneArmor\StoneArmordirtExplosionNew.part
		Part1	CustomizeablePowers\StoneArmor\StoneArmordirtExplosion1new.part
		Part3	CustomizeablePowers\StoneArmor\StoneArmordirtExplosionNew.part
		Part1	CustomizeablePowers\StoneArmor\StoneArmordirtExplosion1new.part
		LifeSpan 7
	End
End

#############################################################

End