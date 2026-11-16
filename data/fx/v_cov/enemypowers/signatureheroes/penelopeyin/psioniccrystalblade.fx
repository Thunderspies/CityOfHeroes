#
FxInfo
Lifespan 100

###########################################################

	Condition
		On Time
		Time 5

		Event
			At HandR
			Type Local
			BhvrOverride
				PositionOffset 0 0.1 0
			End
			Part :PsionicBlade_Glow.part
			Part :PsionicBlade_Arcs.part
		End

		Event
			At HandL
			Type Local
			Bhvr :BladeFade.bhvr
			BhvrOverride
				PositionOffset 0 0.1 0
			End
			Part :PsionicBlade_Glow.part
			Part :PsionicBlade_Arcs.part
		End
	End

	Condition
		On Time
		Time 5

		Event
			At Origin
			Type Local
			ChildFx :PsionicCrystalBlade_CHILD.fx
		End

	End

End
