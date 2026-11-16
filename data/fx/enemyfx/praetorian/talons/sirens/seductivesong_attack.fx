#
FxInfo

	LifeSpan 250


###########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	TOV_Siren_SeductiveSong_Attack 100 100 .8
	End
End

	Condition
		On Time
		Time 20

		Event
			At Head
			Type Local
			Part ENEMYFX\Praetorian\Talons\Sirens\AOE_EnergyDonut.part
			LifeSpan 60
		End

		Event
			At Chest
			Type PositOnly
			EName AnchorAnchor
		End

		Event
			At AnchorAnchor
			Type Local
			EName Anchor
			BhvrOverride
				PyrRotateJitter 10 0 10
				Scale 0.2 0.6 0.2
				Spin 0 -0.1 0
				StartJitter 1 1 1
			End
		End

		Event
			At AnchorAnchor
			Type Local
			EName Anchor2
			BhvrOverride
				PyrRotateJitter 10 0 10
				Scale 0.2 0.6 0.2
				Spin 0 -0.1 0
				StartJitter 1 1 1
			End
		End

	End

	Condition
		On Time
		Time 31

		Event
			At Anchor
			Type Local
			Bhvr ENEMYFX\Praetorian\Talons\Sirens\SeductiveMusic_Grow.bhvr
			BhvrOverride
				PositionOffset 0 -1 0
				PyrRotateJitter 10 90 10
				EndScale 15 10 15
				StartJitter 0 0 0
			End
			Geom FX_MusicRing_04
			LifeSpan 1
		End

		Event
			At Anchor2
			Type Local
			Bhvr ENEMYFX\Praetorian\Talons\Sirens\SeductiveMusic_Grow.bhvr
			BhvrOverride
				PyrRotateJitter 10 90 10
				PositionOffset 0 -1 0
				EndScale 15 10 15
			End
			Geom FX_MusicRing_04
			LifeSpan 1
		End

	End

	Condition
		On Time
		Time 30

		Event
			At Head
			Type Local
			Part ENEMYFX\Praetorian\Talons\Sirens\AOE15_Glow.part
			Part ENEMYFX\Praetorian\Talons\Sirens\AOE15_OuterRing.part
			LifeSpan 30
		End

		Event
			At Root
			Type Local
			#Part ENEMYFX\Praetorian\Talons\Sirens\AOE15_Glow.part
			Part ENEMYFX\Praetorian\Talons\Sirens\AOE15_OuterRing_Flat.part
			LifeSpan 30
		End

	End

	Condition
		On Time
		Time 40

		Event
			At Head
			Type Local
			Part ENEMYFX\Praetorian\Talons\Sirens\AOE15_StreaksOut_Large.part
			Part ENEMYFX\Praetorian\Talons\Sirens\AOE15_StreaksOut.part
			Part ENEMYFX\Praetorian\Talons\Sirens\AOE15_NotesA.part
			Part ENEMYFX\Praetorian\Talons\Sirens\AOE15_NotesB.part
			LifeSpan 50
		End

	End

End
