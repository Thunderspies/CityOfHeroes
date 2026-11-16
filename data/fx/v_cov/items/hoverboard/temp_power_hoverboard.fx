#############################################################
## Temp_power_Hoverboard.fx
#############################################################

FxInfo
Flags InheritAlpha DontSuppress
###############################################################################################

Condition

	Event
		Type	Local
		At	Root
		Sound 	HoverboardFly_Loop 40 40 .05
	End
End


Condition
	On	TriggerBits
	TriggerBits	HOVERBOARD
	DoMany		1

	Event
		EName 	Hoverboard
		Type	local
		At	Mystic
		While	HOVERBOARD
		Flags	OneAtATime
		Geom	GEO_Hoverboard_01
		BhvrOverride
			PositionOffset		0 0 0
			Scale			1 1 1
		End
	End

	Event
		Ename	Stabilizer01
		Type	local
		At	Hoverboard
		While	HOVERBOARD
		Until	FORWARD
		Flags	OneAtATime
		AltPiv	3
#		Bhvr	Behaviors\GenericParticleFade.bhvr
#		BhvrOverride
#			FadeOutLength	5
#		End
		Part	:Stabilizer_Bursts.part
	End

	Event
		Ename	Stabilizer02
		Type	local
		At	Hoverboard
		While	HOVERBOARD
		Until	FORWARD
		Flags	OneAtATime
		AltPiv	4
#		Bhvr	Behaviors\GenericParticleFade.bhvr
#		BhvrOverride
#			FadeOutLength	5
#		End
		Part	:Stabilizer_Bursts.part
	End
End
###############################################################################################

Condition
	On	TriggerBits
	TriggerBits	HOVERBOARD
	DoMany		1

	Event
		EName 	ChildThrust
		Type	local
		At	Chest
		While	HOVERBOARD
		Flags	OneAtATime
	End
End
############################################################

Condition
	On		Triggerbits
	Triggerbits	FORWARD
	DoMany		1

	Event
		EName 	Thruster01
		Type	local
		At	Hoverboard
		While	FORWARD
		Flags	OneAtATime
		AltPiv	1
		ChildFX	:Hoverboard_Jet_Child.fx
	End

	Event
		EName 	Thruster02
		Type	local
		At	Hoverboard
		While	FORWARD
		Flags	OneAtATime
		AltPiv	2
		ChildFX	:Hoverboard_Jet_Child.fx
	End
End

############################################################
End