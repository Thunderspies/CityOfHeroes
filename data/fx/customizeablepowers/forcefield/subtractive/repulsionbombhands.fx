#############################################################
## RepulsionBombHands.fx
#############################################################

FxInfo

Flags InheritAnimScale

Lifespan 150

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	WepR
		Part1	:HandGlows_Repulsion.part
		Part1	:BubbleSphereSmall_RepulsionMove.part
		LifeSpan 60
	End

	Event
		Type	local
		At	WepL
		Part1	:HandGlows_Repulsion.part
		Part1	:BubbleSphereSmall_RepulsionMove.part
		LifeSpan 60
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	LocknarDummyHookup
		Type	Start
		At	Head
		Geom	FX_InfectOthers
		LookAt	Target
		LifeSpan 100
	End
End

Condition
	On	Time
	Time 	7

	Event
		Type 	Local
		At	Origin
		Sound forcehands8c 80 80 .7
	End
End

#############################################################

Condition
	On 	Time
	Time 	25

	Event
		EName	EmitterGlobal
		Type	local
		At	LocknarDummyHookup
		BhvrOverride
			PositionOffset	0 2 0
			Gravity		-.0005
			InitialVelocity	0 0 -.025
		End
	End

	Event
		EName	EmitterSpin
		Type	Posit
		At	EmitterGlobal
		BhvrOverride
			Spin		-.25 0 0
		End
	End

	Event
		EName	EmitterChild
		Type	Local
		At	EmitterSpin
		BhvrOverride
			PositionOffset	0 .2 0
		End
	End

	Event
		EName 	LocknarFadeIn
		Type	Local
		At	EmitterChild
		Bhvr	CustomizeablePowers\ForceField\BubbleFadeIn.bhvr
		BhvrOverride
			Alpha			255
			FadeOutLength	180
			FadeInLength	90
			TintGeom	1
		End
		Geom	tintable_Dark_FX_ForceBubble
		Lifespan 23
	End

	Event
		EName 	LocknarFadeIndots
		Type	Posit
		At	EmitterGlobal	#LocknarDummyHookup
		Bhvr	CustomizeablePowers\ForceField\BubbleFadeIn2.bhvr
		Part1	:BubbleSphereSmall_Repulsion.part
		PMagnet	LocknarDummyHookup
		Lifespan 48
	End
End

#############################################################

Condition
	On 	Time
	Time 	48

	Event
		EName 	Prime
		Type	Start
		At	EmitterChild
		Bhvr	CustomizeablePowers\ForceField\ForceFieldMorterLobbprojectile.bhvr
		BhvrOverride
			Alpha			255
			TrackRate		1.5
			PositionOffset		0 -0.8 0
			TintGeom		1
		End
		Part1	:ForceRepulsionTrail.part
		Part2	:RepulsionMist01.part
		Part3	:RepulsionBallTrail.part
		Part4	:BubbleSphereSmalla.part
		Geom	tintable_Dark_FX_ForceBubble
		Magnet	Target
		LookAt	Target
	End
End

#############################################################

Condition
	On	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
	End

	Event
		Ename	Mushroom
		Type	Local
		At	T_Root
		ChildFx :RepulsionBomb.fx
	End
End

#############################################################

End
