###########################################################

FxInfo
Flags DontSuppress

###########################################################

Condition
	On	Time
	Time	0



		Event
			At Hind_FootR
			Type Local
			POther Hind_LLegR
			Bhvr	behaviors/GenericParticleFade.bhvr
			Part 	:Mist_Cold.part
			Part 	:Mist_Cold_Detached.part
		#	Part 	:Fire_Cold_Body.part
		End

		Event
			At Hind_FootL
			Type Local
			POther Hind_LLegL
			Bhvr	behaviors/GenericParticleFade.bhvr
			Part 	:Mist_Cold.part
			Part 	:Mist_Cold_Detached.part
		#	Part 	:Fire_Cold_Body.part
		End

		Event
			At Fore_UlegR
			Type Local
			POther Fore_LlegR
			Bhvr	behaviors/GenericParticleFade.bhvr
			Part 	:Mist_Cold.part
			Part 	:Mist_Cold_Detached.part
		End

		Event
			At Fore_UlegL
			Type Local
			POther Fore_LlegL
			Bhvr	behaviors/GenericParticleFade.bhvr
			Part 	:Mist_Cold.part
			Part 	:Mist_Cold_Detached.part
		#	Part 	:Fire_Cold_Body.part
		End

		Event
			At Chest
			Type Local
			POther Hips
			Bhvr	behaviors/GenericParticleFade.bhvr
		#	Part 	:Mist_Cold_Large.part
		#	Part 	:Mist_Cold_Detached_Large.part
			Part 	:Fire_Cold_Body.part
		End

		Event
			At Head
			Type Local
			POther Neck
			Bhvr	behaviors/GenericParticleFade.bhvr
		#	Part 	:Mist_Cold_Large.part
		#	Part 	:Mist_Cold_Detached_Large.part
			Part 	:Fire_Cold_Body.part
		End

		Event
			At T1_R
			Type Local
			EName Tail
			Bhvr	behaviors/GenericParticleFade.bhvr
			Part 	:Mist_Cold.part
			Part 	:Mist_Cold_Detached.part
			Part 	:Fire_Cold_Body.part
			POther T2_R
		End

		Event
			At T3_R
			Type Local
			EName Tail
			Bhvr	behaviors/GenericParticleFade.bhvr
			Part 	:Mist_Cold.part
			Part 	:Mist_Cold_Detached.part
			Part 	:Fire_Cold_Body.part
			POther T1_L
		End
End

###########################################################

End