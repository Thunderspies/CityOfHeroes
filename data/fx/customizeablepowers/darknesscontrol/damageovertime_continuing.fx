#
FxInfo

	Flags DontSuppress


###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	DC_DarkGrasp_Hit_Loop 100 100 .3
		bhvr	behaviors/Sound/SoundIn1.bhvr
		lifespan 500
	End
End



	Condition
		On Time
		Time 0

		Event
			At Chest
			Type PositOnly
			Part CustomizeablePowers\DarknessControl\DarkGrasp_ShadowCore_Solid.part
		End

	End

End
