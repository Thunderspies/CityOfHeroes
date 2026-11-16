#############################################################
## HEADER
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Chest
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\NictusSunFlareHitStreaks.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\NictusSunFlareHitStreaks.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\NictusSunFlareHitStreaks.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\BlastFlash.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\BlastFlashTendrils.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\QuickBlastHitStreaks.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\QuickBlastHitStreaksBlue.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\QuickBlastHitStreaks.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\QuickBlastHitStreaksBlue.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\PeaceBlastHitSparks.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\BlastFlashTendrilsPurple.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\BlastFlashTendrilsBlue.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\SoulDrainHitPuddle2.Part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\SoulDrainHitTendril2.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\SoulDrainHitTendrilBlue.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\SoulDrainHitTendrilPurple.part
		#Sound	WarshadeHit2 80 70 .99
		LifeSpan 10
	End

	Event
		Type	Start
		At	Chest
		Bhvr	POWERS\CosmicPowers\PeaceBringer\FootStomp\FootStompCameraShake.bhvr
		BhvrOverride
			Shake          1.5     #How much to shake the camera when event with this bhvr is created
			ShakeFallOff   0.5   #how long shake lasts
			ShakeRadius    128.0  #(falls off slowly, so at 18.0 feet from center, only a mild shake)
		End
		LifeSpan 10
	End
End

#############################################################

End