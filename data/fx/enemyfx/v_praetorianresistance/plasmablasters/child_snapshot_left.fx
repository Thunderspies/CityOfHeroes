#############################################################
## Mercenary Mastermind Assault Rifle - Slug
#############################################################

FxInfo

LifeSpan 300

#############################################################

Condition
	On 	Time
	Time 	26

	Event
		EName	MuzzleFlashOffset
		Type	Local
		At	WepL
		BhvrOverride
			PositionOffset		-0.5 0 0.625
		End
		ChildFX	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Child_MuzzleFlash_01.fx
		Lifespan 30
	End
End

Condition
	On 	Time
	Time 	28

	Event
		Ename	Prime
		Type	Start
		At	MuzzleFlashOffset
		Bhvr	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Projectile_Fade.bhvr
		BhvrOverride
			TrackRate	12.0
		End
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Projectile_Core01.part
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Projectile_Glow01.part
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Projectile_Trail01.part
		LookAt	Target
		Magnet	Target
	End
End

#############################################################

Condition
	On	PrimeHit

	Event
		Type	Destroy
		Ename	ALL
	End
End

#############################################################

End