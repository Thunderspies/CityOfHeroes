#
FxInfo

	LifeSpan 90


###########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	PandoraBeam_Hit_01 100 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part ENEMYFX\Nemesis\NemesisMonstrocity\PandoraHit_Blobs.part
			Part ENEMYFX\Nemesis\NemesisMonstrocity\PandoraHit_Smoke.part
			Part ENEMYFX\Nemesis\NemesisMonstrocity\PandoraHit_Charge.part
		End

	End

End
