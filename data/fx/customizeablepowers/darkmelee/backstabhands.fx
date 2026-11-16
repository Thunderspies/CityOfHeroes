#############################################################
## HEADER
#############################################################

FxInfo

Lifespan 100

#############################################################

Condition
	On	Time
	Time	11

	Event
		Type	local
		At	root
		BhvrOverride
			PositionOffset		0 3 5
		End
		Part1	CustomizeablePowers\DarkMelee\SkullHandsA.part
		Part2	CustomizeablePowers\DarkMelee\SkullHandsB.part
		Part3	CustomizeablePowers\DarkMelee\SkullEmberb.part
		Part1	CustomizeablePowers\DarkMelee\SkullHand.part
		Part3	CustomizeablePowers\DarkMelee\SkullHands.part
		LifeSpan 12
	End
End

Condition
	On	Time
	Time	12

	Event
		EName	Prime
		Type	local
		At	root
		Geom	Skull
		Bhvr	CustomizeablePowers\DarkMelee\GenericParticleFadeSkull.bhvr
		BhvrOverride
			PositionOffset		0 3 5
		End
		Lifespan 1
	End

	Event
		EName	eye1
		Type	local
		At	Prime
		Altpiv	2
		Part1	CustomizeablePowers\DarkMelee\RedEyes.part
		Part2	CustomizeablePowers\DarkMelee\WhitePupil.part
		Lifespan 4
	End

	Event
		EName	eye2
		Type	local
		At	Prime
		Altpiv	3
		Part1	CustomizeablePowers\DarkMelee\RedEyes.part
		Part2	CustomizeablePowers\DarkMelee\WhitePupil.part
		Lifespan 4
	End
End

#############################################################

Condition
	Time 	15
	Chance	.4
	Repeat	2

	Event
		Type	start
		At	Prime
		Bhvr	CustomizeablePowers\DarkMelee\StartOffset.bhvr
		BhvrOverride
			InitialVelocity		.1 .5 0
			InitialVelocityJitter	.6 .1 .6
			Drag			.1
			Physics 		1
			PhysRadius 		.4
			PhysGravity 		1
			physRestitution 	0.01
			physSFriction 	 	.9
			physDFriction 	 	.9
		End
		Part	CustomizeablePowers\DarkMelee\Skulltendril01.part
		Part	CustomizeablePowers\DarkMelee\Skulltendril02.part
		Part	CustomizeablePowers\DarkMelee\SkullCoreSplata.part
		CEvent 	:BackStabPhysicHits.fx
		CThresh 	.01
		CDestroy 	1
		LifeSpan			40
		LifeSpanJitter			10
	End
End

#############################################################

Condition
	Time 	15
	Chance	.4
	Repeat	2

	Event
		Type	start
		At	Prime
		Bhvr	CustomizeablePowers\DarkMelee\StartOffset.bhvr
		BhvrOverride
			InitialVelocity		-.1 .6 0
			InitialVelocityJitter	.6 .1 .6
			Drag			.1
			Physics 		1
			PhysRadius 		.4
			PhysGravity 		1
			physRestitution 	0.01
			physSFriction 	 	.9
			physDFriction 	 	.9
		End
		Part	CustomizeablePowers\DarkMelee\Skulltendril01.part
		Part	CustomizeablePowers\DarkMelee\Skulltendril02.part
		Part	CustomizeablePowers\DarkMelee\SkullCoreSplat.part
		CEvent 	:BackStabPhysicHits.fx
		CThresh 	.01
		CDestroy 	1
		LifeSpan			40
		LifeSpanJitter			10
	End
End

#############################################################

Condition
	Time 	15
	Chance	.4
	Repeat	2

	Event
		Type	start
		At	Prime
		Bhvr	CustomizeablePowers\DarkMelee\StartOffset.bhvr
		BhvrOverride
			InitialVelocity		 0 .4 -.1
			InitialVelocityJitter	.6 .1 .6
			Drag			.1
			Physics 		1
			PhysRadius 		.4
			PhysGravity 		1
			physRestitution 	0.01
			physSFriction 	 	.9
			physDFriction 	 	.9
		End
		Part	CustomizeablePowers\DarkMelee\Skulltendril01.part
		Part	CustomizeablePowers\DarkMelee\Skulltendril02.part
		Part	CustomizeablePowers\DarkMelee\SkullCoreSplat.part
		CEvent 	:BackStabPhysicHits.fx
		CThresh 	.01
		CDestroy 	1
		LifeSpan			40
		LifeSpanJitter			10
	End
End

#############################################################

Condition
	Time 	15
	Chance	.4
	Repeat	2

	Event
		Type	start
		At	Prime
		Bhvr	CustomizeablePowers\DarkMelee\StartOffset.bhvr
		BhvrOverride
			InitialVelocity		0 .4 .1
			InitialVelocityJitter	.6 .1 .6
			Drag			.1
			#PositionOffset		4 1 2
			Physics 		1
			PhysRadius 		.4
			PhysGravity 		1
			physRestitution 	0.01
			physSFriction 	 	.9
			physDFriction 	 	.9
		End
		Part	CustomizeablePowers\DarkMelee\Skulltendril01.part
		Part	CustomizeablePowers\DarkMelee\Skulltendril02.part
		Part	CustomizeablePowers\DarkMelee\SkullCoreSplat.part
		CEvent 	:BackStabPhysicHits.fx
		CThresh 	.01
		CDestroy 	1
		LifeSpan			40
		LifeSpanJitter			10
	End
End

###################################################################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	KickFlash
		Type	Local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\DarkMelee\SmokeHands.part
		part2	CustomizeablePowers\DarkMelee\SmokeHands2.part
		Part3	CustomizeablePowers\DarkMelee\DarkBlastHitTendril.part
		Part1	CustomizeablePowers\DarkMelee\DarkHandsA.part
		Part2	CustomizeablePowers\DarkMelee\DarkHandsB.part
		Part3	CustomizeablePowers\DarkMelee\DarkHandsEmberb.part
		Part1	CustomizeablePowers\DarkMelee\DarkHand.part
		Part3	CustomizeablePowers\DarkMelee\DarkHands.part
		LifeSpan 55
	End

	Event
		ENAME	KickFlash
		Type	Local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\DarkMelee\SmokeHands.part
		part2	CustomizeablePowers\DarkMelee\SmokeHands2.part
		Part3	CustomizeablePowers\DarkMelee\DarkBlastHitTendril.part
		Part1	CustomizeablePowers\DarkMelee\DarkHandsA.part
		Part2	CustomizeablePowers\DarkMelee\DarkHandsB.part
		Part3	CustomizeablePowers\DarkMelee\DarkHandsEmberb.part
		Part1	CustomizeablePowers\DarkMelee\DarkHand.part
		Part3	CustomizeablePowers\DarkMelee\DarkHands.part
		LifeSpan 55
	End
End

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	WepR
		Sound Darkmoonbeam3b 80 80 0.75
		LifeSpan 55
	End
End

#############################################################

End