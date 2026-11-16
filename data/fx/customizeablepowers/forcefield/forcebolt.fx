#############################################################
## Tintable_ForceBolt.fx
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	Origin
		Sound ForceBolthit 120 120 0.7
	End
End

Condition
	On 	Time
	Time 	9

	Event
		EName 	hand
		Type	local
		At	WepL
		Part1	CustomizeablePowers\ForceField\HandGlow.part
		Lifespan 30
	End

	Event
		EName 	hand
		Type	local
		At	WepR
		Part1	CustomizeablePowers\ForceField\HandGlow.part
		Lifespan 30
	End
End

Condition
	On 	Time
	Time 	15

	Event
		EName 	hand2
		Type	local
		At	WepL
		Bhvr	CustomizeablePowers\ForceField\ForceBubbleHands.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	tintable_FX_ForceBubble
		Lifespan 7
	End

	Event
		EName 	hand2
		Type	local
		At	WepR
		Bhvr	CustomizeablePowers\ForceField\ForceBubbleHands.bhvr
		Lifespan 7
		BhvrOverride
			TintGeom	1
		End
		Geom	tintable_FX_ForceBubble
	End
End

#############################################################

Condition
	On 	Time
	Time 	20

	Event
		EName 	Prime
		Type	start
		At	Mystic
		Bhvr	CustomizeablePowers\ForceField\projectileNOspin.bhvr
		BhvrOverride
			TintGeom	1
		End
		Part1	CustomizeablePowers\ForceField\ForceMist01.part
		Part2	CustomizeablePowers\ForceField\ForceBallTrail.part
		Geom	Tintable_ForceBolt
		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	Prime2
		Type	start
		At	Mystic
		Bhvr	CustomizeablePowers\ForceField\projectileSLIGHTspin.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_ForceBoltTail
		Magnet	Target
		LookAt	Target
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		Type    Destroy
		EName 	Prime
	End

	Event
		Type    Destroy
		EName 	Prime2
	End
End

#############################################################

End