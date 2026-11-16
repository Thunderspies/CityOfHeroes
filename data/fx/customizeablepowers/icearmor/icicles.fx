#############################################################
## Ice Armor - Icicles
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		Sound coldBlast2 80 80 .7
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Chest
		bhvr	behaviors\soundFade3.bhvr
		Sound icicle2_loop 80 80 .85
		Flags	PowerLoopingSound
		Lifespan	250
	End
End

#############################################################

Condition
	On 	cycle
	Time 	7
	Chance	0.125

	Event
		EName 	Stonesickle01
		Type	local
		At	Chest
		Bhvr	CustomizeablePowers\IceArmor\Icicles1.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_Icicle01
		LifeSpan	10
	End
End

Condition
	On 	cycle
	Time 	8
	Chance	0.125

	Event
		EName 	Stonesickle02
		Type	local
		At	Chest
		Bhvr	CustomizeablePowers\IceArmor\Icicles2.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_Icicle02
		LifeSpan	10
	End
End

Condition
	On 	Cycle
	Time 	9
	Chance	0.125

	Event
		EName 	Stonesickle03
		Type	local
		At	Chest
		Bhvr	CustomizeablePowers\IceArmor\Icicles1.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_Icicle03
		LifeSpan	10
	End
End

Condition
	On 	Cycle
	Time 	7
	Chance	0.125

	Event
		EName 	Stonesickle04
		Type	local
		At	Chest
		Bhvr	CustomizeablePowers\IceArmor\Icicles2.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_Icicle04
		LifeSpan	10
	End
End

Condition
	On 	Cycle
	Time	8
	Chance	0.125

	Event
		EName 	Stonesickle10
		Type	local
		At	Chest
		Bhvr	CustomizeablePowers\IceArmor\Icicles1.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_Icicle10
		LifeSpan	10
	End
End

Condition
	On 	cycle
	Time 	9
	Chance	0.125

	Event
		EName 	Stonesickle05
		Type	local
		At	Chest
		Bhvr	CustomizeablePowers\IceArmor\Icicles2.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_Icicle05
		LifeSpan	10
	End
End

Condition
	On 	cycle
	Time 	7
	Chance	0.125

	Event
		EName 	Stonesickle06
		Type	local
		At	Chest
		Bhvr	CustomizeablePowers\IceArmor\Icicles1.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_Icicle06
		LifeSpan	10
	End
End

Condition
	On 	Cycle
	Time 	8
	Chance	0.125

	Event
		EName 	Stonesickle07
		Type	local
		At	Chest
		Bhvr	CustomizeablePowers\IceArmor\Icicles2.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_Icicle07
		LifeSpan	10
	End
End

Condition
	On 	cycle
	Time 	9
	Chance	0.125

	Event
		EName 	Stonesickle08
		Type	local
		At	Chest
		Bhvr	CustomizeablePowers\IceArmor\Icicles1.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_Icicle08
		LifeSpan	10
	End
End

Condition
	On 	cycle
	Time 	7
	Chance	0.125

	Event
		EName 	Stonesickle09
		Type	local
		At	Chest
		Bhvr	CustomizeablePowers\IceArmor\Icicles2.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_Icicle09
		LifeSpan	10
	End
End

#############################################################

End