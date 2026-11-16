#############################################################
## FormShield_Test01.fx
#############################################################

FxInfo

Flags DONTSUPPRESS IsArmor

Lifespan 120

#############################################################

Condition
	On 	Time
	Time	0

	Event
		Type	Local
		At	Chest
		Bhvr	ENEMYFX\V_PraetorianPolice\FormShield\FormShield_QuickHitFade.bhvr
		Geom	FX_FormShield_Chest
		Lifespan 17
	End
End

Condition
	On 	Time
	Time	6

	Event
		Type	Local
		At	UArmR
		Bhvr	ENEMYFX\V_PraetorianPolice\FormShield\FormShield_QuickHitFade.bhvr
		Geom	FX_FormShield_UArmR
		Lifespan 11
	End

	Event
		Type	Local
		At	UArmL
		Bhvr	ENEMYFX\V_PraetorianPolice\FormShield\FormShield_QuickHitFade.bhvr
		Geom	FX_FormShield_UArmL
		Lifespan 11
	End
End

Condition
	On 	Time
	Time	12

	Event
		Type	Local
		At	LArmR
		Bhvr	ENEMYFX\V_PraetorianPolice\FormShield\FormShield_QuickHitFade.bhvr
		Geom	FX_FormShield_LArmR
		Lifespan 5
	End

	Event
		Type	Local
		At	LArmL
		Bhvr	ENEMYFX\V_PraetorianPolice\FormShield\FormShield_QuickHitFade.bhvr
		Geom	FX_FormShield_LArmL
		Lifespan 5
	End
End

Condition
	On 	Time
	Time	8

	Event
		Type	Local
		At	ULegR
		Bhvr	ENEMYFX\V_PraetorianPolice\FormShield\FormShield_QuickHitFade.bhvr
		Geom	FX_FormShield_ULegR
		Lifespan 9
	End

	Event
		Type	Local
		At	ULegL
		Bhvr	ENEMYFX\V_PraetorianPolice\FormShield\FormShield_QuickHitFade.bhvr
		Geom	FX_FormShield_ULegL
		Lifespan 9
	End
End

Condition
	On 	Time
	Time	16

	Event
		Type	Local
		At	LLegR
		Bhvr	ENEMYFX\V_PraetorianPolice\FormShield\FormShield_QuickHitFade.bhvr
		Geom	FX_FormShield_LLegR
		Lifespan 1
	End

	Event
		Type	Local
		At	LLegL
		Bhvr	ENEMYFX\V_PraetorianPolice\FormShield\FormShield_QuickHitFade.bhvr
		Geom	FX_FormShield_LLegL
		Lifespan 1
	End
End

#############################################################

End