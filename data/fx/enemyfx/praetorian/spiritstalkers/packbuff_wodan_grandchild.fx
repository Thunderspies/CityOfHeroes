#
FxInfo

	Flags DontSuppress


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type Local
			POther Hips
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :FortifyPack_Glow2_XL.part
			LifeSpan 20
		End

	End

	Condition
		On Time
		Time 7

		Event
			At Head
			Type Local
			POther Neck
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :FortifyPack_Glow_Head_XL.part
			LifeSpan 15
		End

		Event
			At T1_R
			Type Local
			EName Tail
			POther T2_R
			Bhvr behaviors/GenericParticleFade.bhvr
			#Part :FortifyPack_Glow_Tail_Wolf_Big.part
			LifeSpan 15
		End

	End

	Condition
		On Time
		Time 13

		Event
			At Fore_ULegR
			Type Local
			POther Fore_LlegR
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :FortifyPack_Glow_XL.part
			LifeSpan 20
		End

		Event
			At Fore_ULegL
			Type Local
			POther Fore_LlegL
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :FortifyPack_Glow_XL.part
			LifeSpan 20
		End

	End

	Condition
		On Time
		Time 20

		Event
			At Hind_ToeL
			Type Local
			POther Hind_FootL
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :FortifyPack_Glow_XL.part
			LifeSpan 20
		End

		Event
			At Hind_ToeR
			Type Local
			POther Hind_FootR
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :FortifyPack_Glow_XL.part
			LifeSpan 20
		End

		Event
			At Fore_LlegR
			Type Local
			POther Fore_FootR
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :FortifyPack_Glow_XL.part
			LifeSpan 20
		End

		Event
			At Fore_LlegL
			Type Local
			POther Fore_FootL
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :FortifyPack_Glow_XL.part
			LifeSpan 20
		End

		Event
			At Hind_FootR
			Type Local
			POther Hind_LLegR
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :FortifyPack_Glow_XL.part
			LifeSpan 20
		End

		Event
			At Hind_FootL
			Type Local
			POther Hind_LLegL
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :FortifyPack_Glow_XL.part
			LifeSpan 20
		End

	End

	Condition
		On Time
		Time 31

		Event
			At T3_R
			Type Local
			EName Tail
			POther T1_L
			Bhvr behaviors/GenericParticleFade.bhvr
			#Part :FortifyPack_Glow_Tail_Wolf_Big.part
			LifeSpan 10
		End

	End

End
