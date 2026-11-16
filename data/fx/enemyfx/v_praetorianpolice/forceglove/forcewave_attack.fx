#############################################################
## ForceBolt.fx
#############################################################

FxInfo

LifeSpan 60

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	LArmL
		Sound PPD_ForceWave_Attack_01 200 200 1
		Bhvr	ENEMYFX\V_PraetorianPolice\ForceGlove\ImpactBomb_Buildup01.bhvr
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ImpactBomb_Buildup_Glow01.part
		Geom	GEO_FormShield_Gauntlet_LArmL
		pOther	WepL
		Lifespan 15
	End

	Event
		Type	Local
		At	LArmR
		Bhvr	ENEMYFX\V_PraetorianPolice\ForceGlove\ImpactBomb_Buildup01.bhvr
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ImpactBomb_Buildup_Glow01.part
		Geom	GEO_FormShield_Gauntlet_LArmR
		pOther	WepR
		Lifespan 15
	End
End

Condition
	On	Time
	Time	10

	Event
		EName	EmitterFlare_Left
		Type	Local
		At	WepL
		Bhvr	behaviors\GenericParticleFade.bhvr
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceGlove_Emitter_Flare01.part
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceGlove_Emitter_Flare02.part
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceGlove_Emitter_Glow01.part
		part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceGlove_Emitter_Ring01.part
		Lifespan 20
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
		Lifespan 20
	End
End

#############################################################

Condition
	On	Time
	Time	10

	Event
		EName	TargetOffsetHookup
		Type	Posit
		At	Chest
		BhvrOverride
			Scale		15 15 15
		End
		geom	FX_ConeOffsets
		LookAt  Target
	End

	Event
		EName	TargetOffset
		Type	Local
		At	TargetOffsetHookup
		AltPiv	1
	End
End

#############################################################

Condition
	On	Time
	Time	20

	Event
		EName	Prime
		Type	Start
		At	TargetOffsetHookup
		bhvr	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceWave_Projectile01.bhvr
		Geom	ForceBolt_Greyscale
		Magnet	TargetOffset
		LookAt  TargetOffset
		Lifespan 2
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

Condition
	On	Time
	Time	21

	Event
		EName	Prime
		Type	Start
		At	TargetOffsetHookup
		bhvr	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceWave_Projectile01.bhvr
		Geom	ForceBolt_Greyscale
		Magnet	TargetOffset
		LookAt  TargetOffset
		Lifespan 2
	End
End

Condition
	On	Time
	Time	22

	Event
		EName	Prime
		Type	Start
		At	TargetOffsetHookup
		bhvr	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceWave_Projectile01.bhvr
		Geom	ForceBolt_Greyscale
		Magnet	TargetOffset
		LookAt  TargetOffset
		Lifespan 2
	End
End

#############################################################

End