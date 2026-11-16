#############################################################
## ForceBolt.fx
#############################################################

FxInfo

LifeSpan 150

#############################################################

Condition
	On	Time
	Time	0

	Event
		Ename	PushDirection
		Type	Posit
		At	Root
		Lookat	Target
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	LArmL
		Sound PPD_ImpactBomb_Attack_01 200 200 1
		Bhvr	ENEMYFX\V_PraetorianPolice\ForceGlove\ImpactBomb_Buildup01.bhvr
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ImpactBomb_Buildup_Glow01.part
		Geom	GEO_FormShield_Gauntlet_LArmL
		pOther	WepL
		Lifespan 65
	End

	Event
		Type	Local
		At	LArmR
		Bhvr	ENEMYFX\V_PraetorianPolice\ForceGlove\ImpactBomb_Buildup01.bhvr
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ImpactBomb_Buildup_Glow01.part
		Geom	GEO_FormShield_Gauntlet_LArmR
		pOther	WepR
		Lifespan 65
	End
End

#############################################################

Condition
	On	Time
	Time	68

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
	On 	Time
	Time 	72

	Event
		EName 	Prime
		Type	Start
		At	WepL
		Bhvr	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolt_Projectile01.bhvr
		Geom	ForceBolt_Greyscale
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceGlove_Emitter_Blast01.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Glow01.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Mist01.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Trail01.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Trail02.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Trail03.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Rings01.part
		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	Prime2
		Type	Start
		At	WepR
		Bhvr	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolt_Projectile01.bhvr
		Geom	ForceBolt_Greyscale
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceGlove_Emitter_Blast01.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Glow01.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Mist01.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Trail01.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Trail02.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Trail03.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Rings01.part
		Magnet	Target
		LookAt	Target
	End

	Event
		Ename	PushForce
		Type	Local
		At	PushDirection
		BhvrOverride
			PositionOffset		0 0 5
			pyrRotate		15 0 0
			PhysForceType		Forward
			PhysForceRadius		10
			PhysForcePower		250
			PhysForcePowerJitter	100
		End
		LifeSpan 1
	End
End

#############################################################


Condition
	On 	PrimeHit

#	Event
#		Type	PositOnly
#		At	Chest
#		bhvr	:ForceFieldBomb.bhvr
#		BhvrOverride
#			Alpha			50
#			StartColor		0 0 0
#			PyrRotate		180 0 0
#		End
#		geom	FX_DarkWave
#		Lifespan 8
#	End

	Event
		Type	StartPositOnly
		At	Target
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Hit_Shockwave_Balls.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Hit_Shockwave_Glow.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Hit_Shockwave_Smoke.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Hit_Shockwave_Rings.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Hit_Shockwave.part
		Lifespan 8
	End

	Event
		Type    Destroy
		EName 	Prime
	End

	Event
		Type    Destroy
		EName 	Prime2
	End
End

#############################################################

End