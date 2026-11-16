#############################################################
## TarotCards_Attack.fx
#############################################################

FxInfo

Lifespan 300

#############################################################

Condition
	On	Time
	Time	20

	Event
		Type	Local
		At 	Origin
		Sound Tarot_Cast_01 80 80 0.8
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Start
		At	Root
		ChildFX	:Child_WheelOfFortuneSigil.fx
	End
End

## ARM SPIRALS ###########################################################

Condition
	On	Time
	Time	10

	Event
		EName 	Offset_UArmR_Spiral
		Type	local
		At	UarmR
		Bhvr	:ArmSpiral.bhvr
		Lifespan 30
	End

	Event
		Type	local
		At	Offset_UArmR_Spiral
		BhvrOverride
			PositionOffset		0 0 -0.375
		End
		part	Emotes\Juggle\EnergyOrb\Red_Sparkles_01.part
		part	Emotes\Juggle\EnergyOrb\Red_Streak_01.part
		Lifespan 20
	End

	Event
		Type	local
		At	Offset_UArmR_Spiral
		BhvrOverride
			PositionOffset		0 0 0.375
		End
		part	Emotes\Juggle\EnergyOrb\Blue_Sparkles_01.part
		part	Emotes\Juggle\EnergyOrb\Blue_Streak_01.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	30

	Event
		EName 	Offset_LArmR_Spiral
		Type	Local
		At	LarmR
		Bhvr	:ArmSpiral.bhvr
		Lifespan 30
	End

	Event
		Type	Local
		At	Offset_LArmR_Spiral
		BhvrOverride
			PositionOffset		0 0 -0.375
		End
		part	Emotes\Juggle\EnergyOrb\Red_Sparkles_01.part
		part	Emotes\Juggle\EnergyOrb\Red_Streak_01.part
		Lifespan 30
	End

	Event
		Type	Local
		At	Offset_LArmR_Spiral
		BhvrOverride
			PositionOffset		0 0 0.375
		End
		part	Emotes\Juggle\EnergyOrb\Blue_Sparkles_01.part
		part	Emotes\Juggle\EnergyOrb\Blue_Streak_01.part
		Lifespan 30
	End
End

Condition
	On	Time
	Time	10

	Event
		EName 	Offset_UArmL_Spiral
		Type	local
		At	UArmL
		Bhvr	:ArmSpiral.bhvr
		BhvrOverride
			PositionOffset		0.25 0 0
			InitialVelocity		-0.05 0.0 0.0
		End
		Lifespan 30
	End

	Event
		Type	local
		At	Offset_UArmL_Spiral
		BhvrOverride
			PositionOffset		0 0 -0.375
		End
		part	Emotes\Juggle\EnergyOrb\Blue_Sparkles_01.part
		part	Emotes\Juggle\EnergyOrb\Blue_Streak_01.part
		Lifespan 20
	End

	Event
		Type	local
		At	Offset_UArmL_Spiral
		BhvrOverride
			PositionOffset		0 0 0.375
		End
		part	Emotes\Juggle\EnergyOrb\Red_Sparkles_01.part
		part	Emotes\Juggle\EnergyOrb\Red_Streak_01.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	30

	Event
		EName 	Offset_LArmL_Spiral
		Type	Local
		At	LArmL
		Bhvr	:ArmSpiral.bhvr
		BhvrOverride
			PositionOffset		0.25 0 0
			InitialVelocity		-0.05 0.0 0.0
		End
		Lifespan 30
	End

	Event
		Type	Local
		At	Offset_LArmL_Spiral
		BhvrOverride
			PositionOffset		0 0 -0.375
		End
		part	Emotes\Juggle\EnergyOrb\Blue_Sparkles_01.part
		part	Emotes\Juggle\EnergyOrb\Blue_Streak_01.part
		Lifespan 30
	End

	Event
		Type	Local
		At	Offset_LArmL_Spiral
		BhvrOverride
			PositionOffset		0 0 0.375
		End
		part	Emotes\Juggle\EnergyOrb\Red_Sparkles_01.part
		part	Emotes\Juggle\EnergyOrb\Red_Streak_01.part
		Lifespan 30
	End
End

## LEG SPIRALS ###########################################################

Condition
	On	Time
	Time	10

	Event
		EName 	Offset_ULegL_Spiral
		Type	local
		At	ULegL
		Bhvr	:LegSpiral.bhvr
		Lifespan 30
	End

	Event
		Type	local
		At	Offset_ULegL_Spiral
		BhvrOverride
			PositionOffset		0 0 -0.5
		End
		part	Emotes\Juggle\EnergyOrb\Red_Sparkles_01.part
		part	Emotes\Juggle\EnergyOrb\Red_Streak_01.part
		Lifespan 20
	End

	Event
		Type	local
		At	Offset_ULegL_Spiral
		BhvrOverride
			PositionOffset		0 0 0.5
		End
		part	Emotes\Juggle\EnergyOrb\Blue_Sparkles_01.part
		part	Emotes\Juggle\EnergyOrb\Blue_Streak_01.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	30

	Event
		EName 	Offset_LLegL_Spiral
		Type	Local
		At	LLegL
		Bhvr	:LegSpiral.bhvr
		Lifespan 30
	End

	Event
		Type	Local
		At	Offset_LLegL_Spiral
		BhvrOverride
			PositionOffset		0 0 -0.375
		End
		part	Emotes\Juggle\EnergyOrb\Red_Sparkles_01.part
		part	Emotes\Juggle\EnergyOrb\Red_Streak_01.part
		Lifespan 20
	End

	Event
		Type	Local
		At	Offset_LLegL_Spiral
		BhvrOverride
			PositionOffset		0 0 0.375
		End
		part	Emotes\Juggle\EnergyOrb\Blue_Sparkles_01.part
		part	Emotes\Juggle\EnergyOrb\Blue_Streak_01.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	10

	Event
		EName 	Offset_ULegR_Spiral
		Type	local
		At	ULegR
		Bhvr	:LegSpiral.bhvr
		BhvrOverride
			Spin			0.0 -0.5 0.0
		End
		Lifespan 30
	End

	Event
		Type	local
		At	Offset_ULegR_Spiral
		BhvrOverride
			PositionOffset		0 0 -0.5
		End
		part	Emotes\Juggle\EnergyOrb\Red_Sparkles_01.part
		part	Emotes\Juggle\EnergyOrb\Red_Streak_01.part
		Lifespan 20
	End

	Event
		Type	local
		At	Offset_ULegR_Spiral
		BhvrOverride
			PositionOffset		0 0 0.5
		End
		part	Emotes\Juggle\EnergyOrb\Blue_Sparkles_01.part
		part	Emotes\Juggle\EnergyOrb\Blue_Streak_01.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	30

	Event
		EName 	Offset_LLegR_Spiral
		Type	Local
		At	LLegR
		Bhvr	:LegSpiral.bhvr
		BhvrOverride
			Spin			0.0 -0.5 0.0
		End
		Lifespan 30
	End

	Event
		Type	Local
		At	Offset_LLegR_Spiral
		BhvrOverride
			PositionOffset		0 0 -0.375
		End
		part	Emotes\Juggle\EnergyOrb\Red_Sparkles_01.part
		part	Emotes\Juggle\EnergyOrb\Red_Streak_01.part
		Lifespan 20
	End

	Event
		Type	Local
		At	Offset_LLegR_Spiral
		BhvrOverride
			PositionOffset		0 0 0.375
		End
		part	Emotes\Juggle\EnergyOrb\Blue_Sparkles_01.part
		part	Emotes\Juggle\EnergyOrb\Blue_Streak_01.part
		Lifespan 20
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	HookupRootToChest
		Type	Local
		At	Root
		Geom	RootToChestAdjustment
	End

	Event
		EName	HookupOrigin
		Type	Local
		At	HookupRootToChest
		AltPiv	1
		BhvrOverride
			PositionOffset		0 0.5 0
		End
	End

	Event
		EName	HookupSpin
		Type	Local
		At	HookupRootToChest
		AltPiv  1
		BhvrOverride
			PositionOffset		0 -2 0
			Spin			0 -0.3 0
		End
		Geom	TarotCardRig
	End
End

#############################################################

Condition
	On	Time
	Time	15

	Event
		Type	Local
		At	WepL
		ChildFX	:Child_CardsHandsSwirlOut.fx
		Lifespan 20
	End

	Event
		Type	Local
		At	WepR
		ChildFX	:Child_CardsHandsSwirlOut.fx
		Lifespan 20
	End
End


#############################################################

End