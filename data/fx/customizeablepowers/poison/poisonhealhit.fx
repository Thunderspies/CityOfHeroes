#############################################################
## PoisonHealHit.fx
#############################################################

FxInfo

Lifespan 150

#############################################################

Condition
	On	Time
	Time	20

	Event
		Type	local
		At	origin
		Sound alkaloidtoss 90 90 0.85
	End
End

Condition
	on	Time
	Time	30

	Event
		Type	local
		At	origin
		Sound Poisongasressurect 90 90 0.85
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName 	cylinderspawn
		Type	local
		At	T_Root
		BHVR	CustomizeablePowers\Poison\PoisonHealRotate.bhvr
		Geom	CenterDummy
	End

	Event
		EName	RotateNode
		Type	local
		At	cylinderspawn
		altpiv	1
		BhvrOverride
			PositionOffset		0 0 3.5
		End
	End
End

Condition
	On 	Time
	Time	35

	Event
		Type	local
		At	RotateNode
		ChildFX	CustomizeablePowers\Poison\Child_PoisonHealHit.fx
		Lifespan 15
	End
End

#############################################################

Condition
	On	Time
	Time	20

	Event
		Type	local
		At	WepR
		Bhvr	CustomizeablePowers\Poison\YellowGlassScaleDown.bhvr
		Geom	PoisonGlassBall
		Lifespan 28
	End
End

#############################################################

Condition
	On	Time
	Time	45

	Event
		Type	local
		At	T_Root
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\Poison\HealPoisonglowsUp.part
		Part	CustomizeablePowers\Poison\HealPoisonUp.part
		Part	CustomizeablePowers\Poison\HealPoisonSpecksUp.part
		Part	CustomizeablePowers\Poison\HealPoisonSpecksUpYellow.part
		Part	CustomizeablePowers\Poison\HealPoisonBubblesUp.part
		Part	CustomizeablePowers\Poison\HealPoisonglowsUp.part
		Part	CustomizeablePowers\Poison\HealPoisonUpDark.part
		Lifespan 80
	End

	Event
		Type	local
		At	T_Root
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\Poison\HealEnergyBase.part
		Part	CustomizeablePowers\Poison\HealEnergyBaseFlat.part
		Lifespan 70
	End
End

#############################################################

Condition
	On 	Time
	Time	32

	Event
		EName 	sleeticesplatSpinsAndOffsets
		Type	start
		At	T_Root
		BHVR	CustomizeablePowers\Poison\PoisonSplatRotate.bhvr
		Geom	CenterDummy
		Lifespan 50
	End

	Event
		EName 	sleeticesplatSpinsAndOffsets2
		Type	start
		At	T_Root
		BHVR	CustomizeablePowers\Poison\PoisonSplatRotate.bhvr
		Geom	CenterDummy
		Lifespan 50
	End

	Event
		EName 	sleeticesplatSpinsAndOffsets3
		Type	start
		At	T_Root
		BHVR	CustomizeablePowers\Poison\PoisonSplatRotate.bhvr
		Geom	CenterDummy
		Lifespan 50
	End

	Event
		EName 	sleeticesplatSpinsAndOffsets4
		Type	start
		At	T_Root
		BHVR	CustomizeablePowers\Poison\PoisonSplatRotate.bhvr
		Geom	CenterDummy
		Lifespan 50
	End
End

#############################################################

Condition
	On 	Time
	Time	44

	Event
		EName 	sleeticesplat
		Type	start
		At	sleeticesplatSpinsAndOffsets
		Altpiv	1
		BHVR	CustomizeablePowers\Poison\PoisonHealSplat.bhvr
		Splat	ember.tga
		Lifespan 48
	End
End

Condition
	On 	Time
	Time	45

	Event
		EName 	sleeticesplat
		Type	start
		At	sleeticesplatSpinsAndOffsets2
		Altpiv	1
		BHVR	CustomizeablePowers\Poison\PoisonHealSplat.bhvr
		Splat	ember.tga
		Lifespan 48
	End
End

Condition
	On 	Time
	Time	46

	Event
		EName 	sleeticesplat
		Type	start
		At	sleeticesplatSpinsAndOffsets4
		Altpiv	1
		BHVR	CustomizeablePowers\Poison\PoisonHealSplat.bhvr
		Splat	ember.tga
		Lifespan 48
	End
End

Condition
	On 	Time
	Time	47

	Event
		EName 	sleeticesplat
		Type	start
		At	sleeticesplatSpinsAndOffsets3
		Altpiv	1
		BHVR	CustomizeablePowers\Poison\PoisonHealSplat.bhvr
		Splat	ember.tga
		Lifespan 48
	End
End

#############################################################

End