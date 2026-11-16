###########################################################

FxInfo
Lifespan 60

###########################################################

Condition
	On 	Time
	Time	10
	Event
		Type	Local
		At	Root
		Sound 	BM_WildCharge_Attack_01 100 100 .4
	End
End

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
#		Sound 	DD_EnervatingStrike_Hit_01 100 100 .8
	End

#	Event
#		Type	Local
#		At	Root
#		BhvrOverride
#			PositionOffset	0.0 2.0 -15.0
#			PYRRotate	90 0 90
#		End
#		Part	:Air_Dust_Traveling.part
#	#	Part	:Air_Dust_Traveling_Additive.part
#		Lifespan 3
#	End
#


	Event
		At 	Hind_ToeL
		Type 	Local
		POther 	Hind_FootL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part 	:WildCharge_Ripples.part
		Part	:WildCharge_Glow.part
		Lifespan 20
	End

	Event
		At 	Hind_ToeR
		Type 	Local
		POther 	Hind_FootR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part 	:WildCharge_Ripples.part
		Part	:WildCharge_Glow.part
		Lifespan 20
	End

	Event
		At 	Fore_LlegR
		Type 	Local
		POther 	Fore_FootR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part 	:WildCharge_Ripples.part
		Part	:WildCharge_Glow.part
		Lifespan 20
	End

	Event
		At 	Fore_LlegL
		Type 	Local
		POther 	Fore_FootL
		Bhvr	behaviors/GenericParticleFade.bhvr
			Part 	:WildCharge_Ripples.part
			Part	:WildCharge_Glow.part
		Lifespan 20
	End

	Event
		At Hind_FootR
		Type Local
		POther Hind_LLegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part 	:WildCharge_Ripples.part
		Part	:WildCharge_Glow.part
		Lifespan 20
	End

	Event
		At Hind_FootL
		Type Local
		POther Hind_LLegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part 	:WildCharge_Ripples.part
		Part	:WildCharge_Glow.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	13

		Event
			At Fore_UlegR
			Type Local
			POther Fore_LlegR
			Bhvr	behaviors/GenericParticleFade.bhvr
			Part 	:WildCharge_Ripples.part
			Part	:WildCharge_Glow.part
			Lifespan 20
		End

		Event
			At Fore_UlegL
			Type Local
			POther Fore_LlegL
			Bhvr	behaviors/GenericParticleFade.bhvr
			Part 	:WildCharge_Ripples.part
			Part	:WildCharge_Glow.part
			Lifespan 20
		End

		Event
			At Hind_UlegR
			Type Local
			POther HInd_LlegR
			Bhvr	behaviors/GenericParticleFade.bhvr
			Part 	:WildCharge_Ripples.part
			Part	:WildCharge_Glow.part
			Lifespan 20
		End

		Event
			At Hind_UlegL
			Type Local
			POther HInd_LlegL
			Bhvr	behaviors/GenericParticleFade.bhvr
			Part 	:WildCharge_Ripples.part
			Part	:WildCharge_Glow.part
			Lifespan 20
		End
End

Condition
	On	Time
	Time	16

		Event
			At Chest
			Type Local
			Bhvr	behaviors/GenericParticleFade.bhvr
			Part 	:WildCharge_Ripples_Chest.part
			Part	:WildCharge_Glow_Chest.part
			Lifespan 20
		End

		Event
			At Hips
			Type Local
			Bhvr	behaviors/GenericParticleFade.bhvr
			Part 	:WildCharge_Ripples_Chest.part
			Part	:WildCharge_Glow_Chest.part
			Lifespan 20
		End
	End

Condition
	On	Time
	Time	19

		Event
			At T2_R
			Type Local
			EName Tail
			Bhvr	behaviors/GenericParticleFade.bhvr
			Part 	:WildCharge_Ripples.part
			Part	:WildCharge_Glow.part
			POther T3_R
			Lifespan 20
		End
End

Condition
	On	Time
	Time	22

		Event
			At Head
			Type Local
			Bhvr	behaviors/GenericParticleFade.bhvr
			Part 	:WildCharge_Ripples_Head.part
			Part 	:WildCharge_Ripples2_Head.part
			Lifespan 10
		End

		Event
			At Jaw
			Type Local
			Bhvr	behaviors/GenericParticleFade.bhvr
			Part	:WildCharge_Glow_Head.part
			Lifespan 10
		End

		Event
			At T1_L
			Type Local
			EName Tail
			Bhvr	behaviors/GenericParticleFade.bhvr
			Part 	:WildCharge_Ripples.part
			Part	:WildCharge_Glow.part
			POther T2_R
			Lifespan 20
		End

End


End
