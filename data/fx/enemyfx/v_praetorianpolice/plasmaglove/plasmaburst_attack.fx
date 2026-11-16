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
	Time	15

	Event
		EName	EmitterFlare_Left
		Type	Local
		At	WepL
		Sound PPD_PlasmaBurst_Attack_01 200 200 1
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
	Time 	22

	Event
		EName	PlasmaGlove_Left
		Type	Local
		At	LArmL
		ChildFX	ENEMYFX\V_PraetorianPolice\PlasmaGlove\Child_PlasmaGlove_Attack_Left.fx
	End

	Event
		EName	PlasmaGlove_Right
		Type	Local
		At	LArmR
		ChildFX	ENEMYFX\V_PraetorianPolice\PlasmaGlove\Child_PlasmaGlove_Attack_Right.fx
	End
End

#############################################################

Condition
	On 	Time
	Time 	22

	Event
		Type	Start
		At	WepL
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\LargeFlash_Star01.part
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\LargeFlash_Star02.part
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\LargeFlash_Energy01.part
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\LargeFlash_Energy02.part
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\LargeFlash_Glow01.part
		LifeSpan 1
	End

	Event
		EName 	Prime
		Type	Start
		At	WepL
		Bhvr	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolt_Projectile01.bhvr
		Bhvr	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolt_Projectile01.bhvr
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Glow01.part
		Part	ENEMYFX\V_PraetorianPolice\PlasmaGlove\PlasmaBlast_Mist01.part
		Part	ENEMYFX\V_PraetorianPolice\PlasmaGlove\PlasmaBlast_Trail01.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Trail02.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Trail03.part
		Part	ENEMYFX\V_PraetorianPolice\PlasmaGlove\PlasmaBlast_Rings01.part
		Part	ENEMYFX\V_PraetorianPolice\PlasmaGlove\Projectile_Trail_Ember_Small.part
		Part	ENEMYFX\V_PraetorianPolice\PlasmaGlove\Projectile_Trail_Tendril_Small.part
		Part	ENEMYFX\V_PraetorianPolice\PlasmaGlove\Projectile_Trail_Glow_Small.part
		Geom	ForceBolt_Greyscale
		Magnet	Target
		LookAt	Target
	End

	Event
		Type	Start
		At	WepR
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\LargeFlash_Star01.part
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\LargeFlash_Star02.part
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\LargeFlash_Energy01.part
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\LargeFlash_Energy02.part
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\LargeFlash_Glow01.part
		LifeSpan 1
	End

	Event
		EName 	Prime2
		Type	Start
		At	WepR
		Bhvr	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolt_Projectile01.bhvr
		Bhvr	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolt_Projectile01.bhvr
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Glow01.part
		Part	ENEMYFX\V_PraetorianPolice\PlasmaGlove\PlasmaBlast_Mist01.part
		Part	ENEMYFX\V_PraetorianPolice\PlasmaGlove\PlasmaBlast_Trail01.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Trail02.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Trail03.part
		Part	ENEMYFX\V_PraetorianPolice\PlasmaGlove\PlasmaBlast_Rings01.part
		Part	ENEMYFX\V_PraetorianPolice\PlasmaGlove\Projectile_Trail_Ember_Small.part
		Part	ENEMYFX\V_PraetorianPolice\PlasmaGlove\Projectile_Trail_Tendril_Small.part
		Part	ENEMYFX\V_PraetorianPolice\PlasmaGlove\Projectile_Trail_Glow_Small.part
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