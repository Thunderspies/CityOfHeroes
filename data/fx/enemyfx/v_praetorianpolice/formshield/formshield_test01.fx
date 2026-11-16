#############################################################
## FormShield_Test01.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	TriggerBits
	Triggerbits	HIT
	DoMany		1
	Chance		1.0

	Event
		EName	FormShield_HitFlash
		Type	Local
		At	Chest
		Flags	OneAtATime
		Bhvr	ENEMYFX\V_PraetorianPolice\FormShield\FormShield_QuickHitFade.bhvr
		Geom	FX_FormShield_Chest
		Lifespan 9
	End
End

#############################################################

End