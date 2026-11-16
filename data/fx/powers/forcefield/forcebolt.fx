#############################################################
## ForceBolt.fx
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
		Sound forcebolthit 120 120 0.7
	End
End

Condition
	On 	Time
	Time 	9

	Event
		EName 	hand
		Type	local
		At	WepL
		Part1	POWERS\Forcefield\HandGlow.part
		LifeSpan 30
	End

	Event
		EName 	hand
		Type	local
		At	WepR
		Part1	POWERS\Forcefield\HandGlow.part
		LifeSpan 30
	End
End

Condition
	On 	Time
	Time 	15

	Event
		EName 	hand2
		Type	local
		At	WepL
		Geom	FX_ForceBubble
		Bhvr 	behaviors/ForceBubbleHands.bhvr
		LifeSpan 7
	End

	Event
		EName 	hand2
		Type	local
		At	WepR
		Geom	FX_ForceBubble
		Bhvr 	behaviors/ForceBubbleHands.bhvr
		LifeSpan 7
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
		Bhvr	behaviors\projectileNOspin.bhvr
		Geom	ForceBolt
		Magnet	Target
		LookAt	Target
		Part1	POWERS\Forcefield\ForceMist01.part
		Part2	POWERS\Forcefield\ForceBallTrail.part
	End

	Event
		EName 	Prime2
		Type	start
		At	Mystic
		Bhvr	behaviors/projectileSLIGHTspin.bhvr
		Geom	ForceBoltTail
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