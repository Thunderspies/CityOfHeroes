#############################################################
## SingleLargePowerPunch.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	LArmL
		Bhvr	ENEMYFX\V_PraetorianPolice\PlasmaGlove\PlasmaGlove_AttackFlash.bhvr
		BhvrOverride
		End
		Part	ENEMYFX\V_PraetorianPolice\PlasmaGlove\PlasmaGlove_Attack_Glow01.part
		Geom	GEO_FormShield_Gauntlet_LArmL
		pOther	WepL
		Lifespan 10
	End
End

#############################################################

End