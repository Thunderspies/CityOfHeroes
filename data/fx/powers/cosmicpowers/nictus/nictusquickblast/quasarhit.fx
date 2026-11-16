#########################################################
## Umbral Blast - Quasar AOE Hit
#########################################################

FxInfo

LifeSpan	200

#########################################################

Condition
	On 	Time
	Time 	0
	Event
		EName	SwordFlash2
		Type	Local
		At	Chest
		Part	:NictusSunFlareHitStreaks.part
		Part	:BlastFlash.part
		Part	:BlastFlashTendrils.part
		Part	:QuickBlastHitStreaks.part
		Part	:QuickBlastHitStreaksBlue.part
		Part	:PeaceBlastHitSparks.part
		Part	:BlastFlashTendrilsPurple.part
		Part	:BlastFlashTendrilsBlue.part
		LifeSpan 10
	End
End

#########################################################

End			