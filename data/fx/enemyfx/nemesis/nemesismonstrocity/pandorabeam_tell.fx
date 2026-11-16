#
FxInfo

	LifeSpan 120


###########################################################
Condition
	On 	Time
	Time	30
	Event
		Type	Local
		At	Root
		Sound 	PandoraBeam_Tell_01 100 100 1
	End
End

	Condition
		On Time
		Time 60

		Event
			At Hips
			Type Local
			EName Shaker
			Bhvr behaviors/CameraShake_Subtle90.bhvr
			BhvrOverride
				Shake 0.3
			End
		End

		Event
			At Head
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part ENEMYFX\Nemesis\NemesisMonstrocity\Pandora_PowerUp_Streaks.part
			Part :Pandora_PowerUp.part
		End

	End

	Condition
		On Time
		Time 50

		Event
			At Head
			Type Local
			Bhvr :BeamTell_FadeIn.bhvr
			BhvrOverride
				PositionOffset 0 0.1 -1
				EndScale 1.9 2.8 8
				StartColor 255 225 80
			End
			Geom FX_GenericPowerCone
			LifeSpan 1
		End

	End

End
