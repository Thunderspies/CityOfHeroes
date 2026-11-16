#############################################################
## HEADER
#############################################################

FxInfo

Flags InheritAnimScale

#############################################################

Condition

	Event
		EName	JuggleRig
		Type	Local
		At	Mystic
		Anim	FX_JuggleBalls_Male
	End
End

## BALLS ###########################################################

Condition
	On 	Time
	Time 	12

	Event
		EName 	Ball1
		Type	Local
		At	JuggleRig
		AnimPiv	Neck
		Part	Emotes\Juggle\Electric\WitchSparks2.part
		Part	Emotes\Juggle\Electric\ElectricityGlowStarSmall02.part
		Part	Emotes\Juggle\Electric\BodyArcsSmall03.part
		Part	Emotes\Juggle\Electric\GlowRingSmall2.part
		Part	Emotes\Juggle\Electric\CoreElectricity.part
	End
End

Condition
	On 	Time
	Time 	25

	Event
		EName 	Ball2
		Type	Local
		At	JuggleRig
		AnimPiv	Waist
		Part	Emotes\Juggle\Electric\WitchSparks2.part
		Part	Emotes\Juggle\Electric\ElectricityGlowStarSmall02.part
		Part	Emotes\Juggle\Electric\BodyArcsSmall03.part
		Part	Emotes\Juggle\Electric\GlowRingSmall2.part
		Part	Emotes\Juggle\Electric\CoreElectricity.part
	End
End

Condition
	On 	Time
	Time 	35

	Event
		EName 	Ball3
		Type	Local
		At	JuggleRig
		AnimPiv	Chest
		Part	Emotes\Juggle\Electric\WitchSparks2.part
		Part	Emotes\Juggle\Electric\ElectricityGlowStarSmall02.part
		Part	Emotes\Juggle\Electric\BodyArcsSmall03.part
		Part	Emotes\Juggle\Electric\GlowRingSmall2.part
		Part	Emotes\Juggle\Electric\CoreElectricity.part
	End
End

#############################################################

End