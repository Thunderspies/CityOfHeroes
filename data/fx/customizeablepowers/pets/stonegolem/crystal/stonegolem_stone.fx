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
		At	Hips
		Bhvr 	behaviors\StoneArmorGollem.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_Crystal_FX_GollemHips
	End

	Event
		Type	Local
		At	UlegR
		Bhvr 	behaviors\StoneArmorGollem.bhvr
		BhvrOverride
			TintGeom	1
		End
		part	:BodySparkle01.part
		geom	Tintable_Crystal_UGollemLegRight
		pOther	LLegR
	End

	Event
		Type	Local
		At	ULEGL
		Bhvr 	behaviors\StoneArmorGollem.bhvr
		BhvrOverride
			TintGeom	1
		End
		part	:BodySparkle01.part
		geom	Tintable_Crystal_UGollemLegLeft
		pOther	LLegR
	End

	Event
		Type	Local
		At	LLEGL
		Bhvr 	behaviors\StoneArmorGollem.bhvr
		BhvrOverride
			TintGeom	1
		End
		part	:BodySparkle01.part
		geom	Tintable_Crystal_LGollemLegLeft
		pOther	ToeL
	End

	Event
		Type	Local
		At	LLEGR
		Bhvr 	behaviors\StoneArmorGollem.bhvr
		BhvrOverride
			TintGeom	1
		End
		part	:BodySparkle01.part
		geom	Tintable_Crystal_LGollemLegRight
		pOther	ToeR
	End

	Event
		Type	Local
		At	UARML
		Bhvr 	behaviors\StoneArmorGollem.bhvr
		BhvrOverride
			TintGeom	1
		End
		part	:BodySparkle01.part
		geom	Tintable_Crystal_UGolemArmLeft
		pOther	LArmL
	End

	Event
		Type	Local
		At	UARMR
		Bhvr 	behaviors\StoneArmorGollem.bhvr
		BhvrOverride
			TintGeom	1
		End
		part	:BodySparkle01.part
		geom	Tintable_Crystal_UGolemArmRight
		pOther	LArmR
	End

	Event
		Type	Local
		At	LARMR
		Bhvr 	behaviors\StoneArmorGollem.bhvr
		BhvrOverride
			TintGeom	1
		End
		part	:BodySparkle01.part
		geom	Tintable_Crystal_LGollemArmRight
		pOther	HandR
	End

	Event
		Type	Local
		At	LARML
		Bhvr 	behaviors\StoneArmorGollem.bhvr
		BhvrOverride
			TintGeom	1
		End
		part	:BodySparkle01.part
		geom	Tintable_Crystal_LGolemArmLeft
		pOther	HandL
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Ename	Heada
		Type	Local
		At	CHEST
		Bhvr 	behaviors\StoneArmorGollem.bhvr
		BhvrOverride
			TintGeom	1
		End
		part	:BodySparkle02.part
		part	:LightRays1.part
		geom	Tintable_Crystal_HeadChest
		pMagnet	Chest
	End

	Event
		Ename	eye1
		Type	Local
		At	Heada
		altpiv	1
		Part	:Pupil2.part
		Part	:Eye_Glow.part
		Part	:Eye_Sparkle1.part
	End

	Event
		Ename	eye2
		Type	Local
		At	Heada
		altpiv	2
		Part	:Pupil2.part
		Part	:Eye_Glow.part
		Part	:Eye_Sparkle1.part
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