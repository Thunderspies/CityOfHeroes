###########################################################

FxInfo
Lifespan 60

###########################################################


Condition
	On 	Time
	Time	26
	Event
		Type	Local
		At	Root
		Sound 	BM_FortifyPack_Attack_01 100 100 .8
	End
End


	Condition
		On Time
		Time 26

		Event
			At UArmL
			Type Local
			POther LarmL
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :TrainerCharge_Ripples.part
			Part :TrainerCharge_Glow.part
			LifeSpan 50
		End

		Event
			At UArmR
			Type Local
			POther LarmR
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :TrainerCharge_Ripples.part
			Part :TrainerCharge_Glow.part
			LifeSpan 50
		End

		Event
			At LArmR
			Type Local
			POther WepR
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :TrainerCharge_Ripples.part
			Part :TrainerCharge_Glow.part
			LifeSpan 50
		End

		Event
			At LArmL
			Type Local
			POther WepL
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :TrainerCharge_Ripples.part
			Part :TrainerCharge_Glow.part
			LifeSpan 50
		End

		Event
			At WepR
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :TrainerCharge_Ripples.part
			Part :TrainerCharge_Glow_Head.part
			Part :TrainerCharge_Ripples_Chest.part
			Part :TrainerCharge_Ripples_Head.part
			LifeSpan 50
		End

		Event
			At WepL
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :TrainerCharge_Ripples.part
			Part :TrainerCharge_Glow_Head.part
			Part :TrainerCharge_Ripples_Chest.part
			Part :TrainerCharge_Ripples_Head.part
			LifeSpan 50
		End

	End