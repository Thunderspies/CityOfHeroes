#############################################################
## Rooted.fx
#############################################################

FxInfo

Flags	InheritAlpha

#############################################################

Condition
	On	Time
	Time 	0

	Event
		Type	Local
		At 	origin
		Sound Stone6 100 100 0.88
	End
End

Condition
	On	Time
	Time 	5

	Event
		Type	Local
		At 	origin
		Sound Stone_loop 100 100 0.5
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	FootL
		Bhvr	CustomizeablePowers\StoneArmor\RootedFoot.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	Tintable_Rooted_Lava_FootL
	End

	Event
		Type	Local
		At	FootL
		Bhvr	CustomizeablePowers\StoneArmor\RootedFoot.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	Tintable_Rooted_Lava_FootL_AddGlow
	End

	Event
		Type	Local
		At	FootR
		Bhvr	CustomizeablePowers\StoneArmor\RootedFootR.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	Tintable_Rooted_Lava_FootR
	End

	Event
		Type	Local
		At	FootR
		Bhvr	CustomizeablePowers\StoneArmor\RootedFootR.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	Tintable_Rooted_Lava_FootR_AddGlow
	End

	Event
		Type	Local
		At	LLEGL
		Bhvr	CustomizeablePowers\StoneArmor\RootedOpposite.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part	CustomizeablePowers\StoneArmor\StoneArmorRocks.part
		Part	CustomizeablePowers\StoneArmor\StoneArmorDirt01.part
		Part	CustomizeablePowers\StoneArmor\StoneArmorDirt02.part
		Geom	Tintable_Rooted_Lava_LLegR
	End

	Event
		Type	Local
		At	LLEGL
		Bhvr	CustomizeablePowers\StoneArmor\RootedOpposite.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_Rooted_Lava_LLegR_AddGlow
	End

	Event
		Type	Local
		At	LLEGR
		Bhvr	CustomizeablePowers\StoneArmor\RootedOppositeL.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part	CustomizeablePowers\StoneArmor\StoneArmorRocks.part
		Part	CustomizeablePowers\StoneArmor\StoneArmorDirt01.part
		Part	CustomizeablePowers\StoneArmor\StoneArmorDirt02.part
		Geom	Tintable_Rooted_Lava_LLegL
	End

	Event
		Type	Local
		At	LLEGR
		Bhvr	CustomizeablePowers\StoneArmor\RootedOppositeL.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_Rooted_Lava_LLegL_AddGlow
	End
End

###################################################################

Condition
	On 	death

	Event
		Ename	FinalRocks
		Type	start
		At	LLEGL
		Part	CustomizeablePowers\StoneArmor\RocksScatter.part
		Sound graniteexplo 70 70 0.66
		Lifespan 19
	End

	Event
		Ename	FinalDust
		Type	start
		At	LLEGL
		Part	CustomizeablePowers\StoneArmor\StoneArmordirtExplosion.part
		Part	CustomizeablePowers\StoneArmor\StoneArmordirtExplosion1.part
		Lifespan 7
	End

	Event
		Ename	FinalRocks
		Type	start
		At	LLEGR
		Part	CustomizeablePowers\StoneArmor\RocksScatter.part
		Lifespan 19
	End

	Event
		Ename	FinalDust
		Type	start
		At	LLEGR
		Part	CustomizeablePowers\StoneArmor\StoneArmordirtExplosion.part
		Part	CustomizeablePowers\StoneArmor\StoneArmordirtExplosion1.part
		Lifespan 7
	End
End

#############################################################

End