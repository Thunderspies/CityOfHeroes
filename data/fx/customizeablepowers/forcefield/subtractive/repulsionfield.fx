#############################################################
## RepulsionField.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time 	0

	Event
		Type 	Local
		At	Origin
		Sound forcehands6 80 80 .8
	End

	Event
		EName 	Hookup
		Type	PositOnly
		At	Hips
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Geom	RootToChestAdjustment
	End
End

Condition
	On	Time
	Time 	10

	Event
		Type 	Local
		At	Origin
		Bhvr	Behaviors\soundFade3.bhvr
		Sound ForceField3_loop 100.0 100.0 .42
		Flags	PowerLoopingSound
		Lifespan 220
	End
End

Condition
	On	Time
	Time 	10

	Event
		Type 	Local
		At	Origin
		Bhvr	Behaviors\soundFade3.bhvr
		Sound ForceField5_loop 100.0 100.0 .3
		Flags	PowerLoopingSound
		Lifespan 220
	End
End

## HANDS ###########################################################

Condition
	On 	Time
	Time 	10

	Event
		EName 	Prime
		Type	Posit
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	:HandGlows.part
	End

	Event
		EName 	Prime
		Type	Posit
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	:HandGlows.part
	End
End

Condition
	On 	Time
	Time 	53

	Event
		EName 	Prime3
		Type	start
		At	WepR
		Bhvr	CustomizeablePowers\ForceField\ForceBubbleHands.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	tintable_Dark_FX_ForceBubble
		Lifespan 9
	End

	Event
		EName 	Prime4
		Type	start
		At	WepL
		Bhvr	CustomizeablePowers\ForceField\ForceBubbleHands.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	tintable_Dark_FX_ForceBubble
		Lifespan 9
	End
End

#############################################################

Condition
	On 	Time
	Time 	20

	Event
		Ename	ForceBall
		Type	local
		At	Hookup
		Bhvr	CustomizeablePowers\ForceField\ForceFieldBubble.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	tintable_Dark_FX_ForceBubble
		Lifespan 8
	End
End

## LARGE REPLUSION FIELD ###########################################################

Condition
	On 	Time
	Time 	20

	Event
		ENAME	BubbleHit01
		Type	local
		At	Hookup
		Bhvr	CustomizeablePowers\ForceField\ForceRepulsion01.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_Dark_ForceRepulsion01
		Lifespan 13
	End

	Event
		ENAME	BubbleHit02
		Type	local
		At	Hookup
		Bhvr	CustomizeablePowers\ForceField\ForceRepulsion02.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_Dark_ForceRepulsion02
		Lifespan 13
	End

	Event
		ENAME	BubbleHit03
		Type	local
		At	Hookup
		Bhvr	CustomizeablePowers\ForceField\ForceRepulsion03.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_Dark_ForceRepulsion03
		Lifespan 13
	End

	Event
		ENAME	BubbleHit04
		Type	local
		At	Hookup
		Bhvr	CustomizeablePowers\ForceField\ForceRepulsion04.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_Dark_ForceRepulsion04
		Lifespan 13
	End
End

## MEDIUM REPLUSION FIELD ###########################################################

Condition
	On 	Time
	Time 	23

	Event
		ENAME	BubbleHit01a
		Type	local
		At	Hookup
		Bhvr	CustomizeablePowers\ForceField\ForceRepulsion01a.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_Dark_ForceRepulsion01
		Lifespan 5
	End

	Event
		ENAME	BubbleTrail01
		Type	local
		At	BubbleHit01
		Part1	:ForceRepulsionTrail.part
	End

	Event
		ENAME	BubbleHit02a
		Type	local
		At	Hookup
		Bhvr	CustomizeablePowers\ForceField\ForceRepulsion02a.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_Dark_ForceRepulsion02
		Lifespan 5
	End

	Event
		ENAME	BubbleTrail02
		Type	local
		At	BubbleHit02
		Part1	:ForceRepulsionTrail.part
	End

	Event
		ENAME	BubbleHit03a
		Type	local
		At	Hookup
		Bhvr	CustomizeablePowers\ForceField\ForceRepulsion03a.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_Dark_ForceRepulsion03
		Lifespan 5
	End

	Event
		ENAME	BubbleTrail03
		Type	local
		At	BubbleHit03
		Part1	:ForceRepulsionTrail.part
	End

	Event
		ENAME	BubbleHit04a
		Type	local
		At	Hookup
		Bhvr	CustomizeablePowers\ForceField\ForceRepulsion04a.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_Dark_ForceRepulsion04
		Lifespan 5
	End

	Event
		ENAME	BubbleTrail04
		Type	local
		At	BubbleHit04
		Part1	:ForceRepulsionTrail.part
	End
End

## SMALL REPULSION FIELD ###########################################################

Condition
	On 	Time
	Time 	27

	Event
		ENAME	BubbleHit01b
		Type	local
		At	Hookup
		Bhvr	CustomizeablePowers\ForceField\ForceRepulsion01b.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_Dark_ForceRepulsion01
		Lifespan 10
	End

	Event
		ENAME	BubbleHit02b
		Type	local
		At	Hookup
		Bhvr	CustomizeablePowers\ForceField\ForceRepulsion02b.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_Dark_ForceRepulsion02
		Lifespan 10
	End

	Event
		ENAME	BubbleHit03b
		Type	local
		At	Hookup
		Bhvr	CustomizeablePowers\ForceField\ForceRepulsion03b.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_Dark_ForceRepulsion03
		Lifespan 10
	End

	Event
		ENAME	BubbleHit04b
		Type	local
		At	Hookup
		Bhvr	CustomizeablePowers\ForceField\ForceRepulsion04b.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_Dark_ForceRepulsion04
		Lifespan 10
	End
End

## REPULSION CONTINUING EFFECT ###########################################################

Condition
	On 	Cycle
	Time 	35

	Event
		Type	local
		At	Hookup
		Bhvr	CustomizeablePowers\ForceField\ForceBombLarge.bhvr
		BhvrOverride
			TintGeom	1
		End
		Part1	:RepulsionRing3.part
		Part1	:BubbleSphereMelee.part
		Geom	tintable_Dark_FX_ForceBubble
		Lifespan 1
	End
End

#############################################################

End