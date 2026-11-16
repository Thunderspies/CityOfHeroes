#############################################################
## PowerPunchAttached.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	LArmL
		Sound PPD_ForceGlove_Loop 50 50 .05
		BhvrOverride
			StartColor	255 192 48
		End
		Geom	GEO_FormShield_Gauntlet_Pulsing_LArmL
		pOther	WepL
	End

	Event
		Type	Local
		At	LArmR
		BhvrOverride
			StartColor	255 192 48
		End
		Geom	GEO_FormShield_Gauntlet_Pulsing_LArmR
		pOther	WepR
	End
End

Condition
	On 	Cycle
	Time 	30

	Event
		Type	Local
		At	LArmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceGlove_Glow01.part
		pOther	WepL
		Lifespan 1
	End

	Event
		Type	Local
		At	LArmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceGlove_Glow01.part
		pOther	WepR
		Lifespan 1
	End
End

#############################################################

End