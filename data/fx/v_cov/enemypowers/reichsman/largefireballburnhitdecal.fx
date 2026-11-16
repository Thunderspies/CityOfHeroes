#########################################################
##	onfire
##
FxInfo

LifeSpan	140


Condition
	Time 	0

	Event
		EName	VaultHookup
		Type	Startpositonly
		At	origin
		Part	:LargeFireballBurnHitGlowlower.part

		LifeSpan	8

	End

	Event
		EName	VaultHookup
		Type	Startpositonly
		At	origin

		Part	:LargeFireballBurnHitGlow.part
		Part	:LargeFireballBurnHitGlow2.part
		Part	:LargeFireballBurnHitEmbers.part
		LifeSpan	2
	End

	Event
		EName	VaultHookup
		Type	Startpositonly
		At	origin
		Part	:LargeFireballBurnHitScorch.part
		Part	:LargeFireballBurnHitGlowStar.part
		Part	:LargeFireballBurnHitGlowStarCore.part

		LifeSpan	10

	End

	Event
		EName	VaultHookup
		Type	Startpositonly
		At	origin
		Part	:LargeFireballBurnHitDecal.part

		LifeSpan	1

	End

	Event
		EName	VaultHookup
		Type	Startpositonly
		At	origin
	#	Part	:Steam.part

		LifeSpan	23

	End


End


End
