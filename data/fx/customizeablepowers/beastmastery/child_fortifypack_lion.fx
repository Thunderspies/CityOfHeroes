###########################################################

FxInfo
Flags DontSuppress
#Lifespan 50

###########################################################

Condition
	On	Time
	Time	0

	Event
			At 	Hind_ToeL
			Type 	Local
			POther 	Hind_FootL
			Bhvr	behaviors/GenericParticleFade.bhvr
			Part 	:FortifyPack_Glow_Small.part
			Lifespan 20
		End

		Event
			At 	Hind_ToeR
			Type 	Local
			POther 	Hind_FootR
			Bhvr	behaviors/GenericParticleFade.bhvr
			Part 	:FortifyPack_Glow_Small.part
			Lifespan 20
		End

		Event
			At 	Fore_LlegR
			Type 	Local
			POther 	Fore_FootR
			Bhvr	behaviors/GenericParticleFade.bhvr
			Part 	:FortifyPack_Glow_Small.part
			Lifespan 20
		End

		Event
			At 	Fore_LlegL
			Type 	Local
			POther 	Fore_FootL
			Bhvr	behaviors/GenericParticleFade.bhvr
			Part 	:FortifyPack_Glow_Small.part
			Lifespan 20
		End

		Event
			At Hind_FootR
			Type Local
			POther Hind_LLegR
			Bhvr	behaviors/GenericParticleFade.bhvr
			Part 	:FortifyPack_Glow_Small.part
			Lifespan 20
		End

		Event
			At Hind_FootL
			Type Local
			POther Hind_LLegL
			Bhvr	behaviors/GenericParticleFade.bhvr
			Part 	:FortifyPack_Glow_Small.part
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
			Part 	:FortifyPack_Glow_Small.part
			Lifespan 20
		End

		Event
			At Fore_UlegL
			Type Local
			POther Fore_LlegL
			Bhvr	behaviors/GenericParticleFade.bhvr
			Part 	:FortifyPack_Glow_Small.part
			Lifespan 20
		End
End

Condition
	On	Time
	Time	20

		Event
			At Chest
			Type Local
			POther Hips
			Bhvr	behaviors/GenericParticleFade.bhvr
			Part :FortifyPack_Glow.part
			Lifespan 20
		End

		Event
			At Chest
			Type Local
			Bhvr	behaviors/GenericParticleFade.bhvr
			Part 	:FortifyPack_Glow.part
			Lifespan 20
		End
End

Condition
	On	Time
	Time	27
		Event
			At Head
			Type Local
			POther Neck
			Bhvr	behaviors/GenericParticleFade.bhvr
			Part 	:FortifyPack_Glow_Head.part
			Lifespan 20
		End

		Event
			At T1_L
			Type Local
			EName Tail
			Bhvr	behaviors/GenericParticleFade.bhvr
			Part 	:FortifyPack_Glow_Tail.part
			POther T1_R
			Lifespan 10
		End
End

Condition
	On	Time
	Time	36

		Event
			At T1_R
			Type Local
			EName Tail
			Bhvr	behaviors/GenericParticleFade.bhvr
			Part 	:FortifyPack_Glow_Tail.part
			POther T2_R
			Lifespan 10
		End
End

Condition
	On	Time
	Time	42



		Event
			At T2_R
			Type Local
			EName Tail
			Bhvr	behaviors/GenericParticleFade.bhvr
			Part 	:FortifyPack_Glow_Tail.part
			POther T3_R
			Lifespan 10
		End

End