#############################################################
## MoltenStoneArmor.fx
#############################################################

FxInfo

Flags InheritAlpha

#############################################################

Condition
	On	Time
	Time 	0

	Event
		Type	Local
		At 	origin
		Sound MoltenStone 100 100 0.96
	End
End

Condition
	On	Time
	Time 	5

	Event
		Type	Local
		At 	origin
		Sound Molten_loop 100 100 0.55
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	FootLa
		Type	Local
		At	FootL
		Bhvr 	behaviors\StoneArmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_Brimstone_Crystal_FootL
	End

	Event
		Ename	FootRa
		Type	local
		At	FootR
		Bhvr 	behaviors\StoneArmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_Brimstone_Crystal_FootR
	End
End

Condition
	On 	Time
	Time 	3

	Event
		Type	Local
		At	LLEGL
		Bhvr 	behaviors\StoneArmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_Brimstone_Crystal_LLegL
	End

	Event
		Type	Local
		At	LLEGR
		Bhvr 	behaviors\StoneArmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_Brimstone_Crystal_LLegR
	End
End

Condition
	On 	Time
	Time 	5

	Event
		Type	Local
		At	ULEGL
		Bhvr 	behaviors\StoneArmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_Brimstone_Crystal_ULegL
	End

	Event
		Type	Local
		At	UlegR
		Bhvr 	behaviors\StoneArmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_Brimstone_Crystal_ULegR
	End
End

Condition
	On 	Time
	Time 	8

	Event
		Type	Local
		At	LARML
		Bhvr 	behaviors\StoneArmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_Brimstone_Crystal_LArmL
	End

	Event
		Type	Local
		At	LARMR
		Bhvr 	behaviors\StoneArmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_Brimstone_Crystal_LArmR
	End
End

Condition
	On 	Time
	Time 	11

	Event
		Type	Local
		At	CHEST
		Bhvr 	behaviors\StoneArmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_Brimstone_Crystal_Chest
	End

	Event
		Type	Local
		At	UARML
		Bhvr 	behaviors\StoneArmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_Brimstone_Crystal_UArmL
	End

	Event
		Type	Local
		At	UARMR
		Bhvr 	behaviors\StoneArmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_Brimstone_Crystal_UArmR
	End
End

#############################################################

Condition
	On 	Time
	Time 	7

	Event
		Ename	firefeet
		Type	Local
		At	FootL
		Part	CustomizeablePowers\StoneArmor\footFlame.part
		Part	CustomizeablePowers\StoneArmor\footspark.part
	End

	Event
		Ename	firefeet2
		Type	Local
		At	FootR
		Bhvr	CustomizeablePowers\StoneArmor\OffsetLavaFoot.bhvr
		Part	CustomizeablePowers\StoneArmor\footFlame.part
		Part	CustomizeablePowers\StoneArmor\footspark.part
	End
End

Condition
	On 	Time
	Time 	5

	Event
		Type	Local
		At	UARML
		Part	CustomizeablePowers\StoneArmor\LavaFlamesShoulders.part
		Part	CustomizeablePowers\StoneArmor\LavaFlamesShoulders1.part
	End

	Event
		Type	Local
		At	UARMR
		Part	CustomizeablePowers\StoneArmor\LavaFlamesShoulders.part
		Part	CustomizeablePowers\StoneArmor\LavaFlamesShoulders1.part
	End
End

Condition
	On 	Time
	Time 	23

	Event
		Type	Local
		At	LARML
		Part	CustomizeablePowers\StoneArmor\LavaSteam.part
	End

	Event
		Type	Local
		At	LARMR
		Part	CustomizeablePowers\StoneArmor\LavaSteam.part
	End

	Event
		Type	Local
		At	LlegL
		Part	CustomizeablePowers\StoneArmor\LavaSteam.part
	End

	Event
		Type	Local
		At	LlegR
		Part	CustomizeablePowers\StoneArmor\LavaSteam.part
	End

	Event
		Type	Local
		At	Head
		Part	CustomizeablePowers\StoneArmor\LavaSteamTrails.part
	End

	Event
		Type	Local
		At	Hips
		Part	CustomizeablePowers\StoneArmor\LavaSteamTrails.part
	End

	Event
		Type	Local
		At	UlegL
		Part	CustomizeablePowers\StoneArmor\LavaFlamesHips.part
		Part	CustomizeablePowers\StoneArmor\LavaFlameHips1.part
	End

	Event
		Type	Local
		At	UlegR
		Part	CustomizeablePowers\StoneArmor\LavaFlamesHips.part
		Part	CustomizeablePowers\StoneArmor\LavaFlameHips1.part
	End
End

#############################################################

Condition
	On 	Cycle
	Time 	20
	Chance	0.13

	Event
		Type	Local
		At	UARMR
		Part	CustomizeablePowers\StoneArmor\LavaSpray.part
		Lifespan 20
	End
End

Condition
	On 	Cycle
	Time 	20
	Chance	0.13

	Event
		Type	Local
		At	UARML
		Part	CustomizeablePowers\StoneArmor\LavaSpray.part
		Lifespan 20
	End
End

Condition
	On 	Cycle
	Time 	15
	Chance	0.4

	Event
		Type	Local
		At	Hips
		Part	CustomizeablePowers\StoneArmor\LavaSpray2.part
		Lifespan 20
	End
End

#############################################################

Condition
	On 	death

	Event
		Type	start
		At	hips
		Part	CustomizeablePowers\StoneArmor\StoneArmordirtExplosion2.part
		Part	CustomizeablePowers\StoneArmor\StoneArmordirtExplosionFlame.part
		Lifespan 17
	End

	Event
		Type	start
		At	hips
		Part	CustomizeablePowers\StoneArmor\StoneArmorSparks.part
		Lifespan 17
	End

	Event
		Type	start
		At	Root
		Part	CustomizeablePowers\StoneArmor\StoneArmorSparks.part
		Lifespan 17
	End
End

#############################################################

End