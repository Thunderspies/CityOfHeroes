#                                                          
FxInfo

	Input
		InpName Hips
	End

	LifeSpan 30


###########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	WB_SteamHit 100 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type Local
			EName Prime
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				StartJitter 0.3 0.5 0.3
			End
			#Part :Cryonic_Hit_Mist_Large.part
			#Part :Cryonic_Hit_Flash_Large.part
			#Part :Cryonic_Hit_Shockwave_Large.part
			Part :Steam_Hit_Mist_Bright.part
			LifeSpan 10
		End

		Event
			At Hair
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Steam_Hand.part
			Part :Steam_Hand_Detached.part
		End

		Event
			At Chest
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Steam_Hand.part
			Part :Steam_Hand_Detached.part
		End

		Event
			At UArmL
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Steam_Hand.part
			Part :Steam_Hand_Detached.part
		End

		Event
			At LArmL
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Steam_Hand.part
			Part :Steam_Hand_Detached.part
		End

		Event
			At UArmR
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Steam_Hand.part
			Part :Steam_Hand_Detached.part
		End

		Event
			At LArmR
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Steam_Hand.part
			Part :Steam_Hand_Detached.part
		End

	End

	Condition
		On Time
		Time 6

		Event
			At Chest
			Type Local
			EName Prime
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				StartJitter 0.3 0.5 0.3
			End
			Part :Steam_Hit_Mist.part
			Part :Steam_Hit_Flash.part
			Part :Steam_Hit_Shockwave.part
			LifeSpan 10
		End

	End

End
