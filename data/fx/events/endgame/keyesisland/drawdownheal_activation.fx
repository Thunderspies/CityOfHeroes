#############################################################
## Costume Aura - Flies (Body)
#############################################################

FxInfo
Lifespan 60

########################################################

Condition
	On	Time
	Time	0
	Event
		Type	Local
		At 	Root
		Sound	DrawDownHeal_Activation_01 100 100 .8
	End

	Event
		Type	Local
		At	Chest
		Part	:Nucleus_Glow_Large.part
		Part	:Nucleus_Glow_Core.part
	End

	Event
		Type	StartPositOnly
		At	Root
		Part	:Sparks_Rising.part
		Part	:Drawdown_Light_Column.part

	End

	Event
		EName	Sigil
		Type	Start
		At	Root
		Bhvr	behaviors\Splat_RuneCircle.bhvr
		BhvrOverride
			Alpha		255

			StartColor	100 255 0
			Scale		20.0 20.0 20.0
			SplatFlags	ADDITIVE
		End
		Splat	Generic_Glow2.tga
		Lifespan 80
	End

End

#############################################################

End