###########################################################

FxInfo

###########################################################

Condition
	On	Time
	Time	0
		Event
			Type	Local
			At	Origin
			Bhvr	behaviors/GenericParticleFade.bhvr
			ChildFX	:Child_SteamJet_DireWolf.fx
			Lifespan 	30
		End
End

Condition
	On	Cycle
	Time	60

	Event
		Type	Local
		At	Origin
		Bhvr	behaviors/GenericParticleFade.bhvr
			ChildFX	:Child_SteamJet_DireWolf.fx
		Lifespan 	30

	End
End

Condition
	ON	Time
	Time	9

		Event
			At Head
			Type Local
			Bhvr	behaviors/GenericParticleFade.bhvr
			Part	:TrainBeasts_Core_Head.part
			Part	:TrainBeasts_Glow_Head.part
			Part	:TrainBeasts_Ring_Head.part
			Lifespan 10
		End
End

###########################################################

End
