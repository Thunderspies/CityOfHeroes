#############################################################
## HEADER
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Left
		Type	Local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\PainDomination\Stamina2.part
		Part2	CustomizeablePowers\PainDomination\StaminaSmall2.part
		Part3	CustomizeablePowers\PainDomination\AbilityDots.part
		Part4	CustomizeablePowers\PainDomination\AbilitySparklingDots.part
		Sound Soothing_Aura_01 70 70 .75
		Lifespan 45
	End

	Event
		EName	Right
		Type	Local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\PainDomination\Stamina2.part
		Part2	CustomizeablePowers\PainDomination\StaminaSmall2.part
		Part3	CustomizeablePowers\PainDomination\AbilityDots.part
		Part4	CustomizeablePowers\PainDomination\AbilitySparklingDots.part
		Lifespan 45
	End
End


#############################################################

Condition
	On	Time
	Time 	7

	Event
		Type 	Local
		At	Origin
		Sound WorldOfConfusion_Start_01 80 80 .4
	End
End

Condition
	On	Time
	Time 	17

	Event
		Type 	Local
		At	Origin
		Bhvr	CustomizeablePowers\PainDomination\soundFade6.bhvr
		Sound WorldOfConfusion_Loop 100.0 100.0 .65
		Flags	PowerLoopingSound
		Lifespan 120
	End
End

#############################################################

Condition
	On 	Time
	Time 	17

	Event
		Type	PositOnly
		At	HIPS
		Bhvr	CustomizeablePowers\PainDomination\Aura_Expand.bhvr
		BhvrOverride
			TintGeom	1
			Spin		0.0 0.01 0.0
			Alpha		35
		End
		Part1	CustomizeablePowers\PainDomination\Aura_Motes1.part
		Part1	CustomizeablePowers\PainDomination\Aura_Motes2.part
		Part1	CustomizeablePowers\PainDomination\Aura_BubbleEdge1.part
		Geom	Tintable_FX_ForceBubble
	End

	Event
		Type	PositOnly
		At	HIPS
		Bhvr	CustomizeablePowers\PainDomination\Aura_Expand.bhvr
		BhvrOverride
			Spin	0.0 -0.01 0.0
		End
		Part1	CustomizeablePowers\PainDomination\Aura_Motes1.part
		Part1	CustomizeablePowers\PainDomination\Aura_Motes2.part
	End
End

#############################################################

End