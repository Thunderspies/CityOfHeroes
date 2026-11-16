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
		BHVR	:Body_FadeToBlack.bhvr
		Flags	AdoptParentEntity
		LifeSpan 400
	End

#############################################################

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
		EName 	Prime
		Type	Start
		At	Hips
		Part	:Burst_Ring.part
		Part	:Burst_Ring_Lingering.part
		Part	:Burst_Ring_Highlight.part

		Part	:Burst_Flash.part
		LifeSpan 5
	End
End

#########################################################

End