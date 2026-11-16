#############################################################
## PodPerson.fx
#############################################################

FxInfo
Lifespan 250

#############################################################

Condition
	On	Time
	Time	15

	Event
		Type Local
		At Origin
		Sound Mutant_EnergyMorph_01 200 200 .8
	End

	Event
		Type	Start
		At	Hips
		Bhvr 	behaviors/powers/forcefield/ForceBubble.bhvr
		BhvrOverride
			Alpha		255
			StartColor	100 200 255
			PyrRotateJitter	0.0 0.0 20.0
			FadeInLength	20
			FadeOutLength	20
			Spin 		0 -.15 0
			SpinJitter	0 .025 0
			EndScale	1.3 1.3 1.3

		End
		Geom	Tintable_EarthGraspHit
		LifeSpan	35
	End

	Event
		Type	Start
		At	Hips
		Part	:Glow_Spiked.part
		Part	:Glow_Round.part
		LifeSpan 20
	End

	Event
		Type	Start
		At	Hips
		Bhvr 	behaviors/powers/forcefield/ForceBubble.bhvr
		BhvrOverride
			Alpha		255
			StartColor	50 240 255
			FadeInLength	20
			FadeOutLength	20
			Spin 		0 .15 0
			SpinJitter	0 .025 0
			EndScale	1.3 1.3 1.3
		End
		Geom	Tintable_EarthGraspHit
		LifeSpan	30
	End

#############################################################

	Event
		EName 	Prime
		Type	SetLight
	#	BHVR	:Body_FadeToBlack.bhvr
		Flags	AdoptParentEntity
		LifeSpan 400
	End

#############################################################

	Event
		Type	StartPositOnly
		At	Hips
		BhvrOverride
			PositionOffset	0.0 -1.2 0.0
			PyrRotate	0.0 22.0 0.0
		End
		Part	:Glow_Round_Flat.part
		Part	:Glow_Fill_Flat.part
		Part	:Glow_Spiked_Flat.part
		LifeSpan 60
	End


End

Condition
	On	Time
	Time	30

	Event
		Type	Start
		At	Hips
		ChildFX	:BodyGlow.fx
		LifeSpan 20
	End
End

Condition
	On	Time
	Time	50

	Event
		Type	StartPositOnly
		At	Root
		bhvr	behaviors\Crater.bhvr
		BhvrOverride
			StartColor	75 75 75
			PositionOffset	0.0 0.5 0.0
			PyrRotate	0 15 0
		End
		Geom	FX_HellFissure
		Lifespan 100

	End

	Event
		Type	StartPositOnly
		At	Root
		Bhvr	Emotes\CostumeChanges\Stomp\Splat_FootStomp.bhvr
		BhvrOverride
			Scale		16.5 1.5 16.5
			EndScale	16.5 1.5 16.5
			PyrRotate	0 50 0
			PyrRotateJitter	0 0 0
			FadeInLength		10
			FadeOutLength		60
			Alpha		200
			StartColor	50 50 50
		End
		Splat	Ruin_Street_PStains_04.tga
		Lifespan 	90
	End

	Event
		EName 	Prime
		Type	Start
		At	Hips
		Part	:Burst_Ring.part
		Part	:Burst_Ring_Lingering.part
		Part	:Burst_Ring_Highlight.part

		Part	:Burst_Flash.part
		LifeSpan 5
	End



	Event
		Type	StartPositOnly
		At	Root
		Bhvr	behaviors/CameraShake02.bhvr
		BhvrOverride
			PositionOffset	0.0 -1.0 0.0
		End
		Part	:Dust_Circle.part
		Part	:Dust_Circle_Lingering.part
		Part	:Explosion_Rocks.part
		Lifespan 5

	End
End

#########################################################

End