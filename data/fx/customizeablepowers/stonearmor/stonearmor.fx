#############################################################
## StoneArmor.fx
#############################################################

FxInfo

Flags InheritAlpha DontSuppress InheritGeoScale

#############################################################

Condition
	On	Time
	Time 	0

	Event
		Type	Local
		At 	origin
		Sound Stone6 100 100 0.95
	End
End

Condition
	On	Time
	Time 	5

	Event
		Type	Local
		At 	origin
		Sound Stone_loop 100 100 0.3
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	FootL
		Bhvr	CustomizeablePowers\StoneArmor\StoneArmor.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	Tintable_StoneArmor_FootL02
	End

	Event
		Type	Local
		At	FootR
		Bhvr	CustomizeablePowers\StoneArmor\StoneArmor.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	Tintable_StoneArmor_FootR02
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
			TintGeom		1
		End
		Part	CustomizeablePowers\StoneArmor\StoneArmorRocks03.part
		Part	CustomizeablePowers\StoneArmor\StoneArmorDirt01.part
		Part	CustomizeablePowers\StoneArmor\StoneArmorDirt02.part
		geom	Tintable_StoneArmor_LLEGL02
	End

	Event
		Type	Local
		At	LLEGR
		Bhvr	CustomizeablePowers\StoneArmor\StoneArmor.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part	CustomizeablePowers\StoneArmor\StoneArmorRocks03.part
		Part	CustomizeablePowers\StoneArmor\StoneArmorDirt01.part
		Part	CustomizeablePowers\StoneArmor\StoneArmorDirt02.part
		geom	Tintable_StoneArmor_LLEGR02
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
			TintGeom		1
		End
		geom	Tintable_StoneArmor_ULEGR02
	End

	Event
		Type	Local
		At	ULEGL
		Bhvr	CustomizeablePowers\StoneArmor\StoneArmor.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	Tintable_StoneArmor_ULEGL02
	End
End

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	LARMR
		Bhvr	CustomizeablePowers\StoneArmor\StoneArmor.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	Tintable_StoneArmor_LARMR02
	End

	Event
		Type	Local
		At	LARML
		Bhvr	CustomizeablePowers\StoneArmor\StoneArmor.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	Tintable_StoneArmor_LARML02
	End
End

Condition
	On 	Time
	Time 	15


	Event
		Type	Local
		At	CHEST
		Bhvr	CustomizeablePowers\StoneArmor\StoneArmor.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	Tintable_StoneArmor_CHEST02
	End

	Event
		Type	Local
		At	UARML
		Bhvr	CustomizeablePowers\StoneArmor\StoneArmor.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part	CustomizeablePowers\StoneArmor\StoneArmorRocks.part
		Part	CustomizeablePowers\StoneArmor\StoneArmorDirt01.part
		Part	CustomizeablePowers\StoneArmor\StoneArmorDirt02.part
		geom	Tintable_StoneArmor_UARML02
	End

	Event
		Type	Local
		At	UARMR
		Bhvr	CustomizeablePowers\StoneArmor\StoneArmor.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part	CustomizeablePowers\StoneArmor\StoneArmorRocks.part
		Part	CustomizeablePowers\StoneArmor\StoneArmorDirt01.part
		Part	CustomizeablePowers\StoneArmor\StoneArmorDirt02.part
		geom	Tintable_StoneArmor_UARMR02
	End
End

#############################################################

Condition
	On 	death

	Event
		Ename	FinalRocks
		Type	start
		At	chest
		Part	CustomizeablePowers\StoneArmor\RocksScatter2.part
		Part	CustomizeablePowers\StoneArmor\RocksScatter.part
		Sound graniteexplo 70 70 0.66
		Lifespan 19
	End


	Event
		Ename	FinalDust
		Type	start
		At	chest
		Part	CustomizeablePowers\StoneArmor\StoneArmordirtExplosion.part
		Part	CustomizeablePowers\StoneArmor\StoneArmordirtExplosion1.part
		Lifespan 7
	End
End

#############################################################

End