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
		Anim	FX_JuggleBalls_Female
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
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:EnergyOrb\Red_Glow_01.part
		part	:EnergyOrb\Red_Sparkles_01.part
		part	:EnergyOrb\Red_Swirl_01.part
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
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:EnergyOrb\Blue_Glow_01.part
		part	:EnergyOrb\Blue_Sparkles_01.part
		part	:EnergyOrb\Blue_Swirl_01.part
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
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:EnergyOrb\Yellow_Glow_01.part
		part	:EnergyOrb\Yellow_Sparkles_01.part
		part	:EnergyOrb\Yellow_Swirl_01.part
	End
End

#############################################################

End