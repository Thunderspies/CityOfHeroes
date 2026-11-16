#
FxInfo

	Flags DontSuppress

	LifeSpan 60


###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		SoundNoRepeat 1
		Sound 	DC_Haunt_Scare_01 100 100 .8
		Sound 	DC_Haunt_Scare_02 100 100 .8
		Sound 	DC_Haunt_Scare_03 100 100 .8
	End
End


	Condition
		On Cycle
		Time 5

		Event
			At Head
			Type Local
			Bhvr CustomizeablePowers\DarknessControl\Scare_Tunnel.bhvr
			Geom Tintable_DarkGrasp_Flipped
			LifeSpan 15
		End

	End

End
