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
		Time 15

		Event
			At Head
			Type Start
			EName TunnelAnchor
			BhvrOverride
				PositionOffset 0 -0.5 0
			End
			Part :ChillingHowl_Base.part
			Part :ChillingHowl_Base_Snow.part
			Lifespan 10
		End

		Event
			At Head
			Type Start
			EName TunnelAnchor
			BhvrOverride
				PositionOffset 0 -0.5 0
			End
			Part :ChillingHowl_VolumeCone.part
			Lifespan 15
		End

		Event
			At Head
			Type Start
			EName TunnelAnchor
			BhvrOverride
				PositionOffset 0 -0.5 0
			End
			Part :ChillingHowl_Smoke.part
			Part :ChillingHowl_Snow.part
		End
	End

###########################################################

End
