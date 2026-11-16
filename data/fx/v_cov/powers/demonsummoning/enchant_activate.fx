#############################################################
## Enchant_Activate.fx
#############################################################

FxInfo
Lifespan 100

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type Local
		At Origin
		Sound Demon_BuffFX_01 80 80 1
	End

	Event
		Type	Local
		At	WepL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Enchant_Hands_Fire.part
		Part	:Enchant_Hands_Steam.part
		Part	:Enchant_Hands_Hellfire_Highlight.part
		Lifespan 65
	End

	Event
		Type	Local
		At	WepR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Enchant_Hands_Fire.part
		Part	:Enchant_Hands_Steam.part
		Part	:Enchant_Hands_Hellfire_Highlight.part
		Lifespan 65
	End
End

Condition
	On	Time
	Time	20

	Event
		Ename	ConeOffset
		Type	Start
		At	Root
		GEOM	FX_ConeOffsets
	End

	Event
		EName 	RuneAnchor
		Type	Local
		At	ConeOffset
		BhvrOverride
			PositionOffset	0 5 5
		End
		AltPiv	1
	End

	Event
		Type	Local
		At	RuneAnchor
		BhvrOverride
			PyrRotate 	-90 180 0
		End
		Part	:Enchant_Rune_Smoke.part
	End

End

Condition
	On	Time
	Time	40
	Event
		Type	Local
		At	RuneAnchor
		BhvrOverride
			PyrRotate 	-90 180 0
		End
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Enchant_Rune.part
	End
End

Condition
	On	Time
	Time	61

	Event
		EName	Prime
		At 	RuneAnchor
		Type	Start
		Bhvr	behaviors\BeanBagProjectile.bhvr
		BhvrOverride
			TrackRate	1.5
		End
		POther	T_Chest
		Magnet	T_Chest
		Lookat	T_Chest
		Lifespan 80
	End

	Event
		EName 	PrimeHit2
		Type	Posit
		At	Prime
		Part	:Enchant_Rune_Projectile.part
		Part	:Enchant_Rune_Blur_Projectile.part
		Part	:Enchant_Rune_SmokeTrail.part
		Part	:Enchant_Rune_SparkTrail.part

		BhvrOverride
			PyrRotate 	-90 180 0
		End
	End

End

Condition
	On	PrimeHit

	Event
		EName	Prime
		Type	Destroy
	End

	Event
		EName	PrimeHit2
		Type	Destroy
	End
End
#########################################################

End