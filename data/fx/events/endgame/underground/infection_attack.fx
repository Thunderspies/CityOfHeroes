#############################################################
## LichenHeal_Attack.fx
#############################################################

FxInfo
LifeSpan 90

#############################################################

Input
	InpName	Hips
End

#############################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	Infection_Attack_01 100 100 .8
	End
End

Condition
	On 	Time
	Time 	25

	Event
		Type	StartPositOnly
		At	T_Root
		Bhvr	behaviors/CameraShake01.bhvr
		BhvrOverride
			PositionOffset	0.0 0.5 0.0
		End
		Part	:Infection_Dust_Root.part
		Lifespan 10
	End

	Event
		Type	StartPositOnly
		At	T_Root
		Bhvr	behaviors/CameraShake01.bhvr
		BhvrOverride
			PositionOffset	0.0 0.5 0.0
		End
		Part	:Infection_Spores.part
		Part	:Infection_Streaks.part
		Part	:Infection_Dust.part
		Part	:Infection_Spores_Burst.part
		Part	:Infection_Spores_Burst_Linger.part
		Part	:Light_Flash.part
		Part	:Light_Shockwave.part
		Lifespan 5
	End

	Event
		Type	StartPositOnly
		At	T_Root
		Bhvr	:Dust_Expanding.bhvr
		BhvrOverride
			PositionOffset	0.0 0.5 0.0
		End
		Part	:Infection_Dust_Burst.part
		Part	:Infection_Spores_Pillar.part

		Lifespan 20
	End

	Event
		Type	StartPositOnly
		At	T_Root
		BhvrOverride
			PositionOffset	0.0 -1.0 0.0
		End
		Part	:Infection_Dust_Burst_Small.part
		Part	:Infection_Dust_Pillar.part
		Lifespan 20
	End

#############################################################

End