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
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part1	:Stamina2.part
		Part2	:StaminaSmall2.part
		Part3	:AbilityDots.part
		Part4	:AbilitySparklingDots.part
		Sound Soothing_Aura_01 70 70 .75
		Lifespan 45
	End

	Event
		EName	Right
		Type	Local
		At	WepR
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part1	:Stamina2.part
		Part2	:StaminaSmall2.part
		Part3	:AbilityDots.part
		Part4	:AbilitySparklingDots.part
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
		Bhvr	behaviors\soundFade6.bhvr
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
		Bhvr 	:Aura_Expand.bhvr
		BhvrOverride
			Spin		0.0 0.01 0.0
			Alpha		96
		End
		Part1	:Aura_Motes1.part
		Part1	:Aura_Motes2.part
		Part1	:Aura_BubbleEdge1.part
		Geom	FX_PillBoxForceField #FX_ForceBubble
	End

	Event
		Type	PositOnly
		At	HIPS
		Bhvr 	:Aura_Expand.bhvr
		BhvrOverride
			Spin	0.0 -0.01 0.0
		End
		Part1	:Aura_Motes1.part
		Part1	:Aura_Motes2.part
	End
End

#############################################################

End