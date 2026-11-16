#
FxInfo

	LifeSpan 100


###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	TW_DefensiveSweep_HitSlow_01 100 75 .8
	End
End

	Condition
		On Time
		Time 3

		Event
			At Chest
			Type Local
			Part :Head_Shockwave_Tiny.part
			Part :Hit_Spikes_Small.part
			Part :Hit_Spikes_Horizontal.part
			Part :Hit_Spikes_Blur.part
			LifeSpan 2
		End

	End

End
