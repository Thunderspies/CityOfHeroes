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
		Bhvr 	CustomizeablePowers\StoneArmor\Lava\LavaGolem.bhvr
		geom	Tintable_Lava_LGollemLegLeft
	End

	Event
		Type	Local
		At	LLEGL
		Bhvr 	CustomizeablePowers\StoneArmor\Lava\LavaGolem.bhvr
		geom	Tintable_Lava_LGollemLegLeft_AddGlow
	End

	Event
		Type	PositOnly
		At	FootL
		Part	:Smoldering_Fire2.part
		Part	:Smoldering_Fire1.part
		Part	:Smoldering_Smoke1.part
	End

#############################################################

	Event
		Type	Local
		At	LLEGR
		Bhvr 	CustomizeablePowers\StoneArmor\Lava\LavaGolem.bhvr
		geom	Tintable_Lava_LGollemLegRight
	End

	Event
		Type	Local
		At	LLEGR
		Bhvr 	CustomizeablePowers\StoneArmor\Lava\LavaGolem.bhvr
		geom	Tintable_Lava_LGollemLegRight_AddGlow
	End

	Event
		Type	PositOnly
		At	FootR
		Part	:Smoldering_Fire2.part
		Part	:Smoldering_Fire1.part
		Part	:Smoldering_Smoke1.part
	End

#############################################################

	Event
		Type	Local
		At	UlegR
		Bhvr 	CustomizeablePowers\StoneArmor\Lava\LavaGolem.bhvr
		geom	Tintable_Lava_UGollemLegRight
	End

	Event
		Type	Local
		At	UlegR
		Bhvr 	CustomizeablePowers\StoneArmor\Lava\LavaGolem.bhvr
		geom	Tintable_Lava_UGollemLegRight_AddGlow
	End

#############################################################

	Event
		Type	Local
		At	ULEGL
		Bhvr 	CustomizeablePowers\StoneArmor\Lava\LavaGolem.bhvr
		geom	Tintable_Lava_UGollemLegLeft
	End

	Event
		Type	Local
		At	ULEGL
		Bhvr 	CustomizeablePowers\StoneArmor\Lava\LavaGolem.bhvr
		geom	Tintable_Lava_UGollemLegLeft_AddGlow
	End

#############################################################

	Event
		Type	Local
		At	Hips
		Bhvr 	CustomizeablePowers\StoneArmor\Lava\LavaGolem.bhvr
		geom	Tintable_Lava_FX_GollemHips
	End

	Event
		Type	Local
		At	Hips
		Bhvr 	CustomizeablePowers\StoneArmor\Lava\LavaGolem.bhvr
		geom	Tintable_Lava_FX_GollemHips_AddGlow
	End

#############################################################

	Event
		Ename	Heada
		Type	Local
		At	CHEST
		Bhvr 	CustomizeablePowers\StoneArmor\Lava\LavaGolem.bhvr
		geom	Tintable_Lava_HeadChest
	End

	Event
		Ename	Heada
		Type	Local
		At	CHEST
		Bhvr 	CustomizeablePowers\StoneArmor\Lava\LavaGolem.bhvr
		geom	Tintable_Lava_HeadChest_AddGlow
	End

#############################################################

	Event
		Type	Local
		At	LARMR
		Bhvr 	CustomizeablePowers\StoneArmor\Lava\LavaGolem.bhvr
		geom	Tintable_Lava_LGollemArmRight
	End

	Event
		Type	Local
		At	LARMR
		Bhvr 	CustomizeablePowers\StoneArmor\Lava\LavaGolem.bhvr
		geom	Tintable_Lava_LGollemArmRight_AddGlow
	End

	Event
		Type	PositOnly
		At	UArmR
		Part	:Smoldering_Fire2.part
		Part	:Smoldering_Fire1.part
		Part	:Smoldering_Smoke1.part
	End

	Event
		Type	PositOnly
		At	HandR
		Part	:Smoldering_Fire2.part
		Part	:Smoldering_Fire1.part
		Part	:Smoldering_Smoke1.part
	End

#############################################################

	Event
		Type	Local
		At	LARML
		Bhvr 	CustomizeablePowers\StoneArmor\Lava\LavaGolem.bhvr
		geom	Tintable_Lava_LGolemArmLeft
	End

	Event
		Type	Local
		At	LARML
		Bhvr 	CustomizeablePowers\StoneArmor\Lava\LavaGolem.bhvr
		geom	Tintable_Lava_LGolemArmLeft_AddGlow
	End

	Event
		Type	PositOnly
		At	UArmL
		Part	:Smoldering_Fire2.part
		Part	:Smoldering_Fire1.part
		Part	:Smoldering_Smoke1.part
	End

	Event
		Type	PositOnly
		At	HandL
		Part	:Smoldering_Fire2.part
		Part	:Smoldering_Fire1.part
		Part	:Smoldering_Smoke1.part
	End

#############################################################

	Event
		Type	Local
		At	UARML
		Bhvr 	CustomizeablePowers\StoneArmor\Lava\LavaGolem.bhvr
		geom	Tintable_Lava_UGolemArmLeft
	End

	Event
		Type	Local
		At	UARML
		Bhvr 	CustomizeablePowers\StoneArmor\Lava\LavaGolem.bhvr
		geom	Tintable_Lava_UGolemArmLeft_AddGlow
	End

#############################################################

	Event
		Type	Local
		At	UARMR
		Bhvr 	CustomizeablePowers\StoneArmor\Lava\LavaGolem.bhvr
		geom	Tintable_Lava_UGolemArmRight
	End

	Event
		Type	Local
		At	UARMR
		Bhvr 	CustomizeablePowers\StoneArmor\Lava\LavaGolem.bhvr
		geom	Tintable_Lava_UGolemArmRight_AddGlow
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	eye1
		Type	Local
		At	Heada
		altpiv	1
		Part	:Pupil2.part
		Part	:elementalEyeGlowstars.part
		Part	:elementalEyeGlow.part
		Part	:EyeGas01.part
	End

	Event
		Ename	eye2
		Type	Local
		At	Heada
		altpiv	2
		Part	:Pupil2.part
		Part	:elementalEyeGlowstars.part
		Part	:elementalEyeGlow.part
		Part	:EyeGas01.part
	End
End

#############################################################

Condition
	On 	Death

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