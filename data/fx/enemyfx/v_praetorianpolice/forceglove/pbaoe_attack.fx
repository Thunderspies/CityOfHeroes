#############################################################
## ForceBolt.fx
#############################################################

FxInfo

LifeSpan 40

#############################################################

Condition
	On	Time
	Time	5

	Event
		EName	EmitterFlare_Left
		Type	Local
		At	WepL
		Sound PPD_ForceShockwave_Attack_01 100 100 1
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
	Time 	25

	Event
		EName 	CameraShake
		Type	Start
		At	mystic
		Bhvr	Behaviors\CameraShake01.bhvr
		BhvrOverride
			Shake		2.0	#How much to shake the camera when event with this bhvr is created
			ShakeFallOff	0.25	#how long shake lasts
			ShakeRadius	48	#(falls off slowly, so at 18.0 feet from center, only a mild shake)
		End
	End

	Event
		EName 	Mallet2
		Type	start
		At	mystic
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Hit_Flash.part
		Part	POWERS\ElectricityControl\ElectricityMeleeHitStar.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBurst_Hit_Star_Thin.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBurst_Hit_Ring.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBurst_Hit_Ring_Thin.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBurst_Hit_Ring_Large.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Hit_Glow.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Hit_Glow_Large.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBurst_Hit_Glow_Large.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Hit_Balls_Large.part
		Sound PPD_ForceShockwave_Hit_01 100 100 1
		LifeSpan 9
	End
End

#############################################################

End