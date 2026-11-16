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
		At	LArmR
		Bhvr	ENEMYFX\V_PraetorianPolice\PlasmaGlove\PlasmaGlove_AttackFlash.bhvr
		BhvrOverride
		End
		Part	ENEMYFX\V_PraetorianPolice\PlasmaGlove\PlasmaGlove_Attack_Glow01.part
		Geom	GEO_FormShield_Gauntlet_LArmR
		pOther	WepR
		Lifespan 10
	End
End

#############################################################

End