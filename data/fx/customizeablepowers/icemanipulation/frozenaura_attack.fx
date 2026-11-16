#############################################################
## FrozenAura_Attack.fx
#############################################################

FxInfo

lifespan 150

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	UlegR
		Sound IceNova2 100.0 100.0 .88
		Part1	CustomizeablePowers\IceManipulation\ColdArmMist.part
		Part2	CustomizeablePowers\IceManipulation\FrostHands.part
		Part3	CustomizeablePowers\IceManipulation\snowburst.part
		LifeSpan 30
	End

	Event
		Type	Local
		At	LlegR
		Part1	CustomizeablePowers\IceManipulation\ColdArmMist.part
		Part2	CustomizeablePowers\IceManipulation\FrostHands.part
		Part3	CustomizeablePowers\IceManipulation\snowburst.part
		LifeSpan 30
	End

	Event
		Type	Local
		At	FootR
		Part1	CustomizeablePowers\IceManipulation\ColdArmMist.part
		Part2	CustomizeablePowers\IceManipulation\FrostHands.part
		Part3	CustomizeablePowers\IceManipulation\snowburst.part
		LifeSpan 30
	End
End

#############################################################

Condition
	On 	Time
	Time 	25

	Event
		EName 	Prime
		Type	Local
		At	mystic
		Bhvr	Behaviors\CameraShake01.bhvr
		Lifespan 42
	End

	Event
		Type	Local
		At	mystic
		bhvr	CustomizeablePowers\IceManipulation\Infernobubble.bhvr
		Part	CustomizeablePowers\IceManipulation\ColdMistAura.part
		Part	CustomizeablePowers\IceManipulation\swordFlame01.part
		Part	CustomizeablePowers\IceManipulation\SwordFlame02.part
		Part	CustomizeablePowers\IceManipulation\IceCrystals.part
		Part	CustomizeablePowers\IceManipulation\IceCrystals2.part
		Part	CustomizeablePowers\IceManipulation\SnowFlakes.part
	End

	Event
		Type	Local
		At	mystic
		bhvr	CustomizeablePowers\IceManipulation\Infernobubble2.bhvr
		Part1	CustomizeablePowers\IceManipulation\IceAuraSnow.part
	End

	Event
		Type	Local
		At	mystic
		bhvr	CustomizeablePowers\IceManipulation\Infernobubble3.bhvr
		Part2	CustomizeablePowers\IceManipulation\IceAuraBits.part
	End

	Event
		Type	start
		At	mystic
		bhvr	CustomizeablePowers\IceManipulation\IceAuraRing.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	tintable_MegaRing2
		LifeSpan 3
	End
End

#############################################################

End