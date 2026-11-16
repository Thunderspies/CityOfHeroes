#############################################################
## ForceBolt.fx
#############################################################

FxInfo

LifeSpan 200

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
	On	Time
	Time	18

	Event
		EName	EmitterFlare_Left
		Type	Local
		At	WepL
		Sound PPD_ForceBlast_Attack_01 200 200 1
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
	Time 	20

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