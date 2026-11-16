#############################################################
## Ice Armor - Continuing FX (SHARED)
#############################################################

FxInfo
Flags InheritAlpha

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	LLEGL
		Bhvr 	:IceArmor.bhvr
		geom	FX_IceArmor_LLEGL
		part1   :Snowfall.part
	End

	Event
		Type	Local
		At	LLEGR
		Bhvr 	:IceArmor.bhvr
		geom	FX_IceArmor_LLEGR
		part1   :Snowfall.part
	End
End

#############################################################

Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	UlegR
		Bhvr 	:IceArmor.bhvr
		geom	FX_IceArmor_ULEGR
	End

	Event
		Type	Local
		At	ULEGL
		Bhvr 	:IceArmor.bhvr
		geom	FX_IceArmor_ULEGL
	End
End

#############################################################

Condition
	On 	Time
	Time 	25

	Event
		Type	Local
		At	Hips
		Bhvr 	:IceArmor.bhvr
		geom	FX_IceArmor_HIPS
	End

	Event
		Type	Local
		At	UARML
		Bhvr 	:IceArmor.bhvr
		geom	FX_IceArmor_UARML
	End

	Event
		Type	Local
		At	UARMR
		Bhvr 	:IceArmor.bhvr
		geom	FX_IceArmor_UARMR
	End
End

#############################################################

Condition
	On 	Time
	Time 	20

		Event
		Type	Local
		At	LARMR
		Bhvr 	:IceArmor.bhvr
		geom	FX_IceArmor_LARMR
		part1   :Snowfall.part
	End

		Event
		Type	Local
		At	LARML
		Bhvr 	:IceArmor.bhvr
		geom	FX_IceArmor_LARML
		part1   :Snowfall.part
	End
End

#############################################################

Condition
	On 	Time
	Time 	30

	Event
		Type	Local
		At	CHEST
		Bhvr 	:IceArmor.bhvr
		Geom	FX_IceArmor_CHEST
		part1   :Snowfall.part
	End
End

#############################################################

Condition
	On 	Time
	Time 	35

	Event
		Type	Local
		At	Head
		Bhvr 	:IceArmor.bhvr
		geom	FX_IceArmor_HEAD
	End
End

## RICOCHETS ###########################################################

Condition
	On 	Cycle
	Time 	30
	Chance	0.33

	Event
		Bhvr	behaviors/genericParticleFade.bhvr
		Type	Local
		At	LLEGL
		part1	:Ricochet_Ring.part
		part2	:Ricochet_Rays.part
		LifeSpan 33
	End
End

Condition
	On 	Cycle
	Time 	30
	Chance	0.33

	Event
		Bhvr	behaviors/genericParticleFade.bhvr
		Type	Local
		At	LLEGR
		part1	:Ricochet_Ring.part
		part2	:Ricochet_Rays.part
		LifeSpan 33
	End
End

Condition
	On 	Cycle
	Time 	35
	Chance	0.33

	Event
		Bhvr	behaviors/genericParticleFade.bhvr
		Type	Local
		At	UArmL
		part1	:Ricochet_Ring.part
		part2	:Ricochet_Rays.part
		LifeSpan 33
	End
End

Condition
	On 	Cycle
	Time 	27
	Chance	0.33

	Event
		Bhvr	behaviors/genericParticleFade.bhvr
		Type	Local
		At	UArmR
		part1	:Ricochet_Ring.part
		part2	:Ricochet_Rays.part
		LifeSpan 33
	End
End

#############################################################

End