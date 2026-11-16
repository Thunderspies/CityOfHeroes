#############################################################
## ForceBolt.fx
#############################################################

FxInfo

LifeSpan 90

#############################################################

Condition
	On 	Time
	Time 	30

	Event
		Type	Local
		At	LArmL
		Bhvr	ENEMYFX\V_PraetorianPolice\ForceGlove\ImpactBomb_Buildup01.bhvr
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ImpactBomb_Buildup_Glow01.part
		Geom	GEO_FormShield_Gauntlet_LArmL
		pOther	WepL
		Lifespan 17
	End

	Event
		Type	Local
		At	LArmR
		Bhvr	ENEMYFX\V_PraetorianPolice\ForceGlove\ImpactBomb_Buildup01.bhvr
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ImpactBomb_Buildup_Glow01.part
		Geom	GEO_FormShield_Gauntlet_LArmR
		pOther	WepR
		Lifespan 17
	End
End

Condition
	On	Time
	Time	10

	Event
		EName	EmitterFlare_Left
		Type	Local
		At	WepL
		Sound PPD_ForceBurst_Attack_01 200 200 1
		Bhvr	behaviors\GenericParticleFade.bhvr
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceGlove_Emitter_Flare01.part
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceGlove_Emitter_Flare02.part
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceGlove_Emitter_Glow01.part
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceGlove_Emitter_Ring01.part
		Lifespan 40
	End


	Event
		EName	EmitterFlare_Left
		Type	Local
		At	WepR
		Bhvr	behaviors\GenericParticleFade.bhvr
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceGlove_Emitter_Flare01.part
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceGlove_Emitter_Flare02.part
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceGlove_Emitter_Glow01.part
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceGlove_Emitter_Ring01.part
		Lifespan 40
	End
End

#############################################################

Condition
	On	Time
	Time	10

	Event
		EName	StartOffset
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset		0 4 3
		End
	End

	Event
		EName	Hookup
		Type	Local
		At	StartOffset
		BhvrOverride
			Scale		60 60 60
		End
		geom	FX_ConeOffsets
		LookAt  Target
	End

	Event
		EName	TargetOffset
		Type	Local
		At	Hookup
		AltPiv	1
	End
End

#############################################################

#Condition
#	On	Time
#	Time	54
#
#	Event
#		Type	Start
#		At	StartOffset
#		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Hit_Flash.part
#		Part	POWERS\ElectricityControl\ElectricityMeleeHitStar.part
#		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBurst_Hit_Star_Thin.part
#		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBurst_Hit_Ring.part
#		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBurst_Hit_Ring_Thin.part
#		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBurst_Hit_Ring_Large.part
#		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Hit_Glow.part
#		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Hit_Glow_Large.part
#		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBurst_Hit_Glow_Large.part
#		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Hit_Balls_Large.part
#		LifeSpan 9
#	End
#End

#############################################################

Condition
	On	Time
	Time	55

	Event
		Ename	Prime
		Type	Start
		At	StartOffset
		bhvr	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBurst_Projectile01.bhvr
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBurst_Trail01.part
		Geom	ForceBolt_Greyscale
		Magnet	TargetOffset
		LookAt  TargetOffset
		pOther	TargetOffsetHookup
		Lifespan 5
	End

	Event
		Ename	Prime2
		Type	Local
		At	Prime
		BhvrOverride
			Scale			0.85 0.85 0.85
			StartColor		16 96 255
			PositionOffset		0 0 -4
			Alpha			192
		End
		Geom	ForceBolt_Greyscale
		Lifespan 5
	End

	Event
		Ename	Prime3
		Type	Local
		At	Prime
		BhvrOverride
			Scale			0.6 0.6 0.6
			StartColor		16 96 255
			PositionOffset		0 0 -8
			Alpha			128
		End
		Geom	ForceBolt_Greyscale
		Lifespan 5
	End

	Event
		Ename	PushForce
		Type	Local
		At	Prime
		BhvrOverride
			pyrRotate		10 0 0
			PhysForceType		Forward
			PhysForceRadius		20
			PhysForcePower		400
			PhysForcePowerJitter	100
		End
	End
End

#############################################################

Condition
	On	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
	End
End

#############################################################

End