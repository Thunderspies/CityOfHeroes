###########################################################

FxInfo

Flags 	DontSuppress
LifeSpan 40

###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	Dog_TerrifyingRoar_01 100 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			At Head
			Type Local
			EName TunnelAnchor
			BhvrOverride
				PositionOffset 0 -0.5 0
			End
			Part :ChillingHowl_Smoke_Large.part
			Part :ChillingHowl_Snow_Large.part
			Part :ChillingHowl_Base_Large.part
			Part :ChillingHowl_Base_Snow.part
			Part :ChillingHowl_Icicles.part

		End
	End

End
